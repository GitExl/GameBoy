#include "renderlib.h"
#include "render.h"
#include "utils.h"


// Pixel full alpha blend
//
static inline uint32_t renderPixelAlpha(const uint32_t Dest, const uint32_t Src)
{

    if (ALPHA(Src) == 0)
        return Dest;

    if (ALPHA(Src) == 0xFF)
        return Src;
    else
    {
        uint8_t  A = ALPHA(Src);
        uint32_t RB = (((Src & 0x00FF00FF) * A) + ((Dest & 0x00FF00FF) * (0xFF - A))) & 0xFF00FF00;
        uint32_t G =  (((Src & 0x0000FF00) * A) + ((Dest & 0x0000FF00) * (0xFF - A))) & 0x00FF0000;

        return (0xFF << 24) | (RB | G) >> 8;
    }

}


// Pixel 50% blend
//
static inline uint32_t renderPixelAlpha50(const uint32_t Dest, const uint32_t Src)
{

    if (ALPHA(Src) == 0xFF)
        return (((Dest & 0x00FEFEFE) >> 1) + ((Src & 0x00FEFEFE) >> 1)) | (0xFF << 24);
    else
        return Dest;

}


// Pixel solid
//
static inline uint32_t renderPixelSolid(const uint32_t Dest, const uint32_t Src)
{

    return (Src & 0x00FFFFFF) | 0xFF000000;

}


// Alpha simple
//
static inline uint32_t renderPixelAlphaSimple(const uint32_t Dest, const uint32_t Src)
{

    if (ALPHA(Src) == 0xFF)
        return Src;
    else
        return Dest;

}


// Render an outline based on an image's alpha channel
// blendOp SOLID
//
void renderOutline(const Surface* destSurf, const Surface* srcSurf, const int32_t rX, const int32_t rY, const uint32_t Color)
{

    int32_t X, Y;
    int32_t cX, cY;


    for(Y = 0; Y < srcSurf->Height; Y++)
    {
        for(X = 0; X < srcSurf->Width; X++)
        {
            if (ALPHA(srcSurf->ptrRows[Y][X]) != 0)
            {
                cX = X + rX;
                cY = Y + rY;


                // Inside destination surface
                if (cX - 1 >= 0 && cX - 1 < destSurf->Width && cY >= 0 && cY < destSurf->Height)

                    // Inside source surface and no source alpha, or outside source surface
                    if ((X - 1 >= 0 && ALPHA(srcSurf->ptrRows[Y][X - 1]) == 0) || X - 1 < 0)

                        // Render
                        destSurf->ptrRows[cY][cX - 1] = renderPixelSolid(destSurf->ptrRows[cY][cX - 1], Color);


                if (cX >= 0 && cX < destSurf->Width && cY - 1 >= 0 && cY - 1 < destSurf->Height)
                    if ((Y - 1 >= 0 && ALPHA(srcSurf->ptrRows[Y - 1][X]) == 0) || Y - 1 < 0)
                        destSurf->ptrRows[cY - 1][cX] = renderPixelSolid(destSurf->ptrRows[cY - 1][cX], Color);

                if (cX + 1 >= 0 && cX + 1 < destSurf->Width && cY >= 0 && cY < destSurf->Height)
                    if ((X + 1 < srcSurf->Width && ALPHA(srcSurf->ptrRows[Y][X + 1]) == 0) || X + 1 >= srcSurf->Width)
                        destSurf->ptrRows[cY][cX + 1] = renderPixelSolid(destSurf->ptrRows[cY][cX + 1], Color);

                if (cX >= 0 && cX < destSurf->Width && cY + 1 >= 0 && cY + 1 < destSurf->Height)
                    if ((Y + 1 < srcSurf->Height && ALPHA(srcSurf->ptrRows[Y + 1][X]) == 0) || Y + 1 >= srcSurf->Height)
                        destSurf->ptrRows[cY + 1][cX] = renderPixelSolid(destSurf->ptrRows[cY + 1][cX], Color);
            }
        }
    }

}


// Render a line
// blendOp SOLID
//
void renderLine(const Surface* destSurf, int32_t X1, int32_t Y1, const int32_t X2, const int32_t Y2, const uint32_t Color)
{

    int32_t deltaX = abs(X2 - X1);
    int32_t deltaY = abs(Y2 - Y1);
    int32_t X = X1;
    int32_t Y = Y1;
    int32_t xInc1 = 0, xInc2 = 0;
    int32_t yInc1 = 0, yInc2 = 0;
    int32_t Den = 0, Num = 0, numAdd = 0, numPixels = 0, curPixel = 0;


    // X increases
    if (X2 >= X1)
    {
        xInc1 = 1;
        xInc2 = 1;
    }

    // X decreases
    else
    {
        xInc1 = -1;
        xInc2 = -1;
    }

    // Y increases
    if (Y2 >= Y1)
    {
        yInc1 = 1;
        yInc2 = 1;
    }

    // Y decreases
    else
    {
        yInc1 = -1;
        yInc2 = -1;
    }


    // There is at least one x-value for every y-value
    if (deltaX >= deltaY)
    {
        xInc1 = 0;
        yInc2 = 0;
        Den = deltaX;
        Num = deltaX >> 2;
        numAdd = deltaY;
        numPixels = deltaX;
    }

    // There is at least one y-value for every x-value
    else
    {
        xInc2 = 0;
        yInc1 = 0;
        Den = deltaY;
        Num = deltaY >> 2;
        numAdd = deltaX;
        numPixels = deltaY;
    }


    for (curPixel = 0; curPixel <= numPixels; curPixel++)
    {
        if ((uint32_t)X < destSurf->Width && (uint32_t)Y < destSurf->Height)
            destSurf->ptrRows[Y][X] = renderPixelSolid(destSurf->ptrRows[Y][X], Color);

        // Increase numerator by the top of the fraction
        Num += numAdd;
        if (Num >= Den)
        {
            Num -= Den;
            X += xInc1;
            Y += yInc1;
        }

        X += xInc2;
        Y += yInc2;
    }

}


// Render multiline text
// Does word-wrapping to fit text into an area
//
void renderMultilineText(const Surface* Surf, const Font* Fnt, const int32_t X, int32_t Y, const uint32_t Width, const uint32_t Height, char* Text, const uint32_t Color)
{

    uint32_t    charCount;
    int32_t     splitIdx;
    char*       renderString;
    uint32_t    i;
    int32_t     rY = 0;


    // Allocate maximum room assuming all characters are minimum width
    renderString = malloc(Width / Fnt->minWidth + 1);

    while(*Text)
    {
        // Count maximum number of characters on current line
        charCount = fontStringMax(Fnt, Text, Width);
        splitIdx = -1;

        // Find newline
        for(i = 0; i < charCount + 1; i++)
        {
            if (*(Text + i) == 0x0A)
            {
                splitIdx = i;
                break;
            }
        }

        // Find last space
        if (splitIdx == -1)
        {
            for(i = charCount + 1; i > 0; i--)
            {
                if (*(Text + i) == 32)
                {
                    splitIdx = i;
                    break;
                }
            }
        }

        // No split found, render entire line
        if (splitIdx == -1)
        {
            charCount++;
            strncpy(renderString, Text, charCount);
            renderString[charCount] = 0;

            Text += charCount;
        }

        // Empty line
        else if (splitIdx == 0)
            Text += splitIdx + 1;

        // Split found, render up until that
        else
        {
            strncpy(renderString, Text, splitIdx);
            renderString[splitIdx] = 0;

            Text += splitIdx + 1;
        }

        // Render
        if (splitIdx != 0)
            renderText(Surf, Fnt, X, Y + rY, renderString, Color);

        // Advance line
        rY += Fnt->baseLine + 3;
        if (rY + Fnt->baseLine + 3 > Height)
            break;
    }

    free(renderString);

}


// Render text to a surface from a font surface
//
void renderText(const Surface* destSurf, const Font* srcFont, int32_t x, const int32_t y, const char* Text, uint32_t Color)
{

    int32_t     cX, cY;
    uint32_t*   Dest;

    int32_t     dX, dY;
    uint8_t     Char;
    Surface*    Glyph;


    // Strip alpha
    Color &= 0x00FFFFFF;

    do
    {
        Char = *Text++;
        if (!Char)
            break;

        // Skip over empty glyphs using their kerning
        Glyph = srcFont->Glyphs[Char];
        if (!Glyph)
            x += srcFont->Kerning[Char] + 1;

        // Render it otherwise
        else
        {
            cY = Glyph->Height;
            while(cY--)
            {
                dY = y + cY;

                if ((uint32_t)dY < destSurf->Height)
                {
                    cX = Glyph->Width;
                    while(cX--)
                    {
                        dX = x + cX;

                        if ((uint32_t)dX < destSurf->Width)
                        {
                            Dest = destSurf->ptrRows[dY] + dX;
                            *Dest = renderPixelAlpha(*Dest, Color | Glyph->ptrRows[cY][cX]);
                        }
                    }
                }
            }

            x += srcFont->Kerning[Char] + 1;
        }

    } while(Char);

}


// Render a box (no fill)
//
void renderBox(const Surface* destSurf, int32_t x, int32_t y, int32_t Width, int32_t Height, const uint32_t Color)
{

    int32_t rX, rY;


    // Normalize
    if (Width < 0)
    {
        Width = abs(Width);
        x -= Width;
    }

    if (Height < 0)
    {
        Height = abs(Height);
        y -= Height;
    }


    for(rX = x + 1; rX < x + Width; rX++)
    {
        if ((uint32_t)y < destSurf->Height && (uint32_t)rX < destSurf->Width)
            destSurf->ptrRows[y][rX] = renderPixelSolid(destSurf->ptrRows[y][rX], Color);

        if ((uint32_t)(y + Height) < destSurf->Height && (uint32_t)rX < destSurf->Width)
            destSurf->ptrRows[y + Height][rX] = renderPixelSolid(destSurf->ptrRows[y + Height][rX], Color);
    }

    for(rY = y; rY < y + Height + 1; rY++)
    {
        if ((uint32_t)rY < destSurf->Height && (uint32_t)x < destSurf->Width)
            destSurf->ptrRows[rY][x] = renderPixelSolid(destSurf->ptrRows[rY][x], Color);

        if ((uint32_t)rY < destSurf->Height && (uint32_t)(x + Width) < destSurf->Width)
            destSurf->ptrRows[rY][x + Width] = renderPixelSolid(destSurf->ptrRows[rY][x + Width], Color);
    }

}


// Blit one surface onto another, no scaling, no alpha blending
//
void renderBlitFast(const Surface* destSurf, const Surface* srcSurf, int32_t X, int32_t Y)
{

    uint32_t* Dest;
    uint32_t* Src;

    int32_t  cY, Len;
    int32_t  rW, rH, xO, yO;


    rW = srcSurf->Width;
    rH = srcSurf->Height;
    xO = yO = 0;

    // Bounds checks
    if (X < 0)
    {
        rW = rW + X;
        if (rW <= 0)
            return;
        xO = -X;
        X = 0;
    }

    if (Y < 0)
    {
        rH = rH + Y;
        if (rH <= 0)
            return;
        yO = -Y;
        Y = 0;
    }

    if (X + rW > destSurf->Width)
    {
        rW = destSurf->Width - X;
        if (rW <= 0)
            return;
    }

    if (Y + rH > destSurf->Height)
    {
        rH = destSurf->Height - Y;
        if (rH <= 0)
            return;
    }


    // Blit row by row
    Dest = destSurf->ptrRows[Y] + X;
    Src = srcSurf->ptrRows[yO] + xO;
    Len = rW << 2;
    for (cY = 0; cY < rH; cY++)
    {
        memcpy(Dest, Src, Len);
        Dest += destSurf->Width;
        Src += srcSurf->Width;
    }

}


// Render a filled box
// blendOp SOLID, ALPHA50, ALPHA
//
void renderBoxFill(const Surface* destSurf, int32_t X, int32_t Y, int32_t Width, int32_t Height, const uint32_t Color, const uint32_t blendOp)
{

    int32_t cX, cY;

    uint32_t *Dest;
    uint32_t *destRow;


    // Normalize
    if (Width < 0)
    {
        Width = abs(Width);
        X -= Width;
    }

    if (Height < 0)
    {
        Height = abs(Height);
        Y -= Height;
    }


    // Bounds check
    if (X < 0)
    {
        Width = Width + X;
        if (Width <= 0)
            return;
        X = 0;
    }

    if (Y < 0)
    {
        Height = Height + Y;
        if (Height <= 0)
            return;
        Y = 0;
    }

    if (X + Width > destSurf->Width)
    {
        Width = destSurf->Width - X;
        if (Width <= 0)
            return;
    }

    if (Y + Height > destSurf->Height)
    {
        Height = destSurf->Height - Y;
        if (Height <= 0)
            return;
    }


    switch (blendOp)
    {
        case BLEND_SOLID:
            destRow = destSurf->ptrRows[Y] + X;
            for (cY = 0; cY < Height; cY++)
            {
                Dest = destRow;

                cX = Width;
                while(cX-- > 0)
                {
                    *Dest = Color;
                    Dest++;
                }

                destRow += destSurf->Width;
            }
            break;

        case BLEND_ALPHA50:
            destRow = destSurf->ptrRows[Y] + X;
            for (cY = 0; cY < Height; cY++)
            {
                Dest = destRow;

                cX = Width;
                while(cX-- > 0)
                {
                    *Dest = renderPixelAlpha50(*Dest, Color);
                    Dest++;
                }

                destRow += destSurf->Width;
            }
            break;

        case BLEND_ALPHA:
            destRow = destSurf->ptrRows[Y] + X;
            for (cY = 0; cY < Height; cY++)
            {
                Dest = destRow;

                cX = Width;
                while(cX-- > 0)
                {
                    *Dest = renderPixelAlpha(*Dest, Color);
                    Dest++;
                }

                destRow += destSurf->Width;
            }
            break;

        default:
            break;
    }

}


// Blit a surface onto another. Only alpha blending, no scaling.
// blendOp ALPHASIMPLE, ALPHA50, ALPHA
//
void renderBlitAlphaFast(const Surface* destSurf, const Surface *srcSurf, int32_t X, int32_t Y, const uint32_t blendOp)
{

    int32_t cX, cY;
    int32_t rW, rH, xO, yO;

    uint32_t*  Dest;
    uint32_t*  Src;


    rW = srcSurf->Width;
    rH = srcSurf->Height;
    xO = yO = 0;

    // Bounds checks
    if (X < 0)
    {
        rW = rW + X;
        if (rW <= 0)
            return;
        xO = -X;
        X = 0;
    }

    if (Y < 0)
    {
        rH = rH + Y;
        if (rH <= 0)
            return;
        yO = -Y;
        Y = 0;
    }

    if (X + rW > destSurf->Width)
    {
        rW = destSurf->Width - X;
        if (rW <= 0)
            return;
    }

    if (Y + rH > destSurf->Height)
    {
        rH = destSurf->Height - Y;
        if (rH <= 0)
            return;
    }


    // Blit
    switch(blendOp)
    {
        // Simple alpha
        case BLEND_ALPHA_SIMPLE:
            Dest = destSurf->ptrRows[Y] + X;
            for (cY = 0; cY < rH; cY++)
            {
                Src = srcSurf->ptrRows[yO + cY] + xO;

                cX = rW;
                while(cX--)
                {
                    *Dest = renderPixelAlphaSimple(*Dest, *Src);

                    Src++;
                    Dest++;
                }
                Dest += destSurf->Width - rW;
            }
            break;

        // Full alpha
        case BLEND_ALPHA:
            Dest = destSurf->ptrRows[Y] + X;
            for (cY = 0; cY < rH; cY++)
            {
                Src = srcSurf->ptrRows[yO + cY] + xO;

                cX = rW;
                while(cX--)
                {
                    *Dest = renderPixelAlpha(*Dest, *Src);

                    Src++;
                    Dest++;
                }
                Dest += destSurf->Width - rW;
            }
            break;

        // 50% blend
        case BLEND_ALPHA50:
            Dest = destSurf->ptrRows[Y] + X;
            for (cY = 0; cY < rH; cY++)
            {
                Src = srcSurf->ptrRows[yO + cY] + xO;

                cX = rW;
                while(cX--)
                {
                    *Dest = renderPixelAlpha50(*Dest, *Src);

                    Src++;
                    Dest++;
                }
                Dest += destSurf->Width - rW;
            }
            break;

        default:
            break;
    }

}


// Blit a surface onto another. Does scaling and alpha blending.
// blendOp SOLID
//
void renderBlit(const Surface* destSurf, const Surface* srcSurf, const int32_t X, const int32_t Y, const int32_t Width, const int32_t Height)
{

    int32_t  cX, cY;
    int32_t   cU, cV, xRatio, yRatio;
    uint32_t* Dest;


    if (Width < 1 || Height < 1)
        return;

    xRatio  = (srcSurf->Width << 16) / Width;
    yRatio  = (srcSurf->Height << 16) / Height;

    cV = 0;

    for(cY = Y; cY < Y + Height; cY++)
    {
        if ((uint32_t)cY < destSurf->Height)
        {
            cU = 0;

            for(cX = X; cX < X + Width; cX++)
            {
                if ((uint32_t)cX < destSurf->Width)
                {
                    Dest = destSurf->ptrRows[cY] + cX;
                    *Dest = renderPixelSolid(*Dest, srcSurf->ptrRows[cV >> 16][cU >> 16]);
                }

                cU += xRatio;
            }
        }

        cV += yRatio;
    }

}
