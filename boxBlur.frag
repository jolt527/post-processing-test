#version 330 core

out vec4 fragColor;

uniform sampler2D theTexture;
uniform vec2 dimensions;
uniform int blurRadius;

void main() {
    int xCoordinate = int(gl_FragCoord.x);
    int yCoordinate = int(gl_FragCoord.y);
    if (xCoordinate <= blurRadius - 1 || xCoordinate >= dimensions.x - blurRadius ||
        yCoordinate <= blurRadius - 1 || yCoordinate >= dimensions.y - blurRadius) {

        vec2 textureCoordinates = vec2(xCoordinate / dimensions.x, yCoordinate / dimensions.y);
        fragColor = vec4(
            texture(theTexture, textureCoordinates).rgb,
            1.0f
        );
    } else {
        float rSum = 0.0f;
        float gSum = 0.0f;
        float bSum = 0.0f;
        for (int y = yCoordinate - blurRadius; y <= yCoordinate + blurRadius; y++) {
            for (int x = xCoordinate - blurRadius; x <= xCoordinate + blurRadius; x++) {
                vec2 textureCoordinates = vec2(x / dimensions.x, y / dimensions.y);
                vec3 textureColor = texture(theTexture, textureCoordinates).rgb;
                rSum += textureColor.r;
                gSum += textureColor.g;
                bSum += textureColor.b;
            }
        }

        int total = (2 * blurRadius + 1) * (2 * blurRadius + 1);
        fragColor = vec4(
            rSum / total,
            gSum / total,
            bSum / total,
            1.0f
        );
    }
}
