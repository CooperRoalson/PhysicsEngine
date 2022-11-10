#include "PhysicsWorld.h"

PhysicsWorld::~PhysicsWorld() {
    for (PhysicsObject* obj : objects) {delete obj;}
    for (ForceGenerator* fg : forces) {delete fg;}
    delete contacts;
}

void PhysicsWorld::writeObjectData(bool flatShaded, bool initialWrite, Vector3* positions, VertexColor* colors, GLuint* indices, int &vertexIdx, int &indexIdx) const {
    for (PhysicsObject* obj : objects) {
        if (obj->getModel().isFlatShaded() ^ flatShaded) {continue;}
        obj->getModel().writeVertexPositionsAndNormals(positions + vertexIdx*2,obj->getModelMatrix());
        if (initialWrite) {
            obj->getModel().writeVertexColors(colors + vertexIdx);
            obj->getModel().writeIndices(indices + indexIdx, vertexIdx);
        }
        vertexIdx += obj->getModel().numVertices();
        indexIdx += obj->getModel().numIndices();
    }
}

void PhysicsWorld::writeVertexAndIndexCounts(unsigned int &vertexCount, unsigned int &indexCount) const {
    vertexCount = indexCount = 0;
    for (PhysicsObject* obj : objects) {
        vertexCount += obj->getModel().numVertices();
        indexCount += obj->getModel().numIndices();
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
