#include "RigidBody.h"

void RigidBody::calculateDerivedData() {
    orientation.normalize();

    model.calculateInertiaTensor(inverseMass);

    // Calculate the transform matrix for the body
    transformMatrix = Matrix4().translate(position).rotate(orientation);
}

RigidBody::RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, RigidBodyModel model, Shape shape)
        : PhysicsObject(pos, vel, inverseMass, damping, shape), orientation(dir), angularVelocity(rot), model(model) {
    calculateDerivedData();
}

RigidBody::RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, RigidBodyModel model, VertexColor color)
        : RigidBody(pos, vel, dir, rot, inverseMass, damping, model, model.getMatchingShape(color)) {}

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

