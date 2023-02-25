#include "Matrix4.h"
#include <string>
#include <cmath>

#include "Quaternion.h"

const Matrix4 IDENTITY(),ZERO(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);

Matrix4::Matrix4(real values[]) {
    for (int i = 0; i < 16; i++) {data[i] = values[i];}
}

Matrix4::Matrix4() : Matrix4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1) {}

Matrix4::Matrix4(real e11,real e12,real e13,real e14,real e21,real e22,real e23,real e24,real e31,real e32,real e33,real e34,real e41,real e42,real e43,real e44) {
    data[0] = e11; data[1] = e12; data[2] = e13; data[3] = e14;
    data[4] = e21; data[5] = e22; data[6] = e23; data[7] = e24;
    data[8] = e31; data[9] = e32; data[10] = e33; data[11] = e34;
    data[12] = e41; data[13] = e42; data[14] = e43; data[15] = e44;
}


Vector4 Matrix4::getRow(int i) {
    return {data[4*i],data[4*i+1],data[4*i+2],data[4*i+3]};
}

Vector4 Matrix4::getColumn(int i) {
    return {data[i],data[i+4],data[i+8],data[i+12]};
}

Matrix4& Matrix4::multiply(Matrix4 mat) {
    Vector4 row;
    for (int r = 0; r < 4; r++) {
        row = getRow(r);
        for (int c = 0; c < 4; c++) {
            data[r*4+c] = row.dot(mat.getColumn(c));
        }
    }
    return *this;
}
Vector4 Matrix4::multiply(Vector4 vec) {
    return {vec.dot(getRow(0)),vec.dot(getRow(1)),vec.dot(getRow(2)),vec.dot(getRow(3))};
}

Vector4 Matrix4::multiply(Vector3 vec3, real w) {
    return multiply(Vector4(vec3.x,vec3.y,vec3.z,w));
}

Matrix4& Matrix4::translate(real x,real y,real z) {
    multiply(Matrix4(
            1,0,0,x,
            0,1,0,y,
            0,0,1,z,
            0,0,0,1
    ));
    return *this;
}

Matrix4& Matrix4::translate(Vector3 vec) {
    return translate(vec.x,vec.y,vec.z);
}

Matrix4 &Matrix4::rotate(Quaternion q) {
    return multiply({1 - (2*q.j*q.j + 2*q.k*q.k),   2*q.i*q.j - 2*q.k*q.r,        2*q.i*q.k + 2*q.j*q.r,      0,
                     2*q.i*q.j + 2*q.k*q.r,             1 - (2*q.i*q.i + 2*q.k*q.k),  2*q.j*q.k - 2*q.i*q.r,      0,
                     2*q.i*q.k - 2*q.j*q.r,             2*q.j*q.k + 2*q.i*q.r,        1 - (2*q.i*q.i + 2*q.j*q.j),0,
                     0,                                 0,                            0,                          1});
}

Matrix4& Matrix4::rotateX(real xrot) {
    return multiply(Matrix4(1,0,0,0,
                     0,cos(xrot),-sin(xrot),0,
                     0,sin(xrot),cos(xrot),0,
                     0,0,0,1));
}
Matrix4& Matrix4::rotateY(real yrot) {
    return multiply(Matrix4(cos(yrot),0,sin(yrot),0,
                      0,1,0,0,
                      -sin(yrot),0,cos(yrot),0,
                      0,0,0,1));
}
Matrix4& Matrix4::rotateZ(real zrot) {
    return multiply(Matrix4(cos(zrot),-sin(zrot),0,0,
                     sin(zrot),cos(zrot),0,0,
                     0,0,1,0,
                     0,0,0,1));

}
Matrix4& Matrix4::rotate(real yaw, real pitch, real roll) {
    return rotateY(yaw).rotateX(pitch).rotateZ(roll);
}

Matrix4& Matrix4::scale(real scaleX, real scaleY, real scaleZ) {
    return multiply(Matrix4(
            scaleX,0,0,0,
            0,scaleY,0,0,
            0,0,scaleZ,0,
            0,0,0,1
    ));
}
Matrix4& Matrix4::scale(real scalar) {return scale(scalar,scalar,scalar);}

GLfloat* Matrix4::toGLFloatArray() const {
    GLfloat* result = new GLfloat[16];
    for (int i = 0; i < 16; i++) {result[i] = (GLfloat) data[i];}
    return result;
}

Matrix4 Matrix4::viewMatrix(Vector3 viewPos, real viewYaw, real viewPitch, real viewRoll) {
    return Matrix4().rotateZ(-viewRoll).rotateX(-viewPitch).rotateY(-viewYaw).translate(-viewPos);
}

Matrix4 Matrix4::perspectiveProjectionMatrix(real fov, real nearClipping, real farClipping, real aspectRatio) {
    real S = 1 / (tan(fov / 2));
    return {
            S/aspectRatio, 0, 0, 0,
            0, S, 0, 0,
            0, 0, -farClipping / (farClipping - nearClipping),-(farClipping * nearClipping) / (farClipping - nearClipping),
            0, 0, -1, 0
    };
}

Matrix4 Matrix4::orthographicProjectionMatrix(real left, real right, real bottom, real top, real near, real far) {
    return Matrix4().scale(
            2/(right-left),2/(top-bottom),-2/(far-near)
    ).translate(
            -(left+right)/2,-(bottom+top)/2,(near+far)/2
    );
}
