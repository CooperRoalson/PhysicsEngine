#ifndef PHYSICSENGINE_RIGIDBODYMODEL_H
#define PHYSICSENGINE_RIGIDBODYMODEL_H


#include "../core/Matrix4.h"
#include "../render/Shape.h"

/*
 * A class to store the physical shape of a RigidBody
 * for collision and inertia purposes. Not specific to a RigidBody
 */
class RigidBodyModel {
public:
    explicit RigidBodyModel();

    virtual Shape getMatchingShape(VertexColor color);

    /*
     * Calculate an inertia tensor based on the
     * model's geometry and a mass. Should be overriden in
     * subclasses.
     */
    virtual Matrix4 getInverseInertiaTensor(real inverseMass);

};


class RectangularPrismModel : public RigidBodyModel {

private:
    real xLen, yLen, zLen;

public:
    RectangularPrismModel(real xLen, real yLen, real zLen);

    Matrix4 getInverseInertiaTensor(real inverseMass) override;

    Shape getMatchingShape(VertexColor color) override;
};




#endif //PHYSICSENGINE_RIGIDBODYMODEL_H
