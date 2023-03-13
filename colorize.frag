#version 330 core

out vec4 fragColor;

in vec2 textureCoordinates;

uniform sampler2D theTexture;
uniform vec3 selectedColor;

void main() {
    vec3 textureColor = texture(theTexture, textureCoordinates).rgb;
    float averageColorValue = 0.2125f * textureColor.r + 0.7154f * textureColor.g + 0.0721 * textureColor.b;
    fragColor = vec4(
        selectedColor * averageColorValue,
        1.0f
    );
}
