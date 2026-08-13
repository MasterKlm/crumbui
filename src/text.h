#ifndef TEXT_H
#define TEXT_H

#include <glad/glad.h>
#include "charactermap.h"
#include "shader.h"
#include <GLFW/glfw3.h>

void RenderText(Shader *s, CharacterMap *character_map, unsigned int vao, unsigned int vbo,
                 const char* text, float x, float y, float scale, vec3 color);

void MeasureText(CharacterMap *character_map, const char* text, float scale, float* outWidth, float* outHeight);

void loadFont(const char* fontPath, Shader* textShader, CharacterMap** Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO);
#endif