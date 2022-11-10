#ifndef PHYSICSENGINE_OBJECTLINK_H
#define PHYSICSENGINE_OBJECTLINK_H

#include "PhysicsObject.h"
#include "PhysicsContact.h"
#include "ContactGenerator.h"

/*
 * Links connect two PhysicsObjects together, generating a contact if
 * they violate the constraints of their link. It is used as a
 * base class for cables and rods.
 */
class ObjectLink : public ContactGenerator {
public:
    /*
     * Holds the pair of objects that are connected by this link.
     */
    PhysicsObject* objects[2];

    ObjectLink(PhysicsObject* obj1, PhysicsObject* obj2);

protected:
    /*
     * Returns the current length of the link. By default
     * uses the objects' centers.
     */
    virtual real currentLength() const;

};

/*
 * Cables link a pair of PhysicsObjects' centers, generating a
 * contact if they stray too far apart.
 */
class ParticleCable : public ObjectLink {
public:
    /*
     * Holds the maximum length of the cable.
     */
    real maxLength;

    /*
     * Holds the restitution (bounciness) of the cable.
     */
    real restitution;

    ParticleCable(PhysicsObject* obj1, PhysicsObject* obj2, real maxLength, real restitution);

public:
    /*
     * Fills out the contact's fields with values that will prevent
     * the cable from overextending.
     */

    unsigned int addContact(PhysicsContact* contact, unsigned int limit) const override;

};

class ParticleRod : public ObjectLink {
public:
    /*
     * Holds the length of the rod.
     */
    real length;

    ParticleRod(PhysicsObject *obj1, PhysicsObject *obj2, real length);

public:
    /*
     * Fills out the contact's fields with values that will prevent
     * the rod from extending or compressing.
     */
    unsigned int addContact(PhysicsContact* contact, unsigned int limit) const override;

};

#endif //PHYSICSENGINE_OBJECTLINK_H
