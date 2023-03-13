#version 330 core

out vec4 fragColor;

uniform sampler2D theTexture;
uniform vec2 dimensions;
uniform int blockSize;

void main() {
    vec2 adjustedTextureCoordinates = vec2(
        floor(gl_FragCoord.x / blockSize) * blockSize / dimensions.x,
        floor(gl_FragCoord.y / blockSize) * blockSize / dimensions.y
    );
    fragColor = vec4(
        texture(theTexture, adjustedTextureCoordinates).rgb,
        1.0f
    );
}
