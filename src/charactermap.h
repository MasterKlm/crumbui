#pragma once
#include "uthash.h"
#include <cglm/cglm.h>
#include <ft2build.h>
#include <assert.h>
#include FT_FREETYPE_H  

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    vec2   Size;       // Size of glyph
    vec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

typedef struct CharacterMap
{
    char key;
    struct Character value;

    UT_hash_handle hh;
} CharacterMap;

static void addCharacter(CharacterMap **map, CharacterMap *item)
{
    HASH_ADD(hh, *map, key, sizeof(char), item);
}

static CharacterMap *findCharacter(CharacterMap *map, char c)
{
    CharacterMap *result;

    HASH_FIND(hh, map, &c, sizeof(char), result);

    return result;
}