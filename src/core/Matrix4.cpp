#include "Matrix4.h"
#include <string>

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

void Matrix4::multiply(Matrix4 mat) {
    Vector4 row;
    for (int r = 0; r < 4; r++) {
        row = getRow(r);
        for (int c = 0; c < 4; c++) {
            data[r*4+c] = row.dot(mat.getColumn(c));
        }
    }
}
Vector4 Matrix4::multiply(Vector4 vec) {
    return {vec.dot(getRow(0)),vec.dot(getRow(1)),vec.dot(getRow(2)),vec.dot(getRow(3))};
}

void Matrix4::translate(Vector3 vec) {
    data[3] += vec.x;
    data[7] += vec.y;
    data[11] += vec.z;
}

void Matrix4::rotateX(real xrot) {
    multiply(Matrix4(1,0,0,0,
                     0,cos(xrot),-sin(xrot),0,
                     0,sin(xrot),cos(xrot),0,
                     0,0,0,1));
}
void Matrix4::rotateY(real yrot) {
    multiply(Matrix4(cos(yrot),0,sin(yrot),0,
                      0,1,0,0,
                      -sin(yrot),0,cos(yrot),0,
                      0,0,0,1));
}
void Matrix4::rotateZ(real zrot) {
    multiply(Matrix4(cos(zrot),-sin(zrot),0,0,
                     sin(zrot),cos(zrot),0,0,
                     0,0,1,0,
                     0,0,0,1));

}
void Matrix4::rotateXYZ(real yaw, real pitch, real roll) {
    rotateZ(yaw);
    rotateY(pitch);
    rotateX(roll);
}
void Matrix4::rotateZYX(real yaw, real pitch, real roll) {
    rotateX(roll);
    rotateY(pitch);
    rotateZ(yaw);
}

void Matrix4::scale(real scaleX, real scaleY, real scaleZ) {
    data[0] *= scaleX;
    data[5] *= scaleY;
    data[10] *= scaleZ;
}
void Matrix4::scale(real scalar) {scale(scalar,scalar,scalar);}

GLfloat* Matrix4::toGLFloatArray() const {
    GLfloat* result = new GLfloat[16];
    for (int i = 0; i < 16; i++) {result[i] = (GLfloat) data[i];}
    return result;
}

Matrix4 Matrix4::perspectiveProjectionMatrix(real fov, real nearClipping, real farClipping) {
    real S = 1 / (tan(fov / 2));
    return {
            S, 0, 0, 0,
            0, S, 0, 0,
            0, 0, -farClipping / (farClipping - nearClipping),-(farClipping * nearClipping) / (farClipping - nearClipping),
            0, 0, -1, 0
    };
}
