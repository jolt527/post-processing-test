#version 330 core

out vec4 fragColor;

in vec2 textureCoordinates;

uniform sampler2D theTexture;
uniform int lineThickness;

void main() {
    if (int(gl_FragCoord.y / lineThickness) % 2 == 0) {
        discard;
    } else {
        fragColor = vec4(texture(theTexture, textureCoordinates).rgb, 1.0f);
    }
}
