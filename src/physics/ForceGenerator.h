#ifndef PHYSICSENGINE_FORCEGENERATOR_H
#define PHYSICSENGINE_FORCEGENERATOR_H


#include "PhysicsObject.h"
#include "../core/precision.h"

/*
 * Generates translational forces for one or more PhysicsObjects
 */
class ForceGenerator {
public:
    virtual ~ForceGenerator();

    /*
     * Override this in implementations to calculate
     * force applied to the given PhysicsObject
     */
    virtual void updateForce(PhysicsObject* object, real deltaTime) = 0;
};

/*
 * A force generator to apply gravity to one or more PhysicsObjects
 */
class GravityForce : public ForceGenerator {
private:
    Vector3 gravity;

public:
    GravityForce(Vector3 gravity);

    void updateForce(PhysicsObject* object, real deltaTime) override;

};

/*
 * A force generator to apply a drag force to one or more PhysicsObjects
 */
class DragForce : public ForceGenerator {
private:
    /* The two drag coefficients: speed and speed-squared, respectively */
    real k1,k2;

public:
    DragForce(real k1, real k2);

    void updateForce(PhysicsObject* object, real deltaTime) override;

};

/*
 * A force generator to apply a spring force towards a target PhysicsObject
 */
class SpringForce : public ForceGenerator {
private:
    /* The other endpoint of the spring */
    std::function<Vector3(void)> anchorPositionGetter;

    /* The spring constant and rest length of the spring */
    real k, restLength;

    /* Whether the spring will exert pushing forces, or just pulls */
    bool shouldPush;

public:
    /* Creates a SpringForce towards a PhysicsObject */
    SpringForce(PhysicsObject* objectAnchor, real k, real restLength, bool shouldPush);
    /* Creates a SpringForce towards a fixed Vector3 */
    SpringForce(Vector3 staticAnchor, real k, real restLength, bool shouldPush);

    void updateForce(PhysicsObject* object, real deltaTime) override;

};

/*
 * A force generator to apply a buoyancy force for a plane
 * of liquid parallel to the XZ plane
 */
class BuoyancyForce : public ForceGenerator {
private:

    real maxDepth;
    real volume;
    real waterHeight;
    real liquidDensity;

public:

    void updateForce(PhysicsObject* object, real deltaTime) override;

};


#endif //PHYSICSENGINE_FORCEGENERATOR_H
