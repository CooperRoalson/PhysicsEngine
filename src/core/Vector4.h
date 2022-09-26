#ifndef PHYSICSENGINE_VECTOR4_H
#define PHYSICSENGINE_VECTOR4_H

#include <iostream>

#include "precision.h"

struct Vector4 {
    real x,y,z,w;

    Vector4();
    Vector4(real x, real y, real z, real w);

    Vector4 operator-() const;

    Vector4 operator+(const Vector4& vec) const;
    Vector4& operator+=(const Vector4& vec);

    Vector4 operator-(const Vector4& vec) const;
    Vector4& operator-=(const Vector4& vec);

    Vector4 operator*(const real& scalar) const;
    Vector4& operator*=(const real& scalar);

    Vector4 operator/(const real& scalar) const;
    Vector4& operator/=(const real& scalar);

    real magnitudeSquared() const;
    real magnitude() const;

    Vector4 normalized() const;
    void normalize();

    real dot(Vector4 vec) const;
    Vector4 cross(Vector4 vec) const;
    static real dot(Vector4 vec1, Vector4 vec2);
    static Vector4 cross(Vector4 vec1, Vector4 vec2);

    bool isZero() const;
};

std::ostream& operator<<(std::ostream &out, const Vector4 &v);

#endif //PHYSICSENGINE_VECTOR4_H
