#ifndef BUTTON_H
#define BUTTON_H
#include <string.h>
#include "text.h"
#include "helpers.h"
#include <stdbool.h>

typedef struct Renderer Renderer;   

typedef struct Button {
    vec4 color;
    vec3 textColor;
    float* vertices;
    float x, y;
    float baseAlpha;  
    float width, height;
    int verticesSize;
    bool isHovered;
    char text[1024];
} Button;


Button Make_Button(const char* text, const char* fontFilePath, Shader* textShader, CharacterMap** Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO);
void Render_Button_Text(Button* button, Shader *s, CharacterMap *Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO, float x, float y, float scale);
bool isHovered(Renderer* renderer, Button* button, double mouse_x, double mouse_y);

#endif