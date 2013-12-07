#ifndef H_RENDER
#define H_RENDER

#include "surface.h"
#include "font.h"


// Blending operations
enum blendOps
{
    BLEND_SOLID = 0,
    BLEND_ALPHA50 = 1,
    BLEND_ALPHA = 2,
    BLEND_ALPHA_SIMPLE = 3,
};


void renderOutline          (const Surface* destSurf, const Surface* srcSurf, const int32_t rX, const int32_t rY, const uint32_t Color);
void renderLine             (const Surface* destSurf, int32_t X1, int32_t Y1, const int32_t X2, const int32_t Y2, const uint32_t Color);
void renderText             (const Surface* destSurf, const Font* srcFont, int32_t x, const int32_t y, const char* Text, uint32_t Color);
void renderBox              (const Surface* destSurf, int32_t x, int32_t y, int32_t Width, int32_t Height, const uint32_t Color);
void renderBlitFast         (const Surface* destSurf, const Surface* srcSurf, int32_t X, int32_t Y);
void renderBoxFill          (const Surface* destSurf, int32_t X, int32_t Y, int32_t Width, int32_t Height, const uint32_t Color, const uint32_t blendOp);
void renderBlitAlphaFast    (const Surface* destSurf, const Surface *srcSurf, int32_t X, int32_t Y, const uint32_t blendOp);
void renderBlit             (const Surface* destSurf, const Surface* srcSurf, const int32_t X, const int32_t Y, const int32_t Width, const int32_t Height);
void renderMultilineText    (const Surface* Surf, const Font* Fnt, const int32_t x, const int32_t y, const uint32_t Width, const uint32_t Height, char* Text, uint32_t Color);


#endif
