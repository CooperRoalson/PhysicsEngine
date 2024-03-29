#ifndef PHYSICSENGINE_RIGIDBODYMODEL_H
#define PHYSICSENGINE_RIGIDBODYMODEL_H


#include "../math/Matrix4.h"
#include "../render/Shape.h"
#include "BVHTree.h"

/*
 * A class to store the physical shape of a RigidBody
 * for collision and inertia purposes. Not specific to a RigidBody
 */
class RigidBodyModel {
protected:
    BoundingSphere boundingSphere;

public:
    explicit RigidBodyModel();

    virtual Shape getMatchingShape(VertexColor color);

    /*
     * Calculate an inertia tensor based on the
     * model's geometry and a mass. Should be overriden in
     * subclasses.
     */
    virtual Matrix4 getInverseInertiaTensor(real inverseMass);

    BoundingSphere getBoundingSphere() const;

};


class RectangularPrismModel : public RigidBodyModel {

private:
    real xLen, yLen, zLen;

public:
    RectangularPrismModel(real xLen, real yLen, real zLen);

    Matrix4 getInverseInertiaTensor(real inverseMass) override;

    Shape getMatchingShape(VertexColor color) override;
};

std::ostream& operator<<(std::ostream &out, const RigidBodyModel &rm);

#endif //PHYSICSENGINE_RIGIDBODYMODEL_H
