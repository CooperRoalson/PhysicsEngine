#include "ForceGenerator.h"

ForceGenerator::~ForceGenerator() {}

GravityForce::GravityForce(Vector3 gravity) : gravity(gravity) {}

void GravityForce::updateForce(PhysicsObject* object, real deltaTime) {
    object->addForce(gravity);
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

SpringForce::SpringForce(PhysicsObject* objectAnchor, real k, real restLength, bool shouldPush) : anchorPositionGetter([objectAnchor](){return objectAnchor->getPosition();}), k(k), restLength(restLength), shouldPush(shouldPush) {}
SpringForce::SpringForce(Vector3 staticAnchor, real k, real restLength, bool shouldPush) : anchorPositionGetter([staticAnchor](){return staticAnchor;}), k(k), restLength(restLength), shouldPush(shouldPush) {}

real SpringForce::SPRING_DAMPING = 0.75f;

void SpringForce::updateForce(PhysicsObject *object, real deltaTime) {

    // Get displacement
    Vector3 force = object->getPosition() - anchorPositionGetter();

    // Get force magnitude
    real magnitude = (force.magnitude() - restLength) * k;
    if (magnitude < 0 && !shouldPush) {return;} // Don't exert pushes if shouldPush is false

    magnitude *= real_pow(SPRING_DAMPING, deltaTime); // Dampen the spring so it doesn't build up momentum

    force = force.normalized() * -magnitude;

    object->addForce(force);

}
