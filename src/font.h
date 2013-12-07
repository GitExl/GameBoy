#ifndef H_FONT
#define H_FONT

#include "surface.h"


typedef struct
{
    uint8_t     baseLine;
    uint8_t*    Kerning;

    uint8_t     Width, Height;
    uint8_t     minWidth;

    Surface**   Glyphs;
} Font;


Font*       fontLoad        (const char* fileName);
void        fontDestroy     (Font* Fnt);
uint32_t    fontStringWidth (const Font* Fnt, char* Text);
uint32_t    fontStringMax   (const Font* Fnt, char* Text, uint32_t Width);


#endif
