#include "PhysicsWorld.h"
#include "../render/MainWindow.h"

PhysicsWorld::~PhysicsWorld() {
    for (PhysicsObject* obj : objects) {delete obj;}
    for (ForceGenerator* fg : forces) {delete fg;}
    delete contacts;
}

void PhysicsWorld::writeObjectData(bool flatShaded, bool initialWrite, Vector3* positions, VertexColor* colors, GLuint* indices, int &vertexIdx, int &indexIdx) const {
    for (PhysicsObject* obj : objects) {
        MainWindow::writeShape(obj->getShape(), obj->getShapeMatrix(), flatShaded, initialWrite, positions, colors, indices, vertexIdx, indexIdx);
    }

    Renderable* r;
    for (ForceGenerator* fg : forces) {
        if ((r = dynamic_cast<Renderable*>(fg)) != nullptr) {
            MainWindow::writeShape(r->getShape(), Matrix4(), flatShaded, initialWrite, positions, colors, indices, vertexIdx, indexIdx);
        }
    }
    for (ContactGenerator* cg : contactGenerators) {
        if ((r = dynamic_cast<Renderable*>(cg)) != nullptr) {
            MainWindow::writeShape(r->getShape(), Matrix4(), flatShaded, initialWrite, positions, colors, indices, vertexIdx, indexIdx);
        }
    }
}


void PhysicsWorld::writeVertexAndIndexCounts(unsigned int &vertexCount, unsigned int &indexCount) const {
    vertexCount = indexCount = 0;
    for (PhysicsObject* obj : objects) {
        vertexCount += obj->getShape().numVertices();
        indexCount += obj->getShape().numIndices();
    }

    Renderable* r;
    for (ForceGenerator* fg : forces) {
        if ((r = dynamic_cast<Renderable*>(fg)) != nullptr) {
            vertexCount += r->getShape().numVertices();
            indexCount += r->getShape().numIndices();
        }
    }
    for (ContactGenerator* cg : contactGenerators) {
        if ((r = dynamic_cast<Renderable*>(cg)) != nullptr) {
            vertexCount += r->getShape().numVertices();
            indexCount += r->getShape().numIndices();
        }
    }
}

void PhysicsWorld::update(real deltaTime) {
    // Apply the force generators
    forceRegistry.updateForces(deltaTime);

    // Update the objects
    for (PhysicsObject* obj : objects) {
        obj->update(deltaTime);
    }

    // Generate the contacts
    unsigned int usedContacts = generateContacts();

    // Process the contacts
    if (usedContacts) {
        if (calculateContactIterations) { contactResolver.setIterations(usedContacts*2); }
        contactResolver.resolveContacts(contacts, usedContacts, deltaTime);
    }
}

PhysicsWorld::PhysicsWorld(unsigned int maxContacts, unsigned int contactIterations) : maxContacts(maxContacts),contactResolver(contactIterations) {
    contacts = new ParticleContact[maxContacts];
    calculateContactIterations = (contactIterations == 0);
}

unsigned int PhysicsWorld::generateContacts() {
    unsigned int limit = maxContacts;
    PhysicsContact* nextContact = contacts;

    for (ContactGenerator* g : contactGenerators) {
        unsigned int used = g->addContact(nextContact, limit);
        limit -= used;
        nextContact += used;

        // If we've run out of contacts to fill, skip the rest
        if (limit <= 0) { break; }
    }

    // Return the number of contacts used
    return maxContacts - limit;

}

void PhysicsWorld::addObject(PhysicsObject *object) { objects.push_back(object); }

void PhysicsWorld::addForceGenerator(ForceGenerator *fg) { forces.push_back(fg); }

void PhysicsWorld::applyForceToObject(PhysicsObject *obj, ForceGenerator *fg) { forceRegistry.add(obj, fg); }

void PhysicsWorld::addContactGenerator(ContactGenerator* cg) { contactGenerators.push_back(cg); }
