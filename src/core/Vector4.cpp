#include "Vector4.h"

Vector4::Vector4() : Vector4(0,0,0,0) {}
Vector4::Vector4(real x, real y, real z, real w) : x(x), y(y), z(z), w(w) {}

Vector4 Vector4::operator-() const {return {-x,-y,-z,-w};}

Vector4 Vector4::operator+(Vector4 vec) const {return {x+vec.x,y+vec.y,z+vec.z,w+vec.w};}
void Vector4::operator+=(Vector4 vec) {x += vec.x; y += vec.y; z += vec.z; w += vec.w;}

Vector4 Vector4::operator-(Vector4 vec) const {return operator+(-vec);}
void Vector4::operator-=(Vector4 vec) {operator+=(-vec);}

Vector4 Vector4::operator*(real scalar) const {return {x*scalar,y*scalar,z*scalar,w*scalar};}
void Vector4::operator*=(real scalar) {x *= scalar; y *= scalar; z *= scalar; w *= scalar;}

Vector4 Vector4::operator/(real scalar) const {return operator*(1/scalar);}
void Vector4::operator/=(real scalar) {operator*=(1/scalar);}


real Vector4::magnitudeSquared() const {return x*x+y*y+z*z+w*w;}
real Vector4::magnitude() const {return sqrt(magnitudeSquared());}

Vector4 Vector4::normalized() const {return operator/(magnitude());}
void Vector4::normalize() {operator/=(magnitude());}

real Vector4::dot(Vector4 vec) const {return x*vec.x + y*vec.y + z*vec.z + w*vec.w;}
real Vector4::dot(Vector4 vec1, Vector4 vec2) {return vec1.dot(vec2);}

std::ostream& operator<<(std::ostream &out, const Vector4 &v) {
    out << "{" << v.x << "," << v.y << "," << v.z << "," << v.w << "}";
    return out;
}
