#include "Shape.h"

#include <string>

Shape::Shape(unsigned int numVertices, const Vector3* vertexPositions, const VertexColor* vertexColors, unsigned int numIndices, const GLuint* indices, bool flatShading) {
    vertexCount = numVertices;
    indexCount = numIndices;

    vertexPositionArr = new Vector3[vertexCount];
    std::memcpy(vertexPositionArr,vertexPositions,sizeof(Vector3)*vertexCount);

    vertexColorArr = new VertexColor[vertexCount];
    std::memcpy(vertexColorArr,vertexColors,sizeof(VertexColor)*vertexCount);

    indexArr = new GLuint[indexCount];
    std::memcpy(indexArr,indices,sizeof(GLuint)*indexCount);

    flatShaded = flatShading;

    vertexNormalArr = new Vector3[vertexCount];
    generateVertexNormals();
}

Shape::Shape(const Shape& oldShape) {
    vertexCount = oldShape.vertexCount;
    indexCount = oldShape.indexCount;

    vertexPositionArr = new Vector3[vertexCount];
    std::memcpy(vertexPositionArr,oldShape.vertexPositionArr,sizeof(Vector3)*vertexCount);

    vertexColorArr = new VertexColor[vertexCount];
    std::memcpy(vertexColorArr,oldShape.vertexColorArr,sizeof(VertexColor)*vertexCount);

    indexArr = new GLuint[indexCount];
    std::memcpy(indexArr,oldShape.indexArr,sizeof(GLuint)*indexCount);

    flatShaded = oldShape.flatShaded;

    vertexNormalArr = new Vector3[vertexCount];
    std::memcpy(vertexNormalArr,oldShape.vertexNormalArr,sizeof(Vector3)*vertexCount);
}

Shape::~Shape() {
    delete[] vertexColorArr;
    delete[] vertexPositionArr;
    delete[] vertexNormalArr;
    delete[] indexArr;
}

Shape& Shape::operator=(const Shape& s) {
    delete[] vertexColorArr;
    delete[] vertexPositionArr;
    delete[] vertexNormalArr;
    delete[] indexArr;

    vertexCount = s.vertexCount;
    indexCount = s.indexCount;

    vertexPositionArr = new Vector3[vertexCount];
    std::memcpy(vertexPositionArr,s.vertexPositionArr,sizeof(Vector3)*vertexCount);

    vertexColorArr = new VertexColor[vertexCount];
    std::memcpy(vertexColorArr,s.vertexColorArr,sizeof(VertexColor)*vertexCount);

    indexArr = new GLuint[indexCount];
    std::memcpy(indexArr,s.indexArr,sizeof(GLuint)*indexCount);

    flatShaded = s.flatShaded;

    vertexNormalArr = new Vector3[vertexCount];
    std::memcpy(vertexNormalArr,s.vertexNormalArr,sizeof(Vector3)*vertexCount);

    return *this;

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
        if (!flatShaded) {
            vertexNormalArr[i2] += prod;
            vertexNormalArr[i3] += prod;
        }
    }
    for (int i = 0; i < vertexCount; i++) {vertexNormalArr[i] = vertexNormalArr[i].normalized();}
}

unsigned int Shape::numVertices() const {return vertexCount;}
unsigned int Shape::numIndices() const {return indexCount;}
bool Shape::isFlatShaded() const {return flatShaded;}

const Vector3* Shape::getVertexPositions() const {return vertexPositionArr;}
const VertexColor* Shape::getVertexColors() const {return vertexColorArr;}
const GLuint* Shape::getIndices() const {return indexArr;}

void Shape::writeVertexPositionsAndNormals(Vector3* arr, Matrix4 transform) const {
    for (int i = 0; i < vertexCount; i++) {
        arr[2*i] = Vector3(transform.multiply(vertexPositionArr[i],1));
        arr[2*i+1] = Vector3(transform.multiply(vertexNormalArr[i],0));
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

GLuint Shape::getOrMakeMidpoint(GLuint offset, GLuint& numNewVertices, Vector3 *positionArr, VertexColor *colorArr, GLuint v1,
                                GLuint v2, GLuint *idxArr1, GLuint *idxArr2) {
    GLuint temp = std::max(v1,v2);
    v1 = std::min(v1,v2);
    v2 = temp;

    for (int i = 0; i < numNewVertices; i++) {
        if (idxArr1[i] == v1 && idxArr2[i] == v2) {
            return offset+i;
        }
    }

    positionArr[offset+numNewVertices]  = (positionArr[v1] + positionArr[v2]) / 2;
    colorArr[offset+numNewVertices] = {(colorArr[v1].r + colorArr[v2].r) / 2,
                             (colorArr[v1].g + colorArr[v2].g) / 2,
                             (colorArr[v1].b + colorArr[v2].b) / 2};

    idxArr1[numNewVertices] = v1;
    idxArr2[numNewVertices] = v2;

    return offset + numNewVertices++;

}

Shape Shape::subdivided() {
    Vector3 *positions = new Vector3[vertexCount + indexCount];
    std::memcpy(positions, vertexPositionArr, vertexCount * sizeof(Vector3));

    VertexColor *colors = new VertexColor[vertexCount + indexCount];
    std::memcpy(colors, vertexColorArr, vertexCount * sizeof(VertexColor));

    // Arrays where the ith elements represent the two vertices of which the vertexCount+ith vertex is the midpoint
    GLuint *idxArr1 = new GLuint[indexCount];
    GLuint *idxArr2 = new GLuint[indexCount];
    GLuint newVertices = 0;

    GLuint *indices = new GLuint[indexCount * 4];

    for (int i = 0; i < indexCount / 3; i++) {
        GLuint i1 = indexArr[3 * i];
        GLuint i2 = indexArr[3 * i + 1];
        GLuint i3 = indexArr[3 * i + 2];

        GLuint i4 = getOrMakeMidpoint(vertexCount, newVertices, positions, colors, i1,i2, idxArr1, idxArr2);
        GLuint i5 = getOrMakeMidpoint(vertexCount, newVertices, positions, colors, i2,i3, idxArr1, idxArr2);
        GLuint i6 = getOrMakeMidpoint(vertexCount, newVertices, positions, colors, i3,i1, idxArr1, idxArr2);

        indices[12 * i + 0] = i4; indices[12 * i + 1] = i6; indices[12 * i + 2] = i1;
        indices[12 * i + 3] = i4; indices[12 * i + 4] = i2; indices[12 * i + 5] = i5;
        indices[12 * i + 6] = i5; indices[12 * i + 7] = i3; indices[12 * i + 8] = i6;
        indices[12 * i + 9] = i4; indices[12 * i + 10] = i5; indices[12 * i + 11] = i6;
    }

    delete[] idxArr1;
    delete[] idxArr2;

    Shape result = Shape(vertexCount + newVertices, positions, colors, indexCount * 4, indices, flatShaded);

    delete[] positions;
    delete[] colors;
    delete[] indices;

    return result;
}

Shape Shape::rectangularPrism(Vector3 pos, GLfloat sideLengthX, GLfloat sideLengthY, GLfloat sideLengthZ, VertexColor color, bool flatShading) {
    Vector3 positions[8] = {
            {pos.x-sideLengthX/2,pos.y-sideLengthY/2,pos.z-sideLengthZ/2},
            {pos.x+sideLengthX/2,pos.y-sideLengthY/2,pos.z-sideLengthZ/2},
            {pos.x-sideLengthX/2,pos.y+sideLengthY/2,pos.z-sideLengthZ/2},
            {pos.x+sideLengthX/2,pos.y+sideLengthY/2,pos.z-sideLengthZ/2},
            {pos.x-sideLengthX/2,pos.y-sideLengthY/2,pos.z+sideLengthZ/2},
            {pos.x+sideLengthX/2,pos.y-sideLengthY/2,pos.z+sideLengthZ/2},
            {pos.x-sideLengthX/2,pos.y+sideLengthY/2,pos.z+sideLengthZ/2},
            {pos.x+sideLengthX/2,pos.y+sideLengthY/2,pos.z+sideLengthZ/2}
    };
    VertexColor colors[8] = {color,color,color,color,color,color,color,color};
    GLuint indices[] {
            0,3,1,
            0,2,3,
            4,5,7,
            4,7,6,
            5,0,1,
            5,4,0,
            2,7,3,
            2,6,7,
            6,2,0,
            6,0,4,
            1,3,7,
            1,7,5

    };
    return Shape(8,positions,colors,36,indices,flatShading);
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

    Shape s = Shape(tileCount*tileCount*4,positions,colors,tileCount*tileCount*6,indices,true);
    delete[] positions;
    delete[] colors;
    delete[] indices;
    return s;
}

Shape Shape::icosahedron(Vector3 pos, GLfloat radius, VertexColor color, bool flatShading) {
    real t = 1.61803398875f;
    Vector3 positions[20] = {
            {t,1,0},
            {-t,1,0},
            {t,-1,0},
            {-t,-1,0},
            {1,0,t},
            {1,0,-t},
            {-1,0,t},
            {-1,0,-t},
            {0,t,1},
            {0,-t,1},
            {0,t,-1},
            {0,-t,-1}
    };
    for (int i = 0; i < 12; i++) { positions[i] = positions[i].normalized()*radius + pos; }
    if (flatShading) {
        positions[12] = positions[0];
        positions[13] = positions[0];
        positions[14] = positions[2];
        positions[15] = positions[2];
        positions[16] = positions[1];
        positions[17] = positions[1];
        positions[18] = positions[3];
        positions[19] = positions[3];
    }

    VertexColor colors[20] = {color,color,color,color,color,color,color,color,color,color,color,color,color,color,color,color,color,color,color,color};
    GLuint indices[20*3] {
            0,8,4,
            0,5,10,
            2,4,9,
            2,11,5,
            1,6,8,
            1,10,7,
            3,9,6,
            3,7,11,
            0,10,8,
            1,8,10,
            2,9,11,
            3,11,9,
            4,2,0,
            5,0,2,
            6,1,3,
            7,3,1,
            8,6,4,
            9,4,6,
            10,5,7,
            11,7,5
    };

    if (flatShading) {
        indices[3*0] = 12;
        indices[3*1] = 13;
        indices[3*2] = 14;
        indices[3*3] = 15;
        indices[3*4] = 16;
        indices[3*5] = 17;
        indices[3*6] = 18;
        indices[3*7] = 19;
    }

    return Shape(flatShading ? 20 : 12,positions,colors,20*3,indices,flatShading);
}

Shape Shape::icosphere(Vector3 pos, GLfloat radius, VertexColor color, int iterations) {
    Shape s = icosahedron(Vector3(),1,color, false);

    for (int i = 0; i < iterations; i++) {
        s = s.subdivided();
        for (int v = 0; v < s.vertexCount; v++) { s.vertexPositionArr[v] = s.vertexPositionArr[v].normalized(); }
    }
    for (int v = 0; v < s.vertexCount; v++) { s.vertexPositionArr[v] = s.vertexPositionArr[v]*radius + pos; }

    return s;
}

Shape Shape::cylinder(Vector3 p1, Vector3 p2, GLfloat radius, VertexColor color, int circleVertices, bool flatShading) {
    Vector3 axis = p2-p1;
    // Transformation from the unit cylinder to this cylinder
    Matrix4 transformMat = Matrix4().translate(p1).rotate(M_PI_2 - axis.azimuth(), M_PI_2 - axis.elevation(), 0).scale(radius, axis.magnitude(), radius);

    Vector3 positions[2*circleVertices+2];
    VertexColor colors[2*circleVertices+2];

    // Center points of each base
    positions[0] = transformMat.multiply(Vector4(0,0,0,1));
    colors[0] = color;
    positions[circleVertices+1] = transformMat.multiply(Vector4(0,1,0,1));
    colors[circleVertices+1] = color;

    /* Points on the unit cylinder, transformed by the matrix
     * Order:
     *  -Bottom center
     *  -Bottom circle
     *  -Top center
     *  -Top circle
     */
    double angleIncrement = 2*M_PI/circleVertices, angle;
    for (int i = 0; i < circleVertices; i++) {
        angle = angleIncrement * i;
        positions[1 + i] = transformMat.multiply(Vector4(cos(angle), 0, sin(angle),1));
        positions[circleVertices+2 + i] = transformMat.multiply(Vector4(cos(angle+angleIncrement/2), 1, sin(angle+angleIncrement/2),1));

        colors[1 + i] = color;
        colors[circleVertices+2 + i] = color;
    }

    GLuint indices[3*(4*circleVertices)];

    // Bases
    for (int i = 0; i < circleVertices; i++) {
        indices[3*i+0] = 0;
        indices[3*i+1] = i + 1;
        indices[3*i+2] = (i+1)%circleVertices + 1;

        indices[3*i+0 + 3*circleVertices] = 1 + circleVertices;
        indices[3*i+1 + 3*circleVertices] = (i+1)%circleVertices + 2 + circleVertices;
        indices[3*i+2 + 3*circleVertices] = i + 2 + circleVertices;
    }
    // Walls
    for (int i = 0; i < circleVertices; i++) {
        indices[3*i+0 + 3*2*circleVertices] = i + 1;
        indices[3*i+1 + 3*2*circleVertices] = i + 2+circleVertices;
        indices[3*i+2 + 3*2*circleVertices] = (i+1)%circleVertices + 1;

        indices[3*i+0 + 3*3*circleVertices] = i + 2+circleVertices;
        indices[3*i+1 + 3*3*circleVertices] = (i+1)%circleVertices + 2+circleVertices;
        indices[3*i+2 + 3*3*circleVertices] = (i+1)%circleVertices + 1;
    }

    return Shape(circleVertices*2+2,positions,colors,circleVertices*4*3,indices,flatShading);
}

