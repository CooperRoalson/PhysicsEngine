#ifndef PHYSICSENGINE_GEOMETRY_H
#define PHYSICSENGINE_GEOMETRY_H

#include <sdl_opengl.h>
#include "../core/Vector3.h"

struct VertexColor {
    GLfloat r,g,b,a;
};

const static VertexColor C_WHITE{1,1,1,1};
const static VertexColor C_BLACK{0,0,0,1};
const static VertexColor C_RED{1,0,0,1};
const static VertexColor C_YELLOW{1,1,0,1};
const static VertexColor C_GREEN{0,1,0,1};
const static VertexColor C_BLUE{0,0,1,1};
const static VertexColor C_PURPLE{1,0,1,1};

class Shape {
private:
    unsigned int vertexCount;
    VertexColor* vertexColorArr;
    Vector3* vertexPositionArr;
    Vector3* vertexNormalArr;
    unsigned int indexCount;
    GLuint* indexArr;

    void generateVertexNormals();

public:
    Shape(unsigned int numVertices, const Vector3* vertexPositions, const VertexColor* vertexColors, unsigned int numIndices, const GLuint* indices);

    Shape(const Shape& oldShape);

    ~Shape();

    unsigned int numVertices() const;
    unsigned int numIndices() const;

    const Vector3* getVertexPositions() const;
    const Vector3* getVertexNormals() const;
    const VertexColor* getVertexColors() const;
    const GLuint* getIndices() const;

    void writeVertexPositionsAndNormals(Vector3* arr) const;
    void writeVertexColors(VertexColor* arr) const;
    void writeIndices(GLuint* arr, int offset) const;

    static Shape cube(Vector3 pos, GLfloat sideLength, VertexColor color);
    static Shape tiledFloor(Vector3 pos, real sideLength, real tileSideLength, VertexColor color1, VertexColor color2);

};

#endif //PHYSICSENGINE_GEOMETRY_H
