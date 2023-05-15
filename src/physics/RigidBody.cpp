#include "RigidBody.h"

const real RigidBody::ANGULAR_DAMPING(0.9f);

void RigidBody::calculateDerivedData() {
    orientation.normalize();

    // Calculate the transform matrix for the body
    transformMatrix = Matrix4().translate(position).rotate(orientation);
    inverseInertiaTensorWorld = Matrix4().rotate(orientation).multiply(model->getInverseInertiaTensor(inverseMass)).multiply(Matrix4().rotate(orientation).inverse());
}

RigidBody::RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, RigidBodyModel* model, Shape shape)
        : PhysicsObject(pos, vel, inverseMass, damping, shape), orientation(dir), angularVelocity(rot), model(model) {
    calculateDerivedData();
}

RigidBody::RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, RigidBodyModel* model, VertexColor color)
        : RigidBody(pos, vel, dir, rot, inverseMass, damping, model, model->getMatchingShape(color)) {}

Matrix4 RigidBody::getShapeMatrix() const {
    return transformMatrix;
}

void RigidBody::update(real deltaTime) {
    if (!hasFiniteMass()) {return;}

    // Update angular velocity/position
    Vector3 angularAcceleration = inverseInertiaTensorWorld.multiply(Vector4(torqueAccumulator,1 ));
    angularVelocity += angularAcceleration * deltaTime;
    orientation.addScaledVector(angularVelocity*deltaTime);

    if (damping) { angularVelocity *= real_pow(ANGULAR_DAMPING, deltaTime); }

    // Update linear velocity/position and clear accumulators
    PhysicsObject::update(deltaTime);

    calculateDerivedData();

}

void RigidBody::addForceAtPoint(Vector3 force, Vector3 pos) {
    PhysicsObject::addForceAtPoint(force, pos);
    pos -= position;
    if(pos.isZero()) {return;}
    torqueAccumulator += pos.cross(force);
}

void RigidBody::clearAccumulators() {
    PhysicsObject::clearAccumulators();
    torqueAccumulator = Vector3();
}

void RigidBody::addForceAtBodyPoint(Vector3 force, Vector3 relPos) {
    addForceAtPoint(force, getPointInWorldSpace(relPos));
}

Vector3 RigidBody::getPointInWorldSpace(Vector3 bodyPos) {
    return transformMatrix.multiply(Vector4(bodyPos, 1));
}

Vector3 RigidBody::getPointInBodySpace(Vector3 worldPos) {
    return transformMatrix.inverse().multiply(Vector4(worldPos, 1));
}

Quaternion RigidBody::getOrientation() const {
    return orientation;
}

void RigidBody::setPosition(Vector3 vel) {
    PhysicsObject::setPosition(vel);
    calculateDerivedData();
}

BoundingSphere RigidBody::getBoundingSphere() const {
    BoundingSphere sphere = model->getBoundingSphere();
    sphere.center += position;
    return sphere;
}

const RigidBodyModel* RigidBody::getModel() const {
    return model;
}

std::ostream &operator<<(std::ostream &out, const RigidBody &r) {
//    RigidBodyModel m = *r.getModel();
//    std::cout << &m;
    out << "RigidBody(" << *r.getModel() << ")@" << r.getPosition();
    return out;
}
