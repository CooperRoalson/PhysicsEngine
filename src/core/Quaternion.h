#ifndef PHYSICSENGINE_QUATERNION_H
#define PHYSICSENGINE_QUATERNION_H

#include "precision.h"
#include "Vector3.h"

/*
 * Stores a 3D orientation with 3 degrees of freedom
 */
struct Quaternion {
    real r,i,j,k;

    Quaternion();
    Quaternion(real r, real i, real j, real k);


    Quaternion operator-() const;

    Quaternion operator+(Quaternion& other) const;
    void operator+=(Quaternion& other);

    Quaternion operator*(real other) const;
    void operator*=(real other);

    Quaternion operator*(Quaternion& other) const;
    Quaternion& operator*=(Quaternion& other);

    /* Rotate by a vector that represents a rotation around itself as an axis
     * of an amount given by its magnitude */
    void rotateByVector(Vector3& vector) const;

    Quaternion addVector(Vector3& vector) const;

    real magnitudeSquared() const;
    real magnitude() const;

    /* Normalize the quaternion, making it a valid orientation */
    void normalize();

    bool isZero() const;
};

std::ostream& operator<<(std::ostream &out, const Vector4 &v);


#endif //PHYSICSENGINE_QUATERNION_H
