#include "ObjectLink.h"

real ObjectLink::currentLength() const {
    return (objects[1]->getPosition() - objects[0]->getPosition()).magnitude();
}

ObjectLink::ObjectLink(PhysicsObject* obj1, PhysicsObject* obj2) {
    objects[0] = obj1;
    objects[1] = obj2;
}

unsigned int ParticleCable::addContact(PhysicsContact *contact, unsigned int limit) const {

    // Find the length of the cable
    real length = currentLength();

    // Check if it's overextended.
    if (length < maxLength) {
        return 0;
    }

    // Otherwise, fill out the contact
    contact->objects[0] = objects[0];
    contact->objects[1] = objects[1];

    // Calculate the normal
    contact->contactNormal = (objects[1]->getPosition() - objects[0]->getPosition()).normalized();

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    return 1;

}

ParticleCable::ParticleCable(PhysicsObject *obj1, PhysicsObject *obj2, real maxLength, real restitution) : ObjectLink(obj1, obj2), maxLength(maxLength), restitution(restitution) {}

unsigned int ParticleRod::addContact(PhysicsContact *contact, unsigned int limit) const {

    // Find the length of the rod
    real currentLen = currentLength();

    // Check if it's already the correct length.
    if (currentLen == length) {
        return 0;
    }

    // Otherwise, fill out the contact
    contact->objects[0] = objects[0];
    contact->objects[1] = objects[1];

    // Calculate the normal
    Vector3 normal = (objects[1]->getPosition() - objects[0]->getPosition()).normalized();

    // The normal depends on if it's extending or compressing
    if (currentLen > length) {
        contact->contactNormal = normal;
        contact->penetration = currentLen - length;
    } else {
        contact->contactNormal = -normal;
        contact->penetration = length - currentLen;
    }

    // Always use 0 restitution (no bounciness)
    contact->restitution = 0;

    return 1;

}

ParticleRod::ParticleRod(PhysicsObject *obj1, PhysicsObject *obj2, real length) : ObjectLink(obj1, obj2), length(length) {}
