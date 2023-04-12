#include "ForceGenerator.h"

ForceGenerator::~ForceGenerator() {}

UniformGravityForce::UniformGravityForce(Vector3 gravity) : gravity(gravity) {}

void UniformGravityForce::updateForce(PhysicsObject* object, real deltaTime) {
    if (object->hasFiniteMass()) {object->addForce(gravity/object->getInverseMass());}
}


DragForce::DragForce(real k1, real k2) : k1(k1), k2(k2)  {}

void DragForce::updateForce(PhysicsObject* object, real deltaTime) {
    // Get object's speed
    Vector3 force = object->getVelocity();
    real speed = force.magnitude();

    // Calculate and apply drag coefficient from speed
    real dragCoeff = k1 * speed + k2 * speed*speed;
    force = force.normalized()*-dragCoeff;

    object->addForce(force);
}

real SpringForce::SPRING_DAMPING = 0.75f;

SpringForce::SpringForce(PhysicsObject *objectAnchor1, Vector3 connectionPoint1, PhysicsObject *objectAnchor2, Vector3 connectionPoint2,real k, real restLength, bool shouldPush)
        : objects{objectAnchor1, objectAnchor2}, connectionPoints{connectionPoint1, connectionPoint2}, k(k), restLength(restLength), shouldPush(shouldPush) {}

void SpringForce::updateForce(PhysicsObject *object, real deltaTime) {
    Vector3 connectionPos;
    Vector3 anchorPos;
    if (object == objects[0]) {
        connectionPos = objects[0]->getPointInWorldSpace(connectionPoints[0]);
        anchorPos = objects[1]->getPointInWorldSpace(connectionPoints[1]);
    } else if (object == objects[1]) {
        connectionPos = objects[1]->getPointInWorldSpace(connectionPoints[1]);
        anchorPos = objects[0]->getPointInWorldSpace(connectionPoints[0]);
    } else {
        return;
    }

    // Get displacement
    Vector3 force = connectionPos - anchorPos;

    // Get force magnitude
    real magnitude = (force.magnitude() - restLength) * k;
    if (magnitude < 0 && !shouldPush) {return;} // Don't exert pushes if shouldPush is false

    magnitude *= real_pow(SPRING_DAMPING, deltaTime); // Dampen the spring so it doesn't build up momentum

    force = force.normalized() * -magnitude;

    object->addForceAtPoint(force, connectionPos);

}

Shape SpringForce::getShape() const {
    return Shape::cylinder(objects[0]->getPointInWorldSpace(connectionPoints[0]), objects[1]->getPointInWorldSpace(connectionPoints[1]), 0.1, C_BLACK, 6, false);
}

GravitationalAttractionForce::GravitationalAttractionForce(PhysicsObject *srcObject, real gravitationalConstant) : srcObject(srcObject), g(gravitationalConstant) {}

void GravitationalAttractionForce::updateForce(PhysicsObject *object, real deltaTime) {
    // Don't exert force if either mass is infinite
    if (object->getInverseMass() == 0 || srcObject->getInverseMass() == 0) {return;}

    // Get displacement
    Vector3 displacement = srcObject->getPosition() - object->getPosition();
    real dist = displacement.magnitude();

    // Get force magnitude
    real magnitude = g / (object->getInverseMass() * srcObject->getInverseMass() * (dist * dist * dist));

    object->addForce(displacement * magnitude);
}


