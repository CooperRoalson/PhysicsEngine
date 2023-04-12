#ifndef PHYSICSENGINE_FORCEGENERATOR_H
#define PHYSICSENGINE_FORCEGENERATOR_H


#include "PhysicsObject.h"
#include "../core/precision.h"
#include "../render/Renderable.h"

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
 * A force generator to apply standard, uniform gravity to one or more PhysicsObjects
 */
class UniformGravityForce : public ForceGenerator {
private:
    Vector3 gravity;

public:
    UniformGravityForce(Vector3 gravity);

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
 * A force generator to apply a spring force between two PhysicsObjects
 */
class SpringForce : public ForceGenerator, public Renderable {
private:
    /* The point of these objects that the spring is attached
     * to, in object coordinates */
    Vector3 connectionPoints[2];

    /* The anchors of the spring */
    PhysicsObject* objects[2];

    /* The spring constant and rest length of the spring */
    real k, restLength;

    /* Whether the spring will exert pushing forces, or just pulls */
    bool shouldPush;

public:
    static float SPRING_DAMPING;

    /* Creates a SpringForce towards a PhysicsObject */
    SpringForce(PhysicsObject* objectAnchor1, Vector3 connectionPoint1, PhysicsObject* objectAnchor2, Vector3 connectionPoint2, real k, real restLength, bool shouldPush);

    void updateForce(PhysicsObject* object, real deltaTime) override;

    Shape getShape() const override;

};

/*
 * A force generator to pull one or more PhysicsObjects towards a source object through gravity
 */
class GravitationalAttractionForce : public ForceGenerator {
private:
    /* The object creating the gravitational field */
    PhysicsObject* srcObject;

    /* The gravitational constant for this force */
    real g;

public:
    /* Creates a GravitationalAttractionForce towards a PhysicsObject */
    GravitationalAttractionForce(PhysicsObject* srcObject, real gravitationalConstant);

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
