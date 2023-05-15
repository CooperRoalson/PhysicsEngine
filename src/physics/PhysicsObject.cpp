#include "PhysicsObject.h"

#include <cmath>
#include "RigidBody.h"

const real PhysicsObject::DAMPING(0.9f);

bool hasFiniteMass();

PhysicsObject::PhysicsObject(Vector3 pos, Vector3 vel, real inverseMass, bool damping, Shape model) : position(pos), velocity(vel), inverseMass(inverseMass), model(model), damping(damping) {}

real PhysicsObject::getInverseMass() const {return inverseMass;}
bool PhysicsObject::hasFiniteMass() const {return inverseMass > 0.0f;}

Vector3 PhysicsObject::getPosition() const {return position;}
Vector3 PhysicsObject::getVelocity() const {return velocity;}

const Shape& PhysicsObject::getShape() const {return model;}

Matrix4 PhysicsObject::getShapeMatrix() const {
    return Matrix4().translate(position);
}

void PhysicsObject::update(real deltaTime) {
    if (!hasFiniteMass()) {return;}

    position += velocity*deltaTime;

    // a = F/m
    Vector3 acceleration = forceAccumulator * inverseMass;

    velocity += acceleration*deltaTime;

    if (damping) { velocity *= real_pow(DAMPING, deltaTime); }

    clearAccumulators();
}

void PhysicsObject::addForceAtPoint(Vector3 force, Vector3 pos) { if (hasFiniteMass()) {forceAccumulator += force;} }

void PhysicsObject::clearAccumulators() {
    forceAccumulator = Vector3();
}

void PhysicsObject::setVelocity(Vector3 vel) {
    velocity = vel;
}

void PhysicsObject::setPosition(Vector3 pos) {
    position = pos;
}

void PhysicsObject::addForce(Vector3 force) {
    addForceAtBodyPoint(force, Vector3());
}

void PhysicsObject::addForceAtBodyPoint(Vector3 force, Vector3 relPos) {
    addForceAtPoint(force, relPos + position);
}

Vector3 PhysicsObject::getPointInWorldSpace(Vector3 bodyPos) {
    return position + bodyPos;
}

Vector3 PhysicsObject::getPointInBodySpace(Vector3 worldPos) {
    return worldPos - position;
}

BoundingSphere PhysicsObject::getBoundingSphere() const {
    return BoundingSphere(Vector3(), 0);
}

const real Particle::RADIUS = 0.2;
const int Particle::SMOOTHNESS = 2;

Particle::Particle(Vector3 pos, Vector3 vel, real inverseMass, bool damping, VertexColor color) : PhysicsObject(pos,vel,inverseMass,damping,Shape::icosphere(Vector3(), Particle::RADIUS, color, Particle::SMOOTHNESS)) {}

std::ostream &operator<<(std::ostream &out, const PhysicsObject &obj) {
    if (auto p = dynamic_cast<const Particle*>(&obj)) {out << "Particle";}
    else if (auto rb = dynamic_cast<const RigidBody*>(&obj)) {out << "RigidBody(" << *rb->getModel() << ")";}
    else {out << "PhysicsObject";}

    out << "@" << obj.getPosition();
    return out;
}
