#include "shaders.h"

const GLchar* SMOOTH_VERT_SHADER_SRC = R"(
#version 410 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec3 vNormal;
out vec3 vertexColor;
out vec3 vertexNormal;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix*viewMatrix*vec4(vPos, 1);
    vertexColor = vColor;
    vertexNormal = vNormal;
}
)";

const GLchar* FLAT_VERT_SHADER_SRC = R"(
#version 410 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;
layout (location = 2) in vec3 vNormal;
out vec3 vertexColor;
flat out vec3 vertexNormal;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix*viewMatrix*vec4(vPos, 1);
    vertexColor = vColor;
    vertexNormal = vNormal;
}
)";

const GLchar* SMOOTH_FRAG_SHADER_SRC = R"(
#version 410 core
in vec3 vertexColor;
in vec3 vertexNormal;
out vec4 fragColor;

uniform vec3 lightDir;

void main() {
    float light = max(-dot(normalize(vertexNormal),lightDir),0.1);
    fragColor = vec4(vertexColor * light,1);
}
)";

const GLchar* FLAT_FRAG_SHADER_SRC = R"(
#version 410 core
in vec3 vertexColor;
flat in vec3 vertexNormal;
out vec4 fragColor;

uniform vec3 lightDir;

void main() {
    float light = max(-dot(normalize(vertexNormal),lightDir),0.2);
    fragColor = vec4(vertexColor * light,1);
}
)";