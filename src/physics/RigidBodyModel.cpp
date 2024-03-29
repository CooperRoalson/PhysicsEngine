
#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel() {}

Matrix4 RigidBodyModel::getInverseInertiaTensor(real inverseMass) {
    return Matrix4();
}

Shape RigidBodyModel::getMatchingShape(VertexColor color) {
    return Shape(0, nullptr, nullptr, 0, nullptr, false);
}

BoundingSphere RigidBodyModel::getBoundingSphere() const { return boundingSphere; }

RectangularPrismModel::RectangularPrismModel(real xLen, real yLen, real zLen) : xLen(xLen), yLen(yLen), zLen(zLen) {
    boundingSphere = BoundingSphere(Vector3(), (real)0.5 * sqrt(xLen*xLen + yLen*yLen + zLen*zLen));
}

Matrix4 RectangularPrismModel::getInverseInertiaTensor(real inverseMass) {
    return Matrix4(12*inverseMass/(yLen*yLen + zLen*zLen), 0, 0, 0,
                   0, 12*inverseMass/(xLen*xLen + zLen*zLen), 0, 0,
                   0, 0, 12*inverseMass/(xLen*xLen + yLen*yLen), 0,
                   0, 0, 0, 1);
}

Shape RectangularPrismModel::getMatchingShape(VertexColor color) {
    return Shape::rectangularPrism(Vector3(0,0,0), xLen, yLen, zLen, color, true);
}

std::ostream &operator<<(std::ostream &out, const RigidBodyModel &rm) {
    if (auto rp = dynamic_cast<const RectangularPrismModel*>(&rm)) {out << "RectangularPrismModel";}
    else {out << "RigidBodyModel";}
    return out;
}

