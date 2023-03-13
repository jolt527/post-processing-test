#version 330 core

out vec4 fragColor;

in vec2 textureCoordinates;

uniform sampler2D theTexture;

void main() {
    int xBlocks = 150;
    int yBlocks = 150;
    float xBucketSize = 1.0f / xBlocks;
    float yBucketSize = 1.0f / yBlocks;

    vec2 adjustedTextureCoordinates = vec2(
        floor(textureCoordinates.x / xBucketSize) * xBucketSize,
        floor(textureCoordinates.y / yBucketSize) * yBucketSize
    );
    fragColor = vec4(
        texture(theTexture, adjustedTextureCoordinates).rgb,
        1.0f
    );
}
