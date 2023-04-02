#include "PhysicsObject.h"

#include <cmath>

const real PhysicsObject::DAMPING(0.9f);

bool hasFiniteMass();

PhysicsObject::PhysicsObject(Vector3 pos, Vector3 vel, real inverseMass, bool damping, Shape model) : position(pos), velocity(vel), inverseMass(inverseMass), model(model), damping(damping) {}

real PhysicsObject::getInverseMass() const {return inverseMass;}
bool PhysicsObject::hasFiniteMass() const {return inverseMass > 0.0f;}

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

    if (damping) { velocity *= real_pow(DAMPING, deltaTime); }

    clearForceAccumulator();
}

void PhysicsObject::addForce(Vector3 force) { if (hasFiniteMass()) {forceAccumulator += force;} }

void PhysicsObject::clearForceAccumulator() {
    forceAccumulator = Vector3();
}

void PhysicsObject::setVelocity(Vector3 vel) {
    velocity = vel;
}

void PhysicsObject::setPosition(Vector3 pos) {
    position = pos;
}

const real Particle::RADIUS = 0.2;
const int Particle::SMOOTHNESS = 2;

Particle::Particle(Vector3 pos, Vector3 vel, real inverseMass, bool damping, VertexColor color) : PhysicsObject(pos,vel,inverseMass,damping,Shape::icosphere(Vector3(),Particle::RADIUS, color, Particle::SMOOTHNESS)) {}
