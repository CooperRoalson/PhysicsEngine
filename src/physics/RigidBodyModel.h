#ifndef PHYSICSENGINE_RIGIDBODYMODEL_H
#define PHYSICSENGINE_RIGIDBODYMODEL_H


#include "../core/Matrix4.h"
#include "../render/Shape.h"

/*
 * A class to store the physical shape of a RigidBody
 * for collisiona and inertia purposes
 */
class RigidBodyModel {

private:
    /*
     * The inertia tensor for the object, stored as a 3x3
     * subsection of a Matrix4
     */
    Matrix4 inverseInertiaTensor;

public:
    explicit RigidBodyModel();

    Vector3 torqueToAngularAcc(Vector3 torque);

    virtual Shape getMatchingShape(VertexColor color);

    /*
     * Create the inverseInertiaTensor based on the
     * model's geometry and mass. Should be overriden in
     * subclasses, and should be called on creation.
     */
    virtual void calculateInertiaTensor(real inverseMass);

};




#endif //PHYSICSENGINE_RIGIDBODYMODEL_H
