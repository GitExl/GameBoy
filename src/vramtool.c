#include "renderlib.h"
#include "render.h"
#include "font.h"
#include "utils.h"
#include "display.h"
#include "mmu.h"
#include "vramtool.h"


extern Font* defaultFont;
extern uint8_t mode;
extern uint8_t modeRender;

VRAMTool* vrt = NULL;


VRAMTool* VRTInit()
{
    VRAMTool* ctx = calloc(1, sizeof(VRAMTool));

    return ctx;
}

void VRTDestroy(VRAMTool* ctx)
{
    free(ctx);
}

void VRTUpdate(VRAMTool* ctx, uint8_t* keyStates)
{
}

void VRTRender(VRAMTool* ctx, Surface* screen)
{
    uint16_t tileIndex = 0;
    uint32_t tileX = 16;
    uint32_t tileY = 32;
    uint16_t tileSrc = VRAM_TILES1;
    uint8_t x = 0;
    uint8_t y = 0;
    uint8_t tileCol = 0;

    for (tileIndex = 0; tileIndex < 384; tileIndex++)
    {
        for (y = 0; y < 8; y++)
        {
            for (x = 0; x < 8; x++)
            {
                tileCol = 1 << (7 - x);
                tileCol = ((mmu->vram[tileSrc]     & tileCol) ? 1 : 0) |
                          ((mmu->vram[tileSrc + 1] & tileCol) ? 2 : 0);

                screen->ptrRows[tileY + y][tileX + x] = disp->palBG[tileCol];
            }
            tileSrc += 2;
        }

        tileX += 9;
        if (tileX >= 171)
        {
            tileX = 16;
            tileY += 9;
        }
    }

    modeRender = 0;
}

void VRTEventInput(VRAMTool* ctx, SDL_Event* event)
{
    uint16_t keyCode = event->key.keysym.sym;
    switch (event->type)
    {
        case SDL_KEYDOWN:
            modeRender = 1;
            break;
        default:
            break;
    }
}

void VRTEnable(VRAMTool* ctx)
{
    modeRender = 1;
}
