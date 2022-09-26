#ifndef PHYSICSENGINE_PHYSICSOBJECT_H
#define PHYSICSENGINE_PHYSICSOBJECT_H


#include "../core/Vector3.h"
#include "../render/Shape.h"
#include "../core/Matrix4.h"

class PhysicsObject {
private:
    Vector3 position;
    Vector3 velocity;
    Vector3 acceleration;

    real inverseMass;

    Shape model;

public:
    static const Vector3 GRAVITY;
    static const real DAMPING;

    PhysicsObject(Vector3 pos, Vector3 vel, real inverseMass, Shape model);

    bool hasFiniteMass() const;
    real getInverseMass() const;
    real getMass() const;
    Vector3 getPosition() const;
    Vector3 getVelocity() const;
    Vector3 getAcceleration() const;

    const Shape& getModel() const;
    Matrix4 getModelMatrix() const;

    void update(real timeDelta);

    void addAcceleration(Vector3 acc);
    void addForce(Vector3 force);
    void clearForceAccumulator();
};

class Particle : public PhysicsObject {
public:
    static const real RADIUS;
    static const int SMOOTHNESS;

    Particle(Vector3 pos, Vector3 vel, real inverseMass, VertexColor color);
};


#endif //PHYSICSENGINE_PHYSICSOBJECT_H
