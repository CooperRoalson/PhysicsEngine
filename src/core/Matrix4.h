#ifndef PHYSICSENGINE_MATRIX4_H
#define PHYSICSENGINE_MATRIX4_H

#include <SDL_opengl.h>
#include "precision.h"
#include "Vector3.h"
#include "Vector4.h"

class Matrix4 {

private:
    real data[16];
public:
    const static Matrix4 ZERO, IDENTITY;

    Matrix4();
    Matrix4(real values[]);
    Matrix4(real e11,real e12,real e13,real e14,real e21,real e22,real e23,real e24,real e31,real e32,real e33,real e34,real e41,real e42,real e43,real e44);

    Vector4 getRow(int i);
    Vector4 getColumn(int i);

    Matrix4& multiply(Matrix4 mat);
    Vector4 multiply(Vector4 vec);
    Vector4 multiply(Vector3 vec3, real w);

    Matrix4& translate(real x,real y,real z);
    Matrix4& translate(Vector3 vec);
    Matrix4& rotateX(real xrot);
    Matrix4& rotateY(real yrot);
    Matrix4& rotateZ(real zrot);
    Matrix4& rotate(real yaw, real pitch, real roll);
    Matrix4& scale(real scaleX, real scaleY, real scaleZ);
    Matrix4& scale(real scalar);

    GLfloat* toGLFloatArray() const;

    static Matrix4 viewMatrix(Vector3 viewPos, real viewYaw, real viewPitch, real viewRoll);
    static Matrix4 perspectiveProjectionMatrix(real fov, real nearClipping, real farClipping, real aspectRatio);
    static Matrix4 orthographicProjectionMatrix(real left, real right, real bottom, real top, real near, real far);
};


#endif //PHYSICSENGINE_MATRIX4_H
