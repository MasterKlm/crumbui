#include "text.h"
#include "renderer.h"
#include "settings.h"


void RenderText(Shader *s, CharacterMap *character_map, unsigned int vao, unsigned int vbo,
                 const char* text, float x, float y, float scale, vec3 color)
{
    // activate corresponding render state	
    activateShader(s);
    glUniform3f(glGetUniformLocation(s->id, "textColor"), color[0], color[1], color[2]);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    size_t textLength = strlen(text);
    // iterate through all characters
    for (size_t i = 0; i < textLength; i++)
    {
        CharacterMap *found = findCharacter(character_map, text[i]);
        if (found == NULL) continue; // character not in map (e.g. non-ASCII)
        struct Character ch = found->value;

        float xpos = x + ch.Bearing[0] * scale;  
        float top  = y - ch.Bearing[1] * scale; 
        float w = ch.Size[0] * scale;
        float h = ch.Size[1] * scale;
        float bottom = top + h;

        float vertices[6][4] = {
            { xpos,     top,    0.0f, 0.0f },
            { xpos,     bottom, 0.0f, 1.0f },
            { xpos + w, bottom, 1.0f, 1.0f },

            { xpos,     top,    0.0f, 0.0f },
            { xpos + w, bottom, 1.0f, 1.0f },
            { xpos + w, top,    1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void loadFont(const char* fontPath, Shader* textShader, CharacterMap** Characters, unsigned int* TEXTVAO, unsigned int* TEXTVBO)
{


    FT_Library ft;
    if(FT_Init_FreeType(&ft)){
        printf("Error: Could not init FreeType Lib\n");
        return;
    }

    FT_Face face;
    if(FT_New_Face(ft, fontPath, 0, &face))
    {
        printf("Error loading font\n");
        return;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    for (unsigned char c = 0; c < 128; c++)
    {
        // load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            printf("ERROR::FREETYTPE: Failed to load Glyph");
            continue;
        }
        // generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        struct Character character = {
            texture,
            { (float)face->glyph->bitmap.width, (float)face->glyph->bitmap.rows },
            { (float)face->glyph->bitmap_left, (float)face->glyph->bitmap_top },
            (unsigned int)face->glyph->advance.x
        };

        // heap-allocate: uthash stores a pointer to this exact struct, not a copy,
        // so it must outlive this loop iteration (a reused stack var would corrupt every entry)
        CharacterMap *entry = malloc(sizeof(CharacterMap));
        entry->key = (char)c;
        entry->value = character;

        addCharacter(Characters, entry);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    *textShader = CreateShader(TEXT_VERTEX_SHADER_PATH, TEXT_FRAGMENT_SHADER_PATH);

    mat4 projection;
    glm_ortho(0.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0f, -1.0f, 1.0f, projection);

    glGenVertexArrays(1, TEXTVAO);
    glGenBuffers(1, TEXTVBO);
    glBindVertexArray(*TEXTVAO);
    glBindBuffer(GL_ARRAY_BUFFER, *TEXTVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(*TEXTVAO);      


    activateShader(textShader);
    glUniformMatrix4fv(glGetUniformLocation(textShader->id, "projection"), 1, GL_FALSE, (float*)projection);

}