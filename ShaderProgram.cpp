#include "ShaderProgram.h"

#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#define INFO_LOG_SIZE 512

bool ShaderProgram::loadShadersFromFile(const char* vertexShaderFilename, const char* fragmentShaderFilename) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexShaderFilename);
        fShaderFile.open(fragmentShaderFilename);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        return false;
    }
    const char *vertexShaderSource = vertexCode.c_str();
    const char *fragmentShaderSource = fragmentCode.c_str();

    char infoLog[INFO_LOG_SIZE];
    GLint status;

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(vertexShader, INFO_LOG_SIZE, NULL, infoLog);
        std::cerr << "ERROR: vertex shader compliation failed\n" << infoLog << std::endl;
        return false;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, NULL, infoLog);
        std::cerr << "ERROR: fragment shader compliation failed\n" << infoLog << std::endl;
        return false;
    }

    shaderProgramId = glCreateProgram();
    glAttachShader(shaderProgramId, vertexShader);
    glAttachShader(shaderProgramId, fragmentShader);
    glLinkProgram(shaderProgramId);
    glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(shaderProgramId, INFO_LOG_SIZE, NULL, infoLog);
        std::cerr << "ERROR: shader program linking failed\n" << infoLog << std::endl;
        return false;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return true;
}

GLint ShaderProgram::getUniformLocation(char* name) {
    return glGetUniformLocation(shaderProgramId, name);
}

void ShaderProgram::use() {
    glUseProgram(shaderProgramId);
}

void ShaderProgram::cleanup() {
    glDeleteProgram(shaderProgramId);
}
