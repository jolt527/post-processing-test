#version 330 core

layout (location = 0) in vec3 iVertex;
layout (location = 1) in vec2 iTextureCoordinates;

out vec2 textureCoordinates;

void main() {
    textureCoordinates = iTextureCoordinates;
    gl_Position = vec4(iVertex, 1.0f);
}
