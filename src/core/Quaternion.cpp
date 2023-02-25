#include "Quaternion.h"

Quaternion::Quaternion() : Quaternion(1,0,0,0){}

Quaternion::Quaternion(real r, real i, real j, real k) : r(r), i(i), j(j), k(k) {}

Quaternion Quaternion::operator-() const {
    return Quaternion();
}

Quaternion Quaternion::operator+(Quaternion &other) const { return {r+other.r,i+other.i,j+other.j,k+other.k}; }
Quaternion& Quaternion::operator+=(Quaternion &other) { r += other.r; i += other.i; j += other.j; k += other.k; return *this; }

Quaternion Quaternion::operator*(real other) const { return {r*other, i*other, j*other, k*other}; }

Quaternion &Quaternion::operator*=(real other) { r *= other; i *= other; j *= other; k *= other; return *this; }

Quaternion Quaternion::operator*(Quaternion &other) const {
    return {
        r * other.r - i * other.i - j * other.j - k * other.k,
        r * other.i + i * other.r + j * other.k - k * other.j,
        r * other.j + j * other.r + k * other.i - i * other.k,
        r * other.k + k * other.r + i * other.j - j * other.i
    };
}

Quaternion& Quaternion::operator*=(Quaternion &other) {
    r = r * other.r - i * other.i - j * other.j - k * other.k;
    i = r * other.i + i * other.r + j * other.k - k * other.j;
    j = r * other.j + j * other.r + k * other.i - i * other.k;
    k = r * other.k + k * other.r + i * other.j - j * other.i;
    return *this;
}

void Quaternion::rotateByVector(Vector3 vector) {
    Quaternion other {0,vector.x,vector.y,vector.z};
    operator*=(other);
}

Quaternion Quaternion::addScaledVector(Vector3 vector) {
    Quaternion q {0,vector.x,vector.y,vector.z};
    q = (q * (*this)) * 0.5;
    operator+=(q);
}

real Quaternion::magnitudeSquared() const { return r*r + i*i + j*j + k*k; }
real Quaternion::magnitude() const { return sqrt(magnitudeSquared()); }

void Quaternion::normalize() {if (!isZero()) operator*=(1/magnitude());}

bool Quaternion::isZero() const { return !(r || i || j || k); }

Quaternion Quaternion::fromAxisAngle(Vector3 axis, real angle) {
    axis = axis.normalized() * cos(angle/2);
    return {sin(angle/2), axis.x, axis.y, axis.z};
}

Quaternion Quaternion::fromEulerAngles(real yaw, real pitch, real roll) {
    Quaternion q1 = fromAxisAngle({0,0,1},roll);
    Quaternion q2 = fromAxisAngle({1,0,0},pitch);
    Quaternion q3 = fromAxisAngle({0,1,0},yaw);
    return q1 * q2 * q3;
}

std::ostream& operator<<(std::ostream &out, const Quaternion &q) {
    out << "{" << q.r << "," << q.i << "," << q.j << "," << q.k << "}";
    return out;
}
