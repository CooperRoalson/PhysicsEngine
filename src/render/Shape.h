#ifndef PHYSICSENGINE_GEOMETRY_H
#define PHYSICSENGINE_GEOMETRY_H

#include <sdl_opengl.h>
#include "../core/Vector3.h"
#include "../core/Matrix4.h"

struct VertexColor {
    GLfloat r,g,b;
};

const static VertexColor C_WHITE{1,1,1};
const static VertexColor C_BLACK{0,0,0};
const static VertexColor C_RED{1,0,0};
const static VertexColor C_YELLOW{1,1,0};
const static VertexColor C_GREEN{0,1,0};
const static VertexColor C_BLUE{0,0,1};
const static VertexColor C_PURPLE{1,0,1};

class Shape {
private:
    bool flatShaded;
    unsigned int vertexCount;
    VertexColor* vertexColorArr;
    Vector3* vertexPositionArr;
    Vector3* vertexNormalArr;
    unsigned int indexCount;
    GLuint* indexArr;

    void generateVertexNormals();

    static GLuint getOrMakeMidpoint(GLuint offset, GLuint& numNewVertices, Vector3* positionArr, Vector3* normalArr, VertexColor* colorArr, GLuint v1, GLuint v2, GLuint* idxArr1, GLuint* idxArr2);

public:
    Shape(unsigned int numVertices, const Vector3* vertexPositions, const VertexColor* vertexColors, unsigned int numIndices, const GLuint* indices, bool flatShading);

    Shape(const Shape& oldShape);

    ~Shape();

    Shape& operator=(Shape const&);

    unsigned int numVertices() const;
    unsigned int numIndices() const;

    const Vector3* getVertexPositions() const;
    const Vector3* getVertexNormals() const;
    const VertexColor* getVertexColors() const;
    const GLuint* getIndices() const;

    void writeVertexPositionsAndNormals(Vector3* arr, Matrix4 transform) const;
    void writeVertexColors(VertexColor* arr) const;
    void writeIndices(GLuint* arr, int offset) const;

    Shape subdivided();

    static Shape cube(Vector3 pos, GLfloat sideLength, VertexColor color, bool flatShading);
    static Shape icosahedron(Vector3 pos, GLfloat radius, VertexColor color);
    static Shape icosphere(Vector3 pos, GLfloat radius, VertexColor color, int iterations);
    static Shape tiledFloor(Vector3 pos, real sideLength, real tileSideLength, VertexColor color1, VertexColor color2);

    bool isFlatShaded() const;
};

#endif //PHYSICSENGINE_GEOMETRY_H
