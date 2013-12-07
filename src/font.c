#include "renderlib.h"
#include "surface.h"
#include "utils.h"
#include "font.h"


// Load a font
//
Font* fontLoad(const char* fileName)
{
    Font*	    Fnt;
    uint32_t    i;
    uint32_t    x, y;
    uint32_t    Char;
    uint32_t    glyphW, glyphH;
    Surface*    Surf;


    Surf = surfaceReadFromPNG(fileName);
    if (!Surf)
        return NULL;

    Fnt = calloc(1, sizeof(Font));
    Fnt->Width = Surf->Width / 256;
    Fnt->Height = Surf->Height / 2;
    Fnt->minWidth = Fnt->Width;

    // Extract glyphs
    Fnt->Glyphs = calloc(256, sizeof(Surface*));
    for (i = 0; i < 256; i++)
    {

        // Determine glyph width by scanning the glyph surface for any visible pixel
        glyphW = 0;
        glyphH = 0;
        for(x = 0; x < Fnt->Width; x++)
        {
            for(y = 0; y < Fnt->Height; y++)
            {
                if (Surf->ptrRows[y][x + i * Fnt->Width])
                {
                    glyphW = MAX(x, glyphW);
                    glyphH = MAX(y, glyphH);
                }
            }
        }
        glyphH++;
        glyphW++;

        // Extract new glyph surface if needed
        if (glyphW && glyphH)
        {
            Fnt->Glyphs[i] = surfaceCreate(glyphW, glyphH);
            surfaceExtract(Surf, Fnt->Glyphs[i], i * Fnt->Width, 0);

            // Keep track of minimum character width
            Fnt->minWidth = MIN(glyphW, Fnt->minWidth);
        }

    }

    // Scan font lower surface for baseline pixel
    Fnt->baseLine = Fnt->Height;
    for(y = Fnt->Height + 1; y < (uint32_t)Surf->Height; y++)
    {
        if (Surf->ptrRows[y][0] == 0x76000000)
        {
            Fnt->baseLine = y - Fnt->Height;
            break;
        }
    }

    // Scan font lower surface for kerning pixels
    Fnt->Kerning = calloc(256, sizeof(uint8_t));
    for (Char = 0; Char < 256; Char++)
    {
        Fnt->Kerning[Char] = Fnt->Width - 1;
        for (x = 0; x < Fnt->Width; x++)
        {
            if (Surf->ptrRows[Fnt->Height + 1][Char * Fnt->Width + x] == 0xBA000000)
            {
                Fnt->Kerning[Char] = x;
                break;
            }
        }
    }

    surfaceDestroy(Surf);
    return Fnt;
}

// Destroy a font
//
void fontDestroy(Font* Fnt)
{
    uint32_t i;


    if (!Fnt)
        return;

    // Glyph surfaces
    for (i = 0; i < 256; i++)
    {
        if (Fnt->Glyphs[i])
            surfaceDestroy(Fnt->Glyphs[i]);
    }
    free(Fnt->Glyphs);

    free(Fnt->Kerning);
    free(Fnt);
}

// Return the pixel width of a string of text
//
uint32_t fontStringWidth(const Font* Fnt, char* Text)
{
    uint32_t Width = 0;


    if (!Text)
        return 0;

    while(*(Text++))
        Width += Fnt->Kerning[*Text] + 1;

    return Width;
}

// Return the number of characters that fit into a given pixel width
//
uint32_t fontStringMax(const Font* Fnt, char* Text, const uint32_t Width)
{
    uint32_t X = 0;
    uint32_t cChar = 0;


    while(*(Text++))
    {
        X += Fnt->Kerning[*Text] + 1;
        if (X >= Width)
            break;

        cChar++;
    }

    return cChar;
}
