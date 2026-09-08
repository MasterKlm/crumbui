#ifndef RENDERER_H
#define RENDERER_H
#include <glad/glad.h>
#include "shader.h"
#include <cglm/cglm.h>
#include "helpers.h"
#include "charactermap.h"
#define BUTTON_RECT_VERTEX_SHADER_PATH  "assets/vertex_core.shader"
#define BUTTON_RECT_FRAGMENT_SHADER_PATH "assets/fragment_core.shader"
#define TEXT_VERTEX_SHADER_PATH "assets/text_vertex_core.shader"
#define TEXT_FRAGMENT_SHADER_PATH "assets/text_frag_core.shader"


typedef struct Button Button;

typedef struct Renderer {
    unsigned int VAO, VBO, EBO;
    unsigned int TEXTVAO, TEXTVBO;
    Shader rectShader;
    Shader textShader;
    CharacterMap* Characters;

} Renderer;


void PushVerticesToRenderer(Renderer* renderer, float vertices[], size_t verticesSize);
void UpdateButtonGPURendererData(Renderer* renderer, Button* button);
void Renderer_ProcessInput(Renderer* renderer, Button* button, double mouse_x, double mouse_y);
//Renderer Create_Renderer(const char* vertexShaderPath, const char* fragShaderPath);
Renderer Create_Renderer();

Button Create_Button(Renderer* renderer, const char* text, const char* fontFilePath, float x, float y, float width, float height, vec4 color, vec3 textColor);
void Renderer_PushButton(Renderer* renderer, Button* button);
void Renderer_DrawButton(Renderer* renderer, Button* button);
void Renderer_DrawButtonOutline(Renderer* renderer, Button* button);

#endif