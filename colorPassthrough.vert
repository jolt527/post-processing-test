#version 330 core

layout (location = 0) in vec3 iVertex;
layout (location = 1) in vec3 iColor;

out vec3 color;

uniform mat4 modelMatrix;

void main() {
    color = iColor;
    gl_Position = modelMatrix * vec4(iVertex, 1.0);
}
