#version 330 core

out vec4 fragColor;

in vec2 textureCoordinates;

uniform sampler2D theTexture;

void main() {
    vec3 textureColor = texture(theTexture, textureCoordinates).rgb;
    float grayscaleValue = 0.299f * textureColor.r + 0.587f * textureColor.g + 0.114f * textureColor.b;
    fragColor = vec4(
        grayscaleValue,
        grayscaleValue,
        grayscaleValue,
        1.0f
    );
}
