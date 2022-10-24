#ifndef PHYSICSENGINE_PHYSICSCONTACT_H
#define PHYSICSENGINE_PHYSICSCONTACT_H

#include "PhysicsObject.h"

/*
 * A contact representing two PhysicsObjects in contact / colliding.
 * Resolving a contact removes their interpenetration and applies
 * enough impulse to keep them apart. They may also rebound.
 *
 * The contact itself has no callable functions; use a
 * ContactResolver to resolve a set of contacts.
 */
class PhysicsContact {

public:

    /*
     * Holds the particles involved in the contact.
     * The second may be a nullptr for contacts with scenery.
     */
    PhysicsObject* objects[2];

    /*
     * Holds the coefficient of restitution
     */
    real restitution;

    /*
     * Holds the direction of the contact (in world coordinates)
     */
    Vector3 contactNormal;

    /*
     * Holds the depth of penetration at the contact
     */
    real penetration;

protected:

    /*
     * Resolves the contact for both velocity and interpenetration
     */
    virtual void resolve(real deltaTime) = 0;

    /*
     * Calculates the separating velocity at this contact
     */
    virtual real calculateSeparatingVelocity() const = 0;

    /*
     * Handles the impulse calculations for this collision
     */
    virtual void resolveVelocity(real deltaTime) = 0;

    /*
     * Handles the interpenetration resolution for this contact.
     */
    virtual void resolveInterpenetration(real deltaTime) = 0;

};

class ParticleContact : public PhysicsContact {
protected:
    void resolve(real deltaTime) override;
    real calculateSeparatingVelocity() const override;
    void resolveVelocity(real deltaTime) override;
    void resolveInterpenetration(real deltaTime) override;
};



#endif //PHYSICSENGINE_PHYSICSCONTACT_H
