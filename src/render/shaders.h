#ifndef PHYSICSENGINE_SHADERS_H
#define PHYSICSENGINE_SHADERS_H

#include <sdl_opengl.h>

const GLchar* BASIC_VERT_SHADER_SRC = R"(
#version 410 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec4 vColor;
layout (location = 2) in vec3 vNormal;
out vec4 vertexColor;
out vec3 vertexNormal;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix*viewMatrix*vec4(vPos, 1);
    vertexColor = vColor;
    vertexNormal = vNormal;
}
)";

const GLchar* BASIC_FRAG_SHADER_SRC = R"(
#version 410 core
in vec4 vertexColor;
in vec3 vertexNormal;
out vec4 fragColor;

uniform vec3 lightDir;

void main() {
    float light = max(-dot(normalize(vertexNormal),lightDir),0.1);
    fragColor = vertexColor * light;
}
)";

#endif //PHYSICSENGINE_SHADERS_H
