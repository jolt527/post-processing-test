#include "Cube.h"
#include "glm/gtx/transform.hpp"

Cube::Cube(glm::vec3& position, ShaderProgram& shaderProgram)
: shaderProgram(shaderProgram),
  position(position) {
    modelMatrixLocation = shaderProgram.getUniformLocation((char *)"modelMatrix");

    float vertexData[] = {
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,

         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,    0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,    0.0f, 1.0f, 0.0f,

         0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,    1.0f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,    0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,    0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,    1.0f, 0.5f, 0.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 0.5f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 0.5f, 0.0f,
         0.5f,  0.5f, -0.5f,    1.0f, 0.5f, 0.0f,
        -0.5f,  0.5f, -0.5f,    1.0f, 0.5f, 0.0f,
        -0.5f,  0.5f,  0.5f,    1.0f, 0.5f, 0.0f,

        -0.5f, -0.5f, -0.5f,    0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,    0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.5f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,    0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,    0.5f, 0.0f, 1.0f,
    };

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

void Cube::animateAndRender(float deltaTime) {
    angleY += 90.0f * deltaTime;
    angleX += 32.0f * deltaTime;
    glm::mat4 modelMatrix =
        glm::perspective(90.0f, 1280.0f / 720.0f, 0.1f, 100.0f) *
        glm::translate(position) *
        glm::rotate(glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f)) *
        glm::rotate(glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f))
    ;

    shaderProgram.use();
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
