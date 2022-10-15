#include "PhysicsObject.h"

#include <cmath>

const real PhysicsObject::DAMPING(0.95f);

bool hasFiniteMass();

PhysicsObject::PhysicsObject(Vector3 pos, Vector3 vel, real inverseMass, Shape model) : position(pos), velocity(vel), inverseMass(inverseMass), model(model) {}

real PhysicsObject::getInverseMass() const {return inverseMass;}
bool PhysicsObject::hasFiniteMass() const {return inverseMass > 0.0f;}
real PhysicsObject::getMass() const {return hasFiniteMass() ? 1/inverseMass : 0;}

Vector3 PhysicsObject::getPosition() const {return position;}
Vector3 PhysicsObject::getVelocity() const {return velocity;}

const Shape& PhysicsObject::getModel() const {return model;}

Matrix4 PhysicsObject::getModelMatrix() const {
    return Matrix4().translate(position);
}

void PhysicsObject::update(real deltaTime) {
    if (!hasFiniteMass()) {return;}

    position += velocity*deltaTime;

    // a = F/m
    Vector3 acceleration = forceAccumulator * inverseMass;

    velocity += acceleration*deltaTime;
    velocity *= real_pow(DAMPING, deltaTime);

    if (position.y <= 0.0f) {velocity.y *= -1.0f;}

    clearForceAccumulator();
}

void PhysicsObject::addForce(Vector3 force) { if (hasFiniteMass()) {forceAccumulator += force*inverseMass;} }

void PhysicsObject::clearForceAccumulator() {
    forceAccumulator = Vector3();
}

const real Particle::RADIUS = 0.2;
const int Particle::SMOOTHNESS = 2;

Particle::Particle(Vector3 pos, Vector3 vel, real inverseMass, VertexColor color) : PhysicsObject(pos,vel,inverseMass,Shape::icosphere(Vector3(),Particle::RADIUS, color, Particle::SMOOTHNESS)) {}
