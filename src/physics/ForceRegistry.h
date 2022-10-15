#ifndef PHYSICSENGINE_FORCEREGISTRY_H
#define PHYSICSENGINE_FORCEREGISTRY_H

#include "ForceGenerator.h"
#include "PhysicsObject.h"
#include <vector>

class ForceRegistry {
private:
    /*
     * Keeps track of one force generator and
     * its corresponding PhysicsObject
     */
    struct ForceRegistration {
        PhysicsObject* object;
        ForceGenerator* forceGenerator;

        bool operator==(const ForceRegistration& other);
    };

    /*
     * The list of ForceRegistrations
     */
    std::vector<ForceRegistration> registrations;

public:
    /*
     * Registers a ForceGenerator with its PhysicsObject
     */
    void add(PhysicsObject* object, ForceGenerator* fg);

    /*
     * Removes a pair from the registry. If absent, does nothing
     */
    void remove(PhysicsObject* object, ForceGenerator* fg);

    /*
     * Clears the registry. Does not delete the objects themselves
     */
    void clear();

    /*
     * Calls the ForceGenerator::updateForce()
     */
    void updateForces(real deltaTime);

};


#endif //PHYSICSENGINE_FORCEREGISTRY_H
