#ifndef PHYSICSENGINE_PHYSICSCONTACTRESOLVER_H
#define PHYSICSENGINE_PHYSICSCONTACTRESOLVER_H

#include "PhysicsContact.h"

/*
 * The contact resolution routine for particle contacts. One
 * resolver instance can be shared for the entire simulation.
 */
class PhysicsContactResolver {

protected:
    /*
     * Holds the number of iterations allowed.
     */
    unsigned int iterations;

    /*
     * This is a performance tracking value; we keep a record
     * of the actual number of iterations used.
     */
    unsigned int iterationsUsed;

    /*
     * These functions allow derived classes to access protected
     * PhysicsContact functions
     */
    static real getContactSeparatingVelocity(PhysicsContact* contact);
    static void resolveContact(PhysicsContact* contact, real deltaTime);

public:
    /*
     * Creates a new contact resolver.
     */
    explicit PhysicsContactResolver(unsigned int iterations);

    /*
     * Sets the number of iterations that can be used.
     */
    void setIterations(unsigned int iterations);

    /*
     * Resolves a set of particle contacts for both penetration
     * and velocity.
     */
    virtual void resolveContacts(PhysicsContact *contactArray, unsigned int numContacts, real deltaTime) = 0;

};

/*
 * A PhysicsContactResolver that can handle simple Particles
 */
class ParticleContactResolver : public PhysicsContactResolver {

public:
    /*
     * Creates a new particle contact resolver.
     */
    explicit ParticleContactResolver(unsigned int iterations);

    void resolveContacts(PhysicsContact *contactArray, unsigned int numContacts, real deltaTime) override;

};


#endif //PHYSICSENGINE_PHYSICSCONTACTRESOLVER_H
