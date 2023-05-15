#include "PhysicsContactResolver.h"


PhysicsContactResolver::PhysicsContactResolver(unsigned int iterations) : iterations(iterations) {}

void PhysicsContactResolver::setIterations(unsigned int iterations) { this->iterations = iterations; }

real PhysicsContactResolver::getContactSeparatingVelocity(PhysicsContact *contact) {
    return contact->calculateSeparatingVelocity();
}

void PhysicsContactResolver::resolveContact(PhysicsContact *contact, real deltaTime) {
    contact->resolve(deltaTime);
}


void ParticleContactResolver::resolveContacts(PhysicsContact* contactArray, unsigned int numContacts, real deltaTime) {
    iterationsUsed = 0;

    while (iterationsUsed < iterations) {

        // Find the contact with the largest closing velocity
        real maxVel = REAL_MAX;
        unsigned maxIndex = numContacts;
        for (unsigned int i = 0; i < numContacts; i++) {
            real sepVel = getContactSeparatingVelocity(contactArray+i);
            if (sepVel < maxVel && (sepVel < 0 || contactArray[i].penetration > 0)) {
                maxVel = sepVel;
                maxIndex = i;
            }
        }

        // Check if we have anything worth resolving
        if (maxIndex == numContacts) { break; }

        // Resolve the current contact
        resolveContact(contactArray+maxIndex, deltaTime);

        iterationsUsed++;

    }
}

ParticleContactResolver::ParticleContactResolver(unsigned int iterations) : PhysicsContactResolver(iterations) {}
