#ifndef PHYSICSENGINE_RIGIDBODY_H
#define PHYSICSENGINE_RIGIDBODY_H

#include "PhysicsObject.h"
#include "../core/Quaternion.h"

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

    /*
     * Calculates internal data from state data. Should be called
     * after the body's state is directly altered (automatically
     * happens during updates).
     */
    void calculateDerivedData();

    Matrix4 getModelMatrix() const override;

public:
    RigidBody(Vector3 pos, Vector3 vel, Quaternion dir, Vector3 rot, real inverseMass, bool damping, Shape model);

    Quaternion getOrientation() const;

    void addForce(Vector3 force) override;
    void update(real deltaTime) override;

};


#endif //PHYSICSENGINE_RIGIDBODY_H
