#include "BVHTree.h"
#include "RigidBody.h"

bool BVHTree::BVHNode::isLeaf() const {
    return body != nullptr;
}

BVHTree::BVHNode::BVHNode(BVHTree::BVHNode *parent, BoundingSphere volume, RigidBody* body) : parent(parent), volume(volume), body(body), children{nullptr, nullptr} {}

unsigned int BVHTree::getPotentialContacts(const BVHTree::BVHNode *node, PotentialContact *contacts, unsigned int limit) const {
    if (limit == 0 || node->isLeaf()) {return 0;}

    unsigned int count = getPotentialContactsBetween(node->children[0], node->children[1], contacts, limit);

    if (count < limit) {
        count += getPotentialContacts(node->children[0], contacts + count, limit - count);
        if (count < limit) {
            count += getPotentialContacts(node->children[1], contacts + count, limit - count);
        }
    }
    return count;
}

bool BVHTree::overlaps(const BVHTree::BVHNode *node1, const BVHTree::BVHNode *node2) const {
    return node1->volume.overlaps(&node2->volume);
}

unsigned int BVHTree::getPotentialContactsBetween(const BVHTree::BVHNode *node1, const BVHTree::BVHNode *node2, PotentialContact *contacts, unsigned int limit) const {
    if (limit == 0 || !overlaps(node1, node2)) {return 0;}
    // If we've reached 2 leaf nodes that overlap, they might be in contact
    if (node1->isLeaf() && node2->isLeaf()) {
        contacts[0].bodies[0] = node1->body;
        contacts[0].bodies[1] = node2->body;
        return 1;
    }

    // Pick one node to descend into: either the non-leaf, or
    // the larger one if they're both branches
    bool descendIntoFirst = node2->isLeaf() || (!node1->isLeaf() && node1->volume.radius >= node2->volume.radius);
    const BVHNode* splitNode = descendIntoFirst ? node1 : node2;
    const BVHNode* otherNode = descendIntoFirst ? node2 : node1;

    unsigned int count = getPotentialContactsBetween(splitNode->children[0], otherNode, contacts, limit);
    // Check if there's room for the other side
    if (limit > count) {
        return count + getPotentialContactsBetween(splitNode->children[1], otherNode, contacts + count, limit - count);
    } else {
        return count;
    }

}

void BVHTree::insert(BVHTree::BVHNode *node, RigidBody *body) {
    BoundingSphere newVolume = body->getBoundingSphere();
    // If node is a leaf, create 2 children and put the body in one
    if (node->isLeaf()) {
        // Push current node to first child
        node->children[0] = new BVHNode(node, node->volume, node->body);
        // Create new child for new body
        node->children[1] = new BVHNode(node, newVolume, body);
        // Set current node to non-leaf
        node->body = nullptr;

        recalculateBoundingVolume(node);
    } else {
        // Otherwise, choose the child node that grows the least
        if (node->children[0]->volume.getGrowth(newVolume) < node->children[1]->volume.getGrowth(newVolume)) {
            insert(node->children[0], body);
        } else {
            insert(node->children[1], body);
        }
    }
}

void BVHTree::recalculateBoundingVolume(BVHTree::BVHNode *node) {
    node->volume = BoundingSphere(node->children[0]->volume, node->children[1]->volume);
    if (node->parent) {
        recalculateBoundingVolume(node->parent);
    }
}

void BVHTree::deleteNode(BVHNode *node) {
    if (node->parent) {
        BVHNode* sibling = node->parent->children[0] == node ? node->parent->children[1] : node->parent->children[0];

        // Replace parent with sibling
        node->parent->volume = sibling->volume;
        node->parent->body = sibling->body;
        node->parent->children[0] = sibling->children[0];
        node->parent->children[1] = sibling->children[1];
        delete sibling;

        recalculateBoundingVolume(node->parent->parent);
    }

    // Delete the children by first removing their parent to prevent repetition
    if (node->children[0]) {
        node->children[0]->parent = nullptr;
        deleteNode(node->children[0]);
    }
    if (node->children[1]) {
        node->children[1]->parent = nullptr;
        deleteNode(node->children[1]);
    }

    delete node;

}

BVHTree::BVHTree() : root(nullptr) {}

BVHTree::~BVHTree() {
    if (root) {
        deleteNode(root);
    }
}

void BVHTree::insert(RigidBody *body) {
    if (!root) {
        root = new BVHNode(nullptr, body->getBoundingSphere(), body);
    } else {
        insert(root, body);
    }
}

bool BVHTree::remove(RigidBody *body) {
    BVHNode* node = findRigidBody(root, body);
    if (node) { deleteNode(node); return true; }
    return false;
}

BVHTree::BVHNode *BVHTree::findRigidBody(BVHTree::BVHNode *node, RigidBody *body) const {
    if (node->body == body) {return node;}

    BVHNode* result;
    if (node->children[0] && (result = findRigidBody(node->children[0], body))) {return result;}
    if (node->children[1] && (result = findRigidBody(node->children[1], body))) {return result;}

    return nullptr;
}

unsigned int BVHTree::getPotentialContacts(PotentialContact *contacts, unsigned int limit) const {
    if (!root) { return 0; }
    return getPotentialContacts(root, contacts, limit);
}

void BVHTree::print(BVHNode* node, unsigned int level) const {
    for (int i = 0; i < level; i++) {
        std::cout << "| ";
    }
    if (node->body) {
        std::cout << *node->body << std::endl;
    } else {
        std::cout << "BoundingSphere(r=" << node->volume.radius << ")\n";
        print(node->children[0], level+1);
        print(node->children[1], level+1);
    }
}

void BVHTree::print() const {
    print(root, 0);
}

bool BoundingSphere::overlaps(const BoundingSphere* other) const {
    return (other->center-center).magnitudeSquared() <= (radius+other->radius)*(radius+other->radius);
}

real BoundingSphere::getSize() const {
    return 4*M_PI/3 * radius*radius*radius;
}

BoundingSphere::BoundingSphere(Vector3 center, real radius) : center(center), radius(radius) {}

BoundingSphere::BoundingSphere(BoundingSphere &b1, BoundingSphere &b2) {
    Vector3 offset = b2.center - b1.center;
    real distanceSquared = offset.magnitudeSquared();
    real radiusDiff = b2.radius - b1.radius;

    // Check if one sphere encompasses the other
    if (radiusDiff*radiusDiff >= distanceSquared) {
        if (b1.radius > b2.radius) {
            center = b1.center;
            radius = b1.radius;
        } else {
            center = b2.center;
            radius = b2.radius;
        }
    } else {
        real distance = sqrt(distanceSquared);
        radius = (real) 0.5 * (distance + b1.radius + b2.radius);

        // The new center is interpolated between the two centers based on the radii
        center = b1.center + (distance > 0 ? offset * ((radius - b1.radius) / distance) : Vector3());
    }
}

real BoundingSphere::getGrowth(BoundingSphere &newSphere) const {
    BoundingSphere thisSphere = *this;
    return BoundingSphere(thisSphere, newSphere).getSize() - getSize();
}

BoundingSphere::BoundingSphere() {}
