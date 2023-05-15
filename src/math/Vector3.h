#ifndef PHYSICSENGINE_VECTOR3_H
#define PHYSICSENGINE_VECTOR3_H

#include <iostream>

#include "precision.h"
#include "Vector4.h"

struct Vector3 {
    real x,y,z;

    static const Vector3 ZERO, RIGHT, LEFT, UP, DOWN, FORWARD, BACKWARD;

    Vector3();
    Vector3(real x, real y, real z);
    Vector3(Vector4 vec4);

    /*
     * Elevation ranges from -PI/2 (facing -Y) to PI/2 (facing +Y)
     * Azimuth ranges from 0 to 2*PI (both facing -Z) where PI/2 faces -X, PI faces +Z and 3/2*PI faces +X
     */
    static Vector3 fromAngles(real azimuth, real elevation, real magnitude);

    Vector3 operator-() const;

    Vector3 operator+(const Vector3& vec) const;
    Vector3& operator+=(const Vector3& vec);

    Vector3 operator-(const Vector3& vec) const;
    Vector3& operator-=(const Vector3& vec);

    Vector3 operator*(const real& scalar) const;
    Vector3& operator*=(const real& scalar);

    Vector3 operator/(const real& scalar) const;
    Vector3& operator/=(const real& scalar);

    real magnitudeSquared() const;
    real magnitude() const;

    real azimuth() const;
    real elevation() const;

    Vector3 normalized() const;
    void normalize();

    real dot(Vector3 vec) const;
    Vector3 cross(Vector3 vec) const;
    static real dot(Vector3 vec1, Vector3 vec2);
    static Vector3 cross(Vector3 vec1, Vector3 vec2);

    bool isZero() const;
};

std::ostream& operator<<(std::ostream &out, const Vector3 &v);


#endif //PHYSICSENGINE_VECTOR3_H
