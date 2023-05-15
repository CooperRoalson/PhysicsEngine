#ifndef PHYSICSENGINE_BVHTREE_H
#define PHYSICSENGINE_BVHTREE_H
#include "../math/Vector3.h"

// Avoid circular dependency
class RigidBody;

/*
 * A spherical volume meant to completely encompass a RigidBody
 */
struct BoundingSphere {
    Vector3 center;
    real radius;

    BoundingSphere();
    BoundingSphere(Vector3 center, real radius);

    /*
     * Creats a BoundingSphere that fully encompasses two others.
     */
    BoundingSphere(BoundingSphere &b1, BoundingSphere &b2);

    bool overlaps(const BoundingSphere* other) const;
    real getSize() const;

    real getGrowth(BoundingSphere &newSphere) const;
};

/*
 * A "bounding volume hierarchy" tree structure that performs
 * broad-phase collision checks between BoundingSpheres
 */
class BVHTree {
private:
    struct PotentialContact {
        /*
         * Holds the bodies that might be in contact.
         */
        RigidBody* bodies[2];
    };

    struct BVHNode {
        /*
         * Holds this node's child nodes
         */
        BVHNode* children[2];

        /*
         * Holds this node's parent node
         */
        BVHNode* parent;

        /*
         * Holds a single bounding sphere encompassing
         * all the descendants of this node.
         */
        BoundingSphere volume;

        /*
         * If this node is a leaf, holds its associated RigidBody.
         */
        RigidBody* body;

        BVHNode(BVHNode* parent, BoundingSphere volume, RigidBody* body);

        bool isLeaf() const;
    };

    /*
     * Returns whether two nodes' bounding volumes overlap.
     */
    bool overlaps(const BVHNode* node1, const BVHNode* node2) const;

    /*
     * Writes the potential contacts from node down into the contacts array,
     * up to limit. Returns the number written.
     */
    unsigned int getPotentialContacts(const BVHNode* node, PotentialContact* contacts, unsigned int limit) const;

    unsigned int getPotentialContactsBetween(const BVHNode *node1, const BVHNode *node2, PotentialContact *contacts, unsigned int limit) const;

    /*
     * Inserts a RigidBody into the hierarchy.
     */
    void insert(BVHNode* node, RigidBody* body);

    void recalculateBoundingVolume(BVHNode *node);

    /*
     * Deletes a node, removing it from the hierarchy (along with its children)
     * and replacing its parent with its sibling. This also recalculates
     * the bounding volumes of the parent nodes.
     */
    void deleteNode(BVHNode* node);

    BVHNode* findRigidBody(BVHNode* node, RigidBody* body) const;

    BVHNode* root;

public:
    BVHTree();

    ~BVHTree();

    /*
     * Inserts a RigidBody into the hierarchy.
     */
    void insert(RigidBody* body);

    /*
     * Deletes a RigidBody from the hierarchy. Returns whether the RigidBody was found.
     */
    bool remove(RigidBody* body);

    /*
     * Writes the potential contacts from the hierarchy into an array
     * and returns the number written, up to limit.
     */
    unsigned int getPotentialContacts(PotentialContact* contacts, unsigned int limit) const;

};


#endif //PHYSICSENGINE_BVHTREE_H
