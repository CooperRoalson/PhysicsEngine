#ifndef PHYSICSENGINE_VECTOR3_H
#define PHYSICSENGINE_VECTOR3_H

#include <cmath>
#include <iostream>

#include "precision.h"

struct Vector3 {
    real x,y,z;

    static const Vector3 ZERO, RIGHT, LEFT, UP, DOWN, FORWARD, BACKWARD;

    Vector3();
    Vector3(real x, real y, real z);

    static Vector3 fromAngles(real azimuth, real elevation, real magnitude);

    Vector3 operator-() const;

    Vector3 operator+(Vector3 vec) const;
    void operator+=(Vector3 vec);

    Vector3 operator-(Vector3 vec) const;
    void operator-=(Vector3 vec);

    Vector3 operator*(real scalar) const;
    void operator*=(real scalar);

    Vector3 operator/(real scalar) const;
    void operator/=(real scalar);

    real magnitudeSquared() const;
    real magnitude() const;

    Vector3 normalized() const;
    void normalize();

    real dot(Vector3 vec) const;
    Vector3 cross(Vector3 vec) const;
    static real dot(Vector3 vec1, Vector3 vec2);
    static Vector3 cross(Vector3 vec1, Vector3 vec2);

};

std::ostream& operator<<(std::ostream &out, const Vector3 &v);


#endif //PHYSICSENGINE_VECTOR3_H
