#ifndef PHYSICSENGINE_PHYSICSWORLD_H
#define PHYSICSENGINE_PHYSICSWORLD_H

#include <vector>
#include "ForceRegistry.h"
#include "PhysicsContactResolver.h"
#include "ContactGenerator.h"

class PhysicsWorld {

private:
    std::vector<PhysicsObject*> objects;
    std::vector<ForceGenerator*> forces;
    std::vector<ContactGenerator*> contactGenerators;
    ForceRegistry forceRegistry;
    ParticleContactResolver contactResolver;

    /*
     * Holds the maximum number of allowed contacts
     * (the size of the contacts array).
     */
    unsigned int maxContacts;

    /*
     * Holds the list of contacts.
     */
    ParticleContact* contacts;

    /*
     * Whether to calculate the number of contact iterations
     * based on the number of contacts
     */
    bool calculateContactIterations;

    /*
     * Calls each of the registered contact generators to report
     * their contacts. Returns the number of generated contacts.
     */
    unsigned int generateContacts();

public:
    /*
     * Creates a new simulator that can handle up to the given number of contacts
     * per frame. You can also optionally give a number of contact-resolution
     * iterations to use, or else twice the number of contacts will be used.
     */
    explicit PhysicsWorld(unsigned int maxContacts, unsigned int contactIterations=0);

    ~PhysicsWorld();

    void update(real deltaTime);

    /*
     * Calculate the number of vertices and indices used by the PhysicsObject models
     */
    void writeVertexAndIndexCounts(unsigned int &vertexCount, unsigned int &indexCount) const;

    /*
     * Write PhysicsObject rendering data to the relevant arrays.
     */
    void writeObjectData(bool flatShaded, bool initialWrite, Vector3* positions, VertexColor* colors, GLuint* indices, int &vertexIdx, int &indexIdx) const;


    /*
     * Adds a PhysicsObject to the world.
     */
    void addObject(PhysicsObject* object);

    /*
     * Adds a ForceGenerator to the world.
     */
    void addForceGenerator(ForceGenerator* fg);

    /*
     * Registers a force to apply to a PhysicsObject.
     */
    void applyForceToObject(PhysicsObject *obj, ForceGenerator* fg);

    /*
     * Adds a ContactGenerator to the world.
     */
    void addContactGenerator(ContactGenerator* cg);
};


#endif //PHYSICSENGINE_PHYSICSWORLD_H
