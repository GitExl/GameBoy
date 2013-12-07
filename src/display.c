#include "renderlib.h"
#include "display.h"
#include "z80.h"
#include "mmu.h"
#include "utils.h"


DisplayContext* disp;

inline void DisplaySetMode(DisplayContext* ctx, const uint8_t mode);
inline void DisplaySetLine(DisplayContext* ctx, const uint8_t line);

inline void DisplayRenderSprites(DisplayContext* ctx);
inline void DisplayRenderBackground(DisplayContext* ctx);
inline void DisplayRenderWindow(DisplayContext* ctx);
inline void DisplayRenderSpriteLine(DisplayContext* ctx, const uint8_t tileIndex, const int16_t x, const int16_t y, const uint8_t flags);


// Update an RGBA palette with data from a GMB byte palette
//
void DisplaySetPalette(DisplayContext* ctx, uint32_t* pal, uint8_t value)
{
    for(uint8_t i = 0; i < 4; i++)
    {
        switch((value >> (i * 2)) & 0x03)
        {
            case 0: pal[i] = GRAY(255); break;
            case 1: pal[i] = GRAY(191); break;
            case 2: pal[i] = GRAY(95); break;
            case 3: pal[i] = GRAY(0); break;
            default: break;
        }
    }
}

// Render the background area for the current line
//
inline void DisplayRenderBackground(DisplayContext* ctx)
{
    uint8_t scrollX = mmu->io[IO_BGX];
    uint8_t scrollY = mmu->io[IO_BGY];

    // Map tile coordinates
    uint8_t mapX = (scrollX / 8) % 32;
    uint8_t mapY = ((scrollY + ctx->line) / 8) % 32;

    // x and y locations inside the tile gfx
    uint8_t x = scrollX % 8;
    uint8_t y = ((scrollY + ctx->line) % 8) * 2;

    // Memory offsets
    uint16_t src = (mmu->io[IO_LCDCONTROL] & LCD_BGTILEMAP) ? VRAM_BG2 : VRAM_BG1;
    src += mapY * 32 + mapX;
    uint32_t* dest = ctx->screen->ptrRows[ctx->line];
    uint8_t lineDest = 0;

    // Tile
    uint16_t tileSrc = 0;
    uint8_t tileIndex = 0;
    uint8_t col = 0;

    for (uint8_t i = 0; i < 160; i++)
    {
        // Get location of tile gfx
        tileIndex = mmu->vram[src];
        if (mmu->io[IO_LCDCONTROL] & LCD_BGWTILEGFX)
            tileSrc = VRAM_TILES1 + (tileIndex * 16);
        else
            tileSrc = VRAM_TILES2 + (((int8_t)tileIndex + 128) * 16);

        // Calculate color at x and y index inside tile gfx
        col = 1 << (7 - x);
        col = ((mmu->vram[tileSrc + y]     & col) ? 1 : 0) |
              ((mmu->vram[tileSrc + y + 1] & col) ? 2 : 0);

        *dest++ = ctx->palBG[col];
        ctx->lineData[lineDest++] = col;

        x++;
        if (x == 8)
        {
            x = 0;
            mapX++;

            // Wrap around map coordinates
            if (mapX > 31)
            {
                mapX = 0;
                src -= 31;
            }
            else
                src++;
        }
    }
}

// Render the window area for the current line
//
inline void DisplayRenderWindow(DisplayContext* ctx)
{
    int16_t scrollX = mmu->io[IO_WINDOWX] - 7;
    uint8_t scrollY = mmu->io[IO_WINDOWY];

    if (ctx->line < scrollY)
        return;
    if (scrollX > 166 || scrollY > 143)
        return;

    // Map tile coordinates
    uint8_t mapX = (scrollX / 8) % 32;
    uint8_t mapY = ((ctx->line - scrollY) / 8) % 32;

    // Source tile offset
    uint16_t src = (mmu->io[IO_LCDCONTROL] & LCD_WINDOWTILEMAP) ? VRAM_BG2 : VRAM_BG1;
    src += mapY * 32 + mapX;

    // x and y locations inside the tile gfx
    uint8_t x = scrollX % 8;
    uint8_t y = ((ctx->line - scrollY) % 8) * 2;

    if (scrollX < 0)
        scrollX = 0;

    // Destination pixel
    uint32_t* dest = ctx->screen->ptrRows[ctx->line];
    dest += scrollX;
    uint8_t lineDest = scrollX;

    // Tile
    uint16_t tileSrc = 0;
    uint8_t tileIndex = 0;
    uint8_t col = 0;

    for (uint8_t i = scrollX; i < 160; i++)
    {
        // Get location of tile gfx
        tileIndex = mmu->vram[src];
        if (mmu->io[IO_LCDCONTROL] & LCD_BGWTILEGFX)
            tileSrc = VRAM_TILES1 + (tileIndex * 16);
        else
            tileSrc = VRAM_TILES2 + (((int8_t)tileIndex + 128) * 16);

        // Calculate color at x and y index inside tile gfx
        col = 1 << (7 - x);
        col = ((mmu->vram[tileSrc + y]     & col) ? 1 : 0) |
              ((mmu->vram[tileSrc + y + 1] & col) ? 2 : 0);

        *dest++ = ctx->palBG[col];
        ctx->lineData[lineDest++] = col;

        x++;
        if (x == 8)
        {
            x = 0;
            mapX++;
            src++;
        }
    }
}

// Render all sprites for the current line
//
inline void DisplayRenderSprites(DisplayContext* ctx)
{
    Sprite* renderSprites[10];
    uint8_t renderCount = 0;
    uint8_t spriteHeight = (mmu->io[IO_LCDCONTROL] & LCD_SPRITESIZE) ? 16 : 8;
    Sprite* spriteA;
    Sprite* spriteB;
    uint8_t i;

    // Find sprites that are on the current line
    for (i = 0; i < 40; i++)
    {
        spriteA = &ctx->sprites[i];

        // We render sprites that are visible and on the current line
        if (spriteA->y != 0 && spriteA->y < 160 &&
            ctx->line >= spriteA->y - 16 && ctx->line < spriteA->y - 16 + spriteHeight)
        {
            renderSprites[renderCount] = spriteA;
            renderCount++;

            // Render a maximum of 10 sprites per line
            if (renderCount == 10)
                break;
        }
    }

    // Sort sprites from lowest x coordinate to highest
    // TODO: Broken
    if (renderCount > 100)
    {
        uint8_t sorted = 0;
        while(!sorted)
        {
            sorted = 0;

            // Sort sprites by X coordinate
            for (i = 0; i < renderCount - 1; i++)
            {
                if (renderSprites[i]->x < renderSprites[i + 1]->x)
                {
                    spriteA = renderSprites[i];
                    renderSprites[i] = renderSprites[i + 1];
                    renderSprites[i + 1] = spriteA;

                    sorted = 1;
                }
            }
        }
    }

    // Render each sprite
    for (i = 0; i < renderCount; i++)
    {
        spriteA = renderSprites[i];

        // 8x16
        if (spriteHeight == 16)
        {
            if (spriteA->flags & SPR_FLIPY)
            {
                DisplayRenderSpriteLine(ctx, spriteA->tileIndex & 0xFE, spriteA->x - 8, spriteA->y - 8, spriteA->flags);
                DisplayRenderSpriteLine(ctx, spriteA->tileIndex | 0x01, spriteA->x - 8, spriteA->y - 16, spriteA->flags);
            }
            else
            {
                DisplayRenderSpriteLine(ctx, spriteA->tileIndex & 0xFE, spriteA->x - 8, spriteA->y - 16, spriteA->flags);
                DisplayRenderSpriteLine(ctx, spriteA->tileIndex | 0x01, spriteA->x - 8, spriteA->y - 8, spriteA->flags);
            }
        }
        // 8x8
        else
            DisplayRenderSpriteLine(ctx, spriteA->tileIndex, spriteA->x - 8, spriteA->y - 16, spriteA->flags);
    }
}

// Render a tile at specific coordinates, on the current line
//
inline void DisplayRenderSpriteLine(DisplayContext* ctx, const uint8_t tileIndex, const int16_t x, const int16_t y, const uint8_t flags)
{
    uint8_t col = 0;
    uint8_t cx = 0;
    uint32_t* pal;

    uint16_t src = VRAM_TILES1 + (tileIndex * 16);
    uint32_t* dest = ctx->screen->ptrRows[ctx->line] + x;
    int16_t lineDest = x;
    uint8_t priority = flags & SPR_PRIORITY;

    // Palette
    if (flags & SPR_PAL)
        pal = disp->palOBJ2;
    else
        pal = disp->palOBJ1;

    // Source pixel Y
    if (flags & SPR_FLIPY)
        src += 14 - (ctx->line - y) * 2;
    else
        src += (ctx->line - y) * 2;

    for (cx = 0; cx < 8; cx++)
    {
        if (lineDest >= 0 && lineDest < 160)
        {
            // Source pixel X
            if (flags & SPR_FLIPX)
                col = 1 << cx;
            else
                col = 1 << (7 - cx);

            col = ((mmu->vram[src]     & col) ? 1 : 0) |
                  ((mmu->vram[src + 1] & col) ? 2 : 0);

            // Color 0 is transparent
            if (col)
            {
                // OBJ behind BG color 1-3
                if (priority && ctx->lineData[lineDest] == 0)
                    *dest = pal[col];
                else if (!priority)
                    *dest = pal[col];
            }
        }

        dest++;
        lineDest++;
    }
}

// Initialise a new gameboy display context
//
DisplayContext* DisplayInit()
{
    DisplayContext* ctx = calloc(1, sizeof(DisplayContext));

    // Gameboy screen
    ctx->screen = surfaceCreate(160, 144);

    // We reference sprites directly from OAM
    ctx->sprites = (Sprite*)mmu->oam;

    ctx->mode = LCDMODE_VBLANK;
    ctx->line = 144;
    ctx->VRAMenabled = 1;
    ctx->OAMenabled = 1;
    ctx->clock = 0;

    return ctx;
}

// Destroy a gameboy display context
//
void DisplayDestroy(DisplayContext* ctx)
{
    surfaceDestroy(ctx->screen);
    free(ctx);
}

// Set a new LCD mode and update various status bits accordingly
//
inline void DisplaySetMode(DisplayContext* ctx, const uint8_t mode)
{
    ctx->mode = mode;

    // Update LCD status register with current mode bits
    mmu->io[IO_LCDSTATUS] = (mmu->io[IO_LCDSTATUS] & 0xFC) | ctx->mode;

    // Set VRAM bank status
    if (mmu->io[IO_LCDCONTROL] & LCD_DISPLAY)
    {
        if (ctx->mode == LCDMODE_HBLANK || ctx->mode == LCDMODE_VBLANK || ctx->mode == LCDMODE_OAM)
            ctx->VRAMenabled = 1;
        else
            ctx->VRAMenabled = 0;

        if (ctx->mode == LCDMODE_HBLANK || ctx->mode == LCDMODE_VBLANK)
            ctx->OAMenabled = 1;
        else
            ctx->OAMenabled = 0;
    }
    // When the display is turned off, VRAM is always available
    else
    {
        ctx->VRAMenabled = 1;
        ctx->OAMenabled = 1;
    }
}

// Cycle through a display step
// OAM     : 20 cycles
// Transfer: 43 cycles
// HBlank  : 51 cycles
// Line    : 114 cycles
// VBlank  : 10 lines
//
void DisplayStep(DisplayContext* ctx)
{
    switch (ctx->mode)
    {
        // OAM read mode, scanline active
        case LCDMODE_OAM:
            if (ctx->clock >= 20)
            {
                ctx->clock -= 20;
                DisplaySetMode(ctx, LCDMODE_TRANSFER);
            }
            break;

        // VRAM read mode, scanline active
        case LCDMODE_TRANSFER:
            if (ctx->clock >= 43)
            {
                ctx->clock -= 43;
                DisplaySetMode(ctx, LCDMODE_HBLANK);

                // LCDSTAT HBlank interrupt
                if (mmu->io[IO_LCDSTATUS] & LCDST_INT_HBLANK)
                    mmu->io[IO_INTFLAGS] |= Z80INT_LCDSTAT;

                // Render the current line
                if (mmu->io[IO_LCDCONTROL] & LCD_DISPLAY)
                {
                    if (mmu->io[IO_LCDCONTROL] & LCD_DISPLAYBG)
                        DisplayRenderBackground(ctx);
                    if (mmu->io[IO_LCDCONTROL] & LCD_DISPLAYWINDOW)
                        DisplayRenderWindow(ctx);
                    if (mmu->io[IO_LCDCONTROL] & LCD_DISPLAYSPRITES)
                        DisplayRenderSprites(ctx);
                }
            }
            break;

        // Horizontal blank period
        case LCDMODE_HBLANK:
            if (ctx->clock >= 51)
            {
                // Last visible line was finished, start VBlank period
                if (ctx->line == 143)
                {
                    DisplaySetMode(ctx, LCDMODE_VBLANK);

                    // Global VBlank interrupt
                    mmu->io[IO_INTFLAGS] |= Z80INT_VBLANK;

                    // LCDSTAT VBlank interrupt
                    if (mmu->io[IO_LCDSTATUS] & LCDST_INT_VBLANK)
                        mmu->io[IO_INTFLAGS] |= Z80INT_LCDSTAT;
                }

                // Advance to next line
                else
                {
                    DisplaySetMode(ctx, LCDMODE_OAM);

                    // LCDSTAT OAM interrupt
                    if (mmu->io[IO_LCDSTATUS] & LCDST_INT_OAM)
                        mmu->io[IO_INTFLAGS] |= Z80INT_LCDSTAT;
                }

                ctx->clock -= 51;
                DisplaySetLine(ctx, ctx->line + 1);
            }
            break;

        // Vertical blank period, 10 lines
        case LCDMODE_VBLANK:
            if (ctx->clock >= 114)
            {
                ctx->clock -= 114;
                DisplaySetLine(ctx, ctx->line + 1);

                // Restart scan
                if (ctx->line == 154)
                {
                    DisplaySetMode(ctx, LCDMODE_OAM);
                    DisplaySetLine(ctx, 0);

                    // LCDSTAT OAM interrupt
                    if (mmu->io[IO_LCDSTATUS] & LCDST_INT_OAM)
                        mmu->io[IO_INTFLAGS] |= Z80INT_LCDSTAT;
                }
            }
            break;

        default:
            break;
    }
}

// Set the current display line
//
inline void DisplaySetLine(DisplayContext* ctx, const uint8_t line)
{
    ctx->line = line;

    // LCDSTAT LCDYC interrupt
    if (mmu->io[IO_LCDSTATUS] & LCDST_INT_CDC && ctx->line == mmu->io[IO_LCDYCDC])
        mmu->io[IO_INTFLAGS] |= Z80INT_LCDSTAT;

    // LCDSTAT LCDYC flag
    if (ctx->line == mmu->io[IO_LCDYCDC])
        mmu->io[IO_LCDSTATUS] |= LCDST_CDC;
    else
        mmu->io[IO_LCDSTATUS] &= ~LCDST_CDC;
}

// DisplaySetLine (inlined) cannot be called from other files, hence this wrapper
//
void DisplaySetLineIndirect(DisplayContext* ctx, const uint8_t line)
{
    DisplaySetLine(ctx, line);
}
