#ifndef CUBE_H
#define CUBE_H

#include "ShaderProgram.h"
#include <GL/glew.h>
#include "glm/gtx/transform.hpp"

class Cube {

public:

    Cube(glm::vec3& position, ShaderProgram& shaderProgram);

    void animateAndRender(float deltaTime);

private:

    ShaderProgram& shaderProgram;
    GLint modelMatrixLocation;
    GLint colorLocation;

    GLuint vbo;
    GLuint vao;

    glm::vec3& position;
    float angleX;
    float angleY;
};

#endif
