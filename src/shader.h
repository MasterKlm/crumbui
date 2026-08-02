#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    long int id;
    const char* vertexShaderPath;
    const char* fragmentShaderPath;

} Shader;

Shader CreateShader(const char* vShaderPath, const char* fShaderPath);

void activateShader(const Shader* shader_s);

char* loadShaderSrc(const char* filePath);  

GLuint compileShader(const char* filePath, GLenum type);

#endif