//
// Created by Cooper Roalson on 10/19/22.
//

#include "PhysicsContact.h"

void ParticleContact::resolve(real deltaTime) {
    resolveVelocity(deltaTime);
    resolveInterpenetration(deltaTime);
}

real ParticleContact::calculateSeparatingVelocity() const {
    Vector3 relativeVelocity = objects[0]->getVelocity()
            - (objects[1] ? objects[1]->getVelocity() : Vector3());
    return relativeVelocity.dot(contactNormal);
}

void ParticleContact::resolveVelocity(real deltaTime) {
    // Find the velocity in the direction of the contact
    real separatingVelocity = calculateSeparatingVelocity();

    // Check if it needs to be resolved
    if (separatingVelocity > 0) {
        // No impulse required
        return;
    }

    // Calculate new separating velocity
    real newSepVelocity = -separatingVelocity * restitution;

    // Check the velocity buildup due to acceleration only
    //Vector3 accCausedVelocity =

    real deltaVelocity = newSepVelocity - separatingVelocity;

    // We apply the change in velocity to each object in
    // proportion to their inverse mass
    real totalInverseMass = objects[0]->getInverseMass()
            + (objects[1] ? objects[1]->getInverseMass() : 0);

    // If all particles have infinite mass, impulses have no effect
    if (totalInverseMass <= 0) { return; }

    // Calculate the amount of impulse to apply
    real impulse = deltaVelocity / totalInverseMass;

    // Find the impulse per unit of inverse mass
    Vector3 impulsePerIMass = contactNormal * impulse;

    // Apply impulses, which go in the direction of the contact
    objects[0]->setVelocity(objects[0]->getVelocity() + impulsePerIMass * objects[0]->getInverseMass());
    if (objects[1]) {
        // Opposite direction to object 0
        objects[1]->setVelocity(objects[1]->getVelocity() - impulsePerIMass * objects[1]->getInverseMass());
    }
}

void ParticleContact::resolveInterpenetration(real deltaTime) {
    // If there isn't penetration, skip this
    if (penetration <= 0) { return; }

    // The movement of each object is based on their inverse mass, so find the total
    real totalInverseMass = objects[0]->getInverseMass()
                            + (objects[1] ? objects[1]->getInverseMass() : 0);

    // If all particles have infinite mass, do nothing
    if (totalInverseMass <= 0) { return; }

    // Find the amount of penetration per unit of inverse math
    Vector3 movePerIMass = contactNormal * (penetration / totalInverseMass);

    // Calculate the movement amounts
    objects[0]->setPosition(objects[0]->getPosition() + movePerIMass * objects[0]->getInverseMass());
    if (objects[1]) {
        // Opposite direction to object 0
        objects[1]->setVelocity(objects[1]->getVelocity() - movePerIMass * objects[1]->getInverseMass());
    }
}
