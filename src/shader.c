#include "shader.h"
#include <assert.h>
#include <string.h>

#define MY_KILOBYTE_SIZE 1024
#define MY_MEGABYTE_SIZE MY_KILOBYTE_SIZE * 1000

Shader CreateShader(const char* vShaderPath, const char* fShaderPath)
{
    Shader shader_s;
    int success;
    char infoLog[512];

    GLuint vertexShader     = compileShader(vShaderPath, GL_VERTEX_SHADER);
    GLuint fragmentShader   = compileShader(fShaderPath, GL_FRAGMENT_SHADER);

    shader_s.id = glCreateProgram();
    glAttachShader(shader_s.id, vertexShader);
    glAttachShader(shader_s.id, fragmentShader);
    glLinkProgram(shader_s.id);

    glGetProgramiv(shader_s.id, GL_LINK_STATUS, &success);

    if(!success){
        glGetProgramInfoLog(shader_s.id, 512, NULL, infoLog);
        printf("Linking shader Error: %s", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shader_s;
}

void activateShader(const Shader* shader_s){ glUseProgram(shader_s->id); }

char* loadShaderSrc(const char* filePath)
{
    FILE* fp = fopen(filePath, "rb");
    assert(fp != NULL && "ERROR OPENING SHADER FILE PATH");

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *buffer = malloc(size + 1);
    if (buffer == NULL) {
        perror("malloc");
        fclose(fp);
        assert(buffer != NULL && "Malloc error");
    }

    size_t bytesRead = fread(buffer, 1, size, fp);
    if (bytesRead != (size_t)size) {
        perror("fread");
        free(buffer);
        fclose(fp);
        assert(0 && "Fread error");
    }

    buffer[size] = '\0';
    fclose(fp);

    return buffer;   // caller owns this now, must free it
}



GLuint compileShader(const char* filePath, GLenum type)
{
    int success;
    char infoLog[512];

    GLuint ret = glCreateShader(type);

    char* shaderSrc = loadShaderSrc(filePath);   
    const GLchar* shader = shaderSrc;            
    glShaderSource(ret, 1, &shader, NULL);       
    glCompileShader(ret);

    glGetShaderiv(ret, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(ret, 512, NULL, infoLog);
        printf("Error with vertex shader compilations: %s", infoLog);
    }

    free(shaderSrc);   
    return ret;
}


