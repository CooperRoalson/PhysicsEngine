#include "Vector3.h"

const Vector3 Vector3::ZERO(0,0,0), Vector3::RIGHT(1,0,0), Vector3::LEFT(-1,0,0), Vector3::UP(0,1,0), Vector3::DOWN(0,-1,0), Vector3::FORWARD(0,0,1), Vector3::BACKWARD(0,0,-1);

Vector3::Vector3() : Vector3(0,0,0) {}
Vector3::Vector3(real x, real y, real z) : x(x), y(y), z(z) {}

Vector3 Vector3::fromAngles(real azimuth, real elevation, real magnitude) {
    return Vector3(cos(azimuth)*cos(elevation),sin(azimuth)*cos(elevation),sin(elevation)) * magnitude;
}

Vector3 Vector3::operator-() const {return {-x,-y,-z};}

Vector3 Vector3::operator+(Vector3 vec) const {return {x+vec.x,y+vec.y,z+vec.z};}
void Vector3::operator+=(Vector3 vec) {x += vec.x; y += vec.y; z += vec.z;}

Vector3 Vector3::operator-(Vector3 vec) const {return operator+(-vec);}
void Vector3::operator-=(Vector3 vec) {operator+=(-vec);}

Vector3 Vector3::operator*(real scalar) const {return {x*scalar,y*scalar,z*scalar};}
void Vector3::operator*=(real scalar) {x *= scalar; y *= scalar; z *= scalar;}

Vector3 Vector3::operator/(real scalar) const {return operator*(1/scalar);}
void Vector3::operator/=(real scalar) {operator*=(1/scalar);}

real Vector3::magnitudeSquared() const {return x*x+y*y+z*z;}
real Vector3::magnitude() const {return sqrt(magnitudeSquared());}

Vector3 Vector3::normalized() const {return operator/(magnitude());}
void Vector3::normalize() {operator/=(magnitude());}

real Vector3::dot(Vector3 vec) const {return x*vec.x + y*vec.y + z*vec.z;}
Vector3 Vector3::cross(Vector3 vec) const {return {y*vec.z-z*vec.y, z*vec.x-x*vec.z, x*vec.y-y*vec.x};}
real Vector3::dot(Vector3 vec1, Vector3 vec2) {return vec1.dot(vec2);}
Vector3 Vector3::cross(Vector3 vec1, Vector3 vec2) {return vec1.cross(vec2);}

std::ostream& operator<<(std::ostream &out, const Vector3 &v) {
    out << "{" << v.x << "," << v.y << "," << v.z << "}";
    return out;
}
