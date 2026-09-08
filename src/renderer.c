#include "renderer.h"
#include "button.h"

#define BUTTON_TEXT_MARGIN_X 10.0f
#define BUTTON_TEXT_MARGIN_Y 22.0f

void PushVerticesToRenderer(Renderer* renderer, float vertices[], size_t verticesSize)
{
    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    glBindVertexArray(renderer->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    activateShader(&renderer->rectShader);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}


void UpdateButtonGPURendererData(Renderer* renderer, Button* button)
{
    glBindBuffer(GL_ARRAY_BUFFER, renderer->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, button->verticesSize, button->vertices);
}


Renderer Create_Renderer()
{

    Renderer r = {0};

    glGenVertexArrays(1, &r.VAO);
    glGenBuffers(1, &r.VBO);
    glGenBuffers(1, &r.EBO);

    r.rectShader = CreateShader(BUTTON_RECT_VERTEX_SHADER_PATH, BUTTON_RECT_FRAGMENT_SHADER_PATH);


    return r;
}


Button Create_Button(Renderer* renderer, const char* text, const char* fontFilePath, float x, float y, float width, float height, vec4 color, vec3 textColor){
    Button button = Make_Button(text, fontFilePath, &renderer->textShader, &renderer->Characters, &renderer->TEXTVAO, &renderer->TEXTVBO);
    button.x = x;
    button.y = y;
    button.width = width;
    button.height = height;
    button.baseAlpha = color[3];
    glm_vec4_copy(color, button.color);
    glm_vec3_copy(textColor, button.textColor);
    //printf("Vec3 bytes: %d\n",  sizeof(vec3));
    button.vertices = calcQuadVertromfWidth(x, y, width, height, button.color[0], button.color[1], button.color[2], button.color[3], &button.verticesSize);
    return button;
}


void Renderer_PushButton(Renderer* renderer, Button* button)
{
    PushVerticesToRenderer(renderer, button->vertices, button->verticesSize);
}

void Renderer_ProcessInput(Renderer* renderer, Button* button, double mouse_x, double mouse_y)
{
    
    button->isHovered = isHovered(renderer, button, mouse_x, mouse_y);
    
}

void Renderer_DrawButton(Renderer* renderer, Button* button)
{
    activateShader(&renderer->rectShader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);
    float rectX = button->x;
    float rectY = (float)viewport[3] - button->y - button->height;
    glUniform2f(glGetUniformLocation(program, "uRectPos"), rectX, rectY);
    glUniform2f(glGetUniformLocation(program, "uRectSize"), button->width, button->height);
    glUniform1f(glGetUniformLocation(program, "uRadius"), button->rounding);

    glBindVertexArray(renderer->VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    if (button->isOutline)
    {
        Renderer_DrawButtonOutline(renderer, button);
    }

    Render_Button_Text(button, &renderer->textShader, renderer->Characters,
                  &renderer->TEXTVAO, &renderer->TEXTVBO,
                  button->x + BUTTON_TEXT_MARGIN_X, button->y + BUTTON_TEXT_MARGIN_Y, (button->width / button->height) * 0.1f );
}


static void DrawBorderQuad(Renderer* renderer, GLint program, GLint viewportHeight, float x, float y, float w, float h, vec4 color)
{
    size_t vertsSize;
    float* verts = calcQuadVertromfWidth(x, y, w, h, color[0], color[1], color[2], color[3], &vertsSize);
    PushVerticesToRenderer(renderer, verts, vertsSize);
    free(verts);

    float rectY = (float)viewportHeight - y - h;
    glUniform2f(glGetUniformLocation(program, "uRectPos"), x, rectY);
    glUniform2f(glGetUniformLocation(program, "uRectSize"), w, h);
    glUniform1f(glGetUniformLocation(program, "uRadius"), 0.0f);

    glBindVertexArray(renderer->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


void Renderer_DrawButtonOutline(Renderer* renderer, Button* button)
{
    activateShader(&renderer->rectShader);

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    float bw = button->borderWidth;

    DrawBorderQuad(renderer, program, viewport[3], button->x, button->y, button->width, bw, button->borderColor);
    DrawBorderQuad(renderer, program, viewport[3], button->x, button->y + button->height - bw, button->width, bw, button->borderColor);
    DrawBorderQuad(renderer, program, viewport[3], button->x, button->y, bw, button->height, button->borderColor);
    DrawBorderQuad(renderer, program, viewport[3], button->x + button->width - bw, button->y, bw, button->height, button->borderColor);

    PushVerticesToRenderer(renderer, button->vertices, button->verticesSize);
}