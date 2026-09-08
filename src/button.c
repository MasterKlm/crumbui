#include "button.h"
#include "renderer.h"  


Button Make_Button(const char* text, const char* fontFilePath, Shader* textShader, CharacterMap** Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO){
    loadFont(fontFilePath, textShader, Characters, TEXTVAO, TEXTVBO);
    Button b = {0};
    strcpy(b.text, text);
    return b;
}


void Button_SetCenterText(Button* button, bool center)
{
    button->centerText = center;
}


void Button_SetFont(Button* button, const char* fontFilePath, Shader* textShader, CharacterMap** Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO)
{
    loadFont(fontFilePath, textShader, Characters, TEXTVAO, TEXTVBO);
}


void Button_SetRounding(Button* button, float radius)
{
    button->rounding = radius;
}


void Button_SetPadding(Button* button, float padding)
{
    button->padding = padding;
}


void Button_SetOutline(Renderer* renderer, Button* button, bool outline, float borderWidth, vec4 borderColor)
{
    button->isOutline = outline;
    button->borderWidth = borderWidth;
    glm_vec4_copy(borderColor, button->borderColor);
    button->color[3] = outline ? 0.0f : button->baseAlpha;

    free(button->vertices);
    button->vertices = calcQuadVertromfWidth(button->x, button->y, button->width, button->height,
                                              button->color[0], button->color[1], button->color[2], button->color[3],
                                              &button->verticesSize);
    UpdateButtonGPURendererData(renderer, button);
}


void Button_SetOpacity(Renderer* renderer, Button* button, float opacity)
{
    button->color[3] = opacity;
    button->baseAlpha = opacity;
    free(button->vertices);
    button->vertices = calcQuadVertromfWidth(button->x, button->y, button->width, button->height, button->color[0], button->color[1], button->color[2], button->color[3], &button->verticesSize);
    UpdateButtonGPURendererData(renderer, button);
}


void Render_Button_Text(Button* button, Shader *s, CharacterMap *Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO, float x, float y, float scale)
{
    if (button->centerText)
    {
        float textWidth, textHeight;
        MeasureText(Characters, button->text, scale, &textWidth, &textHeight);
        x = button->x + (button->width - textWidth) / 2.0f;
        y = button->y + (button->height + textHeight) / 2.0f;
    }
    else
    {
        x += button->padding;
        y += button->padding;
    }

    RenderText(s, Characters, *TEXTVAO, *TEXTVBO, button->text, x, y, scale, button->textColor);
}


bool isHovered(Renderer* renderer, Button* button, double mouse_x, double mouse_y)
{
    float mouseNDCXpos =  convertPixelXToNDC(mouse_x);
    float mouseNDCYpos = convertPixelYToNDC(mouse_y); 
    float buttonNDCXpos = convertPixelXToNDC(button->x);
    float buttonNDCYpos = convertPixelYToNDC(button->y);
    float buttonNDCWidth = convertPixelWidthToNDC(button->width);
    float buttonNDCHeight = convertPixelHeightToNDC(button->height);
    //printf("%f\n", (buttonNDCXpos + buttonNDCWidth));
    bool hovered = (mouseNDCXpos >= buttonNDCXpos && mouseNDCXpos <= (buttonNDCXpos + buttonNDCWidth) &&
                mouseNDCYpos <= buttonNDCYpos && mouseNDCYpos >= (buttonNDCYpos - buttonNDCHeight));
    if(hovered){
        //printf("Is hovered\n");

        button->color[3] = .5f;
        free(button->vertices);
        button->vertices = calcQuadVertromfWidth(button->x, button->y, button->width, button->height, button->color[0], button->color[1], button->color[2], button->color[3], &button->verticesSize);
        UpdateButtonGPURendererData(renderer, button);



    }
    else{
        //printf("Original Alpha Val: %f\n", button->baseAlpha);
        
        if(button->color[3] != button->baseAlpha){
            button->color[3] = button->baseAlpha;
            free(button->vertices);
            button->vertices = calcQuadVertromfWidth(button->x, button->y, button->width, button->height, button->color[0], button->color[1], button->color[2], button->color[3], &button->verticesSize);
            UpdateButtonGPURendererData(renderer, button);
        }
        
    }

    return hovered;
}