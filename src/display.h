#ifndef H_DISPLAY
#define H_DISPLAY

#include "surface.h"


// VRAM memory areas at 0x8000
enum
{
    VRAM_TILES1 = 0x0000,
    VRAM_TILES2 = 0x0800,
    VRAM_BG1    = 0x1800,
    VRAM_BG2    = 0x1C00,
};

// LCD control flags
enum
{
    LCD_DISPLAYBG      = 0x01,
    LCD_DISPLAYSPRITES = 0x02,
    LCD_SPRITESIZE     = 0x04,
    LCD_BGTILEMAP      = 0x08,
    LCD_BGWTILEGFX     = 0x10,
    LCD_DISPLAYWINDOW  = 0x20,
    LCD_WINDOWTILEMAP  = 0x40,
    LCD_DISPLAY        = 0x80,
};

// LCD status flags
enum
{
    LCDST_MODE_MASK  = 0x03,
    LCDST_CDC        = 0x04,
    LCDST_INT_HBLANK = 0x08,
    LCDST_INT_VBLANK = 0x10,
    LCDST_INT_OAM    = 0x20,
    LCDST_INT_CDC    = 0x40,
};

// LCD status modes
enum
{
    LCDMODE_HBLANK   = 0,
    LCDMODE_VBLANK   = 1,
    LCDMODE_OAM      = 2,
    LCDMODE_TRANSFER = 3,
};

// OAM sprite flags
enum
{
    SPR_PRIORITY   = 0x80,
    SPR_FLIPY      = 0x40,
    SPR_FLIPX      = 0x20,
    SPR_PAL        = 0x10,
    SPR_BANK       = 0x08,
    SPR_CGBPALMASK = 0x07,
};


// OAM sprite
typedef struct
{
    uint8_t y;
    uint8_t x;
    uint8_t tileIndex;
    uint8_t flags;
} Sprite;

// Gameboy display context
typedef struct
{
    Surface* screen;
    Sprite* sprites;

    uint8_t  mode;
    uint32_t clock;
    uint8_t  line;

    uint8_t VRAMenabled;
    uint8_t OAMenabled;

    uint32_t palBG[4];
    uint32_t palOBJ1[4];
    uint32_t palOBJ2[4];

    uint8_t lineData[160];
} DisplayContext;


extern DisplayContext* disp;


DisplayContext* DisplayInit();
void DisplayDestroy(DisplayContext* ctx);
void DisplayStep(DisplayContext* ctx);
void DisplaySetPalette(DisplayContext* ctx, uint32_t* pal, uint8_t value);
void DisplaySetLineIndirect(DisplayContext* ctx, const uint8_t line);

#endif
