#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "GL/glew.h"

class ShaderProgram {

public:

    bool loadShadersFromFile(const char* vertexShaderFilename, const char* fragmentShaderFilename);
    GLint getUniformLocation(char* name);
    void use();
    void cleanup();

private:

    GLuint shaderProgramId;
};

#endif
