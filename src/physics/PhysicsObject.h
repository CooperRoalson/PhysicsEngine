#ifndef PHYSICSENGINE_PHYSICSOBJECT_H
#define PHYSICSENGINE_PHYSICSOBJECT_H


#include "../core/Vector3.h"
#include "../render/Shape.h"
#include "../core/Matrix4.h"

class PhysicsObject {
private:
    Vector3 position;
    Vector3 velocity;

    /*
     * Stores accumulated force to apply at next iteration.
     * Cleared afterward
     */
    Vector3 forceAccumulator;

    real inverseMass;

    Shape model;

    // Does the object experience damping?
    bool damping;

    // Clears the force accumulator. Called after each integration step
    void clearForceAccumulator();

public:
    static const real DAMPING;

    PhysicsObject(Vector3 pos, Vector3 vel, real inverseMass, bool damping, Shape model);

    bool hasFiniteMass() const;
    real getInverseMass() const;
    real getMass() const;
    Vector3 getPosition() const;
    Vector3 getVelocity() const;

    const Shape& getModel() const;
    Matrix4 getModelMatrix() const;

    /*
     * Updates the object's position and velocity based on a time duration of `timeDelta`
     */
    virtual void update(real deltaTime);

    void addForce(Vector3 force);
    void setVelocity(Vector3 vel);
    void setPosition(Vector3 vel);

};

class Particle : public PhysicsObject {
public:
    static const real RADIUS;
    static const int SMOOTHNESS;

    Particle(Vector3 pos, Vector3 vel, real inverseMass, bool damping, VertexColor color);
};


#endif //PHYSICSENGINE_PHYSICSOBJECT_H
