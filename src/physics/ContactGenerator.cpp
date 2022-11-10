#include "ContactGenerator.h"

FloorContactGenerator::FloorContactGenerator(PhysicsObject *object, real floorY, real restitution) : object(object), floorY(floorY), restitution(restitution) {}

unsigned int FloorContactGenerator::addContact(PhysicsContact *contact, unsigned int limit) const {
    real y = object->getPosition().y;

    if (y > floorY) { return 0; }

    contact->objects[0] = object;
    contact->objects[1] = nullptr;
    contact->penetration = floorY - y;
    contact->contactNormal = Vector3::UP;
    contact->restitution = restitution;

    return 1;
}

