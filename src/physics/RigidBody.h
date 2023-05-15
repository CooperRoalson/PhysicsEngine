#ifndef PHYSICSENGINE_RIGIDBODY_H
#define PHYSICSENGINE_RIGIDBODY_H

#include "PhysicsObject.h"
#include "../math/Quaternion.h"
#include "RigidBodyModel.h"

/*
 * A PhysicsObject that also stores an orientation
 * and an angular momentum, as well as experiencing
 * collisions with other RigidBodies.
 */
class RigidBody : public PhysicsObject {
protected:
    /*
     * Holds the RigidBody's 3D orientation
     */
    Quaternion orientation;

    /*
     * Holds the RigidBody's angular velocity
     */
    Vector3 angularVelocity;

    /*
     * Holds the matrix for converting between body space
     * and world space. Used via the getPointIn__Space functions.
     */
    Matrix4 transformMatrix;

    Matrix4 inverseInertiaTensorWorld;

    /*
     * Stores the physical geometry of the RigidBody
     */
    RigidBodyModel* model;

    /*
     * Sums up the torques each frame
     */
    Vector3 torqueAccumulator;

    /*
     * Calculates internal data from state data. Should be called
     * after the body's state is directly altered (automatically
     * happens during updates).
     */
    void calculateDerivedData();

    void clearAccumulators() override;

    Matrix4 getShapeMatrix() const override;

public:
    static const real ANGULAR_DAMPING;

    RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, RigidBodyModel* model, Shape shape);
    RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, RigidBodyModel* model, VertexColor color);

    Quaternion getOrientation() const;

    void setPosition(Vector3 vel) override;

    void addForceAtPoint(Vector3 force, Vector3 pos) override;
    void addForceAtBodyPoint(Vector3 force, Vector3 relPos) override;
    void update(real deltaTime) override;

    Vector3 getPointInWorldSpace(Vector3 bodyPos) override;
    Vector3 getPointInBodySpace(Vector3 worldPos) override;

    BoundingSphere getBoundingSphere() const override;
    const RigidBodyModel* getModel() const;

};

#endif //PHYSICSENGINE_RIGIDBODY_H
