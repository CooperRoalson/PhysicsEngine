
#include "RigidBodyModel.h"

RigidBodyModel::RigidBodyModel() : inverseInertiaTensor() {}

void RigidBodyModel::calculateInertiaTensor(real inverseMass) {}

Vector3 RigidBodyModel::torqueToAngularAcc(Vector3 torque) {
    return Vector3(inverseInertiaTensor.multiply(Vector4(torque, 0)));
}

Shape RigidBodyModel::getMatchingShape(VertexColor color) {
    return Shape(0, nullptr, nullptr, 0, nullptr, false);
}
