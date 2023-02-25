#include "Vector4.h"
#include "Quaternion.h"


#include <cmath>

Vector4::Vector4() : Vector4(0,0,0,0) {}
Vector4::Vector4(real x, real y, real z, real w) : x(x), y(y), z(z), w(w) {}

Vector4 Vector4::operator-() const {return {-x,-y,-z,-w};}

Vector4 Vector4::operator+(const Vector4& vec) const {return {x+vec.x,y+vec.y,z+vec.z,w+vec.w};}
Vector4& Vector4::operator+=(const Vector4& vec) {x += vec.x; y += vec.y; z += vec.z; w += vec.w; return *this;}

Vector4 Vector4::operator-(const Vector4& vec) const {return operator+(-vec);}
Vector4& Vector4::operator-=(const Vector4& vec) {return operator+=(-vec);}

Vector4 Vector4::operator*(const real& scalar) const {return {x*scalar,y*scalar,z*scalar,w*scalar};}
Vector4& Vector4::operator*=(const real& scalar) {x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this;}

Vector4 Vector4::operator/(const real& scalar) const {return operator*(1/scalar);}
Vector4& Vector4::operator/=(const real& scalar) {return operator*=(1/scalar);}


real Vector4::magnitudeSquared() const {return x*x+y*y+z*z+w*w;}
real Vector4::magnitude() const {return sqrt(magnitudeSquared());}

Vector4 Vector4::normalized() const {return (isZero()) ? Vector4() : operator/(magnitude());}
void Vector4::normalize() {if (!isZero()) operator/=(magnitude());}

real Vector4::dot(Vector4 vec) const {return x*vec.x + y*vec.y + z*vec.z + w*vec.w;}
real Vector4::dot(Vector4 vec1, Vector4 vec2) {return vec1.dot(vec2);}

bool Vector4::isZero() const {return !(x || y || z || w);}

std::ostream& operator<<(std::ostream &out, const Vector4 &v) {
    out << "{" << v.x << "," << v.y << "," << v.z << "," << v.w << "}";
    return out;
}
