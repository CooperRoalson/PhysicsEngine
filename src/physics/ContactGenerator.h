#ifndef PHYSICSENGINE_CONTACTGENERATOR_H
#define PHYSICSENGINE_CONTACTGENERATOR_H

#include "PhysicsContact.h"

/*
 * Interface for creating contacts between PhysicsObjects
 */
class ContactGenerator {

public:
    /*
     * Fills the given contact structure with the generated
     * contact. The contact pointer should point to the first
     * available contact in a contact array, where limit is the
     * maximum number of contacts in the array that can be written
     * to. The method returns the number of contacts that have
     * been written.
     */
    virtual unsigned int addContact(PhysicsContact* contact, unsigned int limit) const = 0;

};

class FloorContactGenerator : public ContactGenerator {

public:
    PhysicsObject* object;
    real floorY;

    real restitution;

    FloorContactGenerator(PhysicsObject* object, real floorY, real restitution);

    unsigned int addContact(PhysicsContact* contact, unsigned int limit) const override;

};


#endif //PHYSICSENGINE_CONTACTGENERATOR_H
