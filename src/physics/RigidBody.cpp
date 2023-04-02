#include "RigidBody.h"

void RigidBody::calculateDerivedData() {
    orientation.normalize();

    // Calculate the transform matrix for the body
    transformMatrix = Matrix4().translate(position).rotate(orientation);
}

RigidBody::RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, Shape model)
        : PhysicsObject(pos, vel, inverseMass, damping, model), orientation(dir), angularVelocity(rot) {
    calculateDerivedData();
}

Matrix4 RigidBody::getModelMatrix() const {
    return transformMatrix;
}

void RigidBody::update(real deltaTime) {
    PhysicsObject::update(deltaTime);

    if (!hasFiniteMass()) {return;}

    orientation.addScaledVector(angularVelocity*deltaTime);
    if (damping) { angularVelocity *= real_pow(DAMPING, deltaTime); }

    calculateDerivedData();

}

void RigidBody::addForce(Vector3 force) {
    PhysicsObject::addForce(force);
}

