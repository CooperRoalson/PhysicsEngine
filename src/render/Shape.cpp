#include "Shape.h"

#include <string>

Shape::Shape(unsigned int numVertices, const Vector3* vertexPositions, const VertexColor* vertexColors, unsigned int numIndices, const GLuint* indices) {
    vertexCount = numVertices;
    indexCount = numIndices;

    vertexPositionArr = new Vector3[numVertices];
    std::memcpy(vertexPositionArr,vertexPositions,sizeof(Vector3)*numVertices);

    vertexColorArr = new VertexColor[numVertices];
    std::memcpy(vertexColorArr,vertexColors,sizeof(VertexColor)*numVertices);

    indexArr = new GLuint[numIndices];
    std::memcpy(indexArr,indices,sizeof(GLuint)*numIndices);

    vertexNormalArr = new Vector3[numVertices];
    generateVertexNormals();
}

Shape::Shape(const Shape& oldShape) : Shape::Shape(oldShape.numVertices(), oldShape.getVertexPositions(), oldShape.getVertexColors(), oldShape.numIndices(), oldShape.getIndices()) {}

Shape::~Shape() {
    delete[] vertexColorArr;
    delete[] vertexPositionArr;
    delete[] vertexNormalArr;
    delete[] indexArr;
}

void Shape::generateVertexNormals() {
    for (int i = 0; i < vertexCount; i++) {vertexNormalArr[i] = Vector3();}
    GLuint i1,i2,i3;
    Vector3 prod;
    for (int i = 0; i < indexCount/3; i++) {
        i1 = indexArr[3*i];
        i2 = indexArr[3*i+1];
        i3 = indexArr[3*i+2];
        prod = (vertexPositionArr[i2]-vertexPositionArr[i1]).cross(vertexPositionArr[i3]-vertexPositionArr[i1]);
        vertexNormalArr[i1] += prod;
        vertexNormalArr[i2] += prod;
        vertexNormalArr[i3] += prod;
    }
    for (int i = 0; i < vertexCount; i++) {vertexNormalArr[i].normalize();}
}

unsigned int Shape::numVertices() const {return vertexCount;}
unsigned int Shape::numIndices() const {return indexCount;}

const Vector3* Shape::getVertexPositions() const {return vertexPositionArr;}
const VertexColor* Shape::getVertexColors() const {return vertexColorArr;}
const GLuint* Shape::getIndices() const {return indexArr;}

void Shape::writeVertexPositionsAndNormals(Vector3* arr) const {
    for (int i = 0; i < vertexCount; i++) {
        arr[2*i] = vertexPositionArr[i];
        arr[2*i+1] = vertexNormalArr[i];
    }
}
void Shape::writeVertexColors(VertexColor* arr) const {
    std::copy(vertexColorArr, vertexColorArr+vertexCount, arr);
}
void Shape::writeIndices(GLuint* arr, int offset) const {
    for (int i = 0; i < indexCount; i++) {
        arr[i] = indexArr[i] + offset;
    }
}

Shape Shape::cube(Vector3 pos, GLfloat sideLength, VertexColor color) {
    Vector3 positions[8] = {
            {pos.x-sideLength/2,pos.y-sideLength/2,pos.z-sideLength/2},
            {pos.x+sideLength/2,pos.y-sideLength/2,pos.z-sideLength/2},
            {pos.x-sideLength/2,pos.y+sideLength/2,pos.z-sideLength/2},
            {pos.x+sideLength/2,pos.y+sideLength/2,pos.z-sideLength/2},
            {pos.x-sideLength/2,pos.y-sideLength/2,pos.z+sideLength/2},
            {pos.x+sideLength/2,pos.y-sideLength/2,pos.z+sideLength/2},
            {pos.x-sideLength/2,pos.y+sideLength/2,pos.z+sideLength/2},
            {pos.x+sideLength/2,pos.y+sideLength/2,pos.z+sideLength/2}
    };
    VertexColor colors[8] = {color,color,color,color,color,color,color,color};
    GLuint indices[] {
            0,3,1,
            0,2,3,
            4,5,7,
            4,7,6,
            0,1,5,
            0,5,4,
            2,7,3,
            2,6,7,
            0,6,2,
            0,4,6,
            1,3,7,
            1,7,5

    };
    return Shape(8,positions,colors,36,indices);
}

Shape Shape::tiledFloor(Vector3 pos, real sideLength, real tileSideLength, VertexColor color1, VertexColor color2) {
    int tileCount = (int) (sideLength/tileSideLength);
    Vector3* positions = new Vector3[tileCount*tileCount*4];
    VertexColor* colors = new VertexColor[tileCount*tileCount*4];
    GLuint* indices = new GLuint[tileCount*tileCount*6];

    int i;
    Vector3 v;
    VertexColor c;
    for (int iz = 0; iz < tileCount; iz++) {
        for (int ix = 0; ix < tileCount; ix++) {
            i = iz * tileCount + ix;

            v = pos + Vector3(tileSideLength*(ix-tileCount/2.0f), 0,tileSideLength*(iz - tileCount / 2.0f));
            positions[4*i] = v;
            positions[4*i+1] = v + Vector3(tileSideLength,0,0);
            positions[4*i+2] = v + Vector3(0,0,tileSideLength);
            positions[4*i+3] = v + Vector3(tileSideLength,0,tileSideLength);

            c = (ix + iz) % 2 == 0 ? color1 : color2;
            colors[4*i] = c;
            colors[4*i+1] = c;
            colors[4*i+2] = c;
            colors[4*i+3] = c;

            indices[6*i+0] = 4*i+0; indices[6*i+1] = 4*i+3;indices[6*i+2] = 4*i+1;
            indices[6*i+3] = 4*i+0; indices[6*i+4] = 4*i+2;indices[6*i+5] = 4*i+3;
        }
    }

    Shape s = Shape(tileCount*tileCount*4,positions,colors,tileCount*tileCount*6,indices);
    delete[] positions;
    delete[] colors;
    delete[] indices;
    return s;
}
