#ifndef H_MMU
#define H_MMU

// IO memory map
enum
{
    // LCD
    IO_LCDCONTROL = 0x0040,
    IO_LCDSTATUS  = 0x0041,
    IO_LCDY       = 0x0044,
    IO_LCDYCDC    = 0x0045,

    // Tilemap positions
    IO_BGY     = 0x0042,
    IO_BGX     = 0x0043,
    IO_WINDOWY = 0x004A,
    IO_WINDOWX = 0x004B,

    // Palettes
    IO_BGPAL   = 0x0047,
    IO_OBJPAL1 = 0x0048,
    IO_OBJPAL2 = 0x0049,

    // Serial transfer
    IO_SERIALDATA = 0x0001,
    IO_SERIALCTRL = 0x0002,

    // Interrupts
    IO_INTFLAGS = 0x000F,
    IO_INTENABLE = 0x00FF,

    // BIOS switch
    IO_BIOS = 0x0050,

    // Timer
    IO_TMR_DIV  = 0x0004,
    IO_TMR_TIMA = 0x0005,
    IO_TMR_TMA  = 0x0006,
    IO_TMR_TAC  = 0x0007,

    // Sound
    // Channel 1 - tone and sweep
    IO_SND_C1SWEEP  = 0x0010,
    IO_SND_C1SLWP   = 0x0011,
    IO_SND_C1ENV    = 0x0012,
    IO_SND_C1FREQLO = 0x0013,
    IO_SND_C1FREQHI = 0x0014,

    // Channel 2 - tone
    IO_SND_C2SLWP   = 0x0016,
    IO_SND_C2ENV    = 0x0017,
    IO_SND_C2FREQLO = 0x0018,
    IO_SND_C2FREQHI = 0x0019,

    // Channel 3 - wave output
    IO_SND_C3STATE    = 0x001A,
    IO_SND_C3LENGTH   = 0x001B,
    IO_SND_C3VOLUME   = 0x001C,
    IO_SND_C3FREQLO   = 0x001D,
    IO_SND_C3FREQHI   = 0x001E,
    IO_SND_C3WAVEDATA = 0x0030,

    // Channel 4 - noise
    IO_SND_C4LEN   = 0x0020,
    IO_SND_C4ENV   = 0x0021,
    IO_SND_C4POLYC = 0x0022,
    IO_SND_C4CCONS = 0x0023,

    // Sound control
    IO_SND_CHANNELC = 0x0024,
    IO_SND_OUTPUTC  = 0x0025,
    IO_SND_STATE    = 0x0026,

    // Miscellaneous
    IO_JOYPAD = 0x0000,
    IO_OAMDMA = 0x0046,

    // GameBoy color only, not yet implemented or named
    IO_GC_BGPALINDEX  = 0x0068,
    IO_GC_BGPALDATA   = 0x0069,
    IO_GC_SPRPALINDEX = 0x006A,
    IO_GC_SPRPALDATA  = 0x006A,

    IO_GC_VRAMBANK = 0x004F,

    IO_GC_HDMA1 = 0x0051,
    IO_GC_HDMA2 = 0x0052,
    IO_GC_HDMA3 = 0x0053,
    IO_GC_HDMA4 = 0x0054,
    IO_GC_HDMA5 = 0x0055,

    IO_GC_KEY1    = 0x004D,
    IO_GC_IR      = 0x0056,
    IO_GC_RAMBANK = 0x004D,
};

// ROM type settings
enum
{
    MMU_ROM_MBC = 0,
    MMU_ROM_RAM = 1,
    MMU_ROM_BAT = 2,
    MMU_ROM_MMM = 3,
    MMU_ROM_TIMR = 4,
    MMU_ROM_RMBL = 5,
};

// Memory access mode
enum
{
    ACCESS_VRAMHRAM = 0,
    ACCESS_ALL      = 1,
};


// Gameboy memory manager context
typedef struct
{
    uint8_t* cartridge;
    char*    cartName;

    uint8_t* memory;

    uint8_t* rom0;
    uint8_t* rom1;

    uint8_t* vram;
    uint8_t* extram;
    uint8_t* ram0;
    uint8_t* ram1;
    uint8_t* echoram;

    uint8_t* oam;
    uint8_t* unusable;
    uint8_t* io;
    uint8_t* hram;

    uint8_t keypadColumn;
    uint8_t keyRows[2];

    uint8_t accessMode;
    uint8_t inBIOS;
} MMUContext;


extern MMUContext* mmu;


MMUContext* MMUInit();
void MMUDestroy(MMUContext* ctx);
uint8_t MMULoadCartridge(MMUContext* ctx, char* fileName);
uint8_t MMUReadByte(MMUContext* ctx, const uint16_t address);
uint16_t MMUReadWord(MMUContext* ctx, const uint16_t address);
void MMUWriteByte(MMUContext* ctx, const uint16_t address, const uint8_t data);
void MMUWriteWord(MMUContext* ctx, const uint16_t address, const uint16_t data);
void MMUSetBIOS(MMUContext* ctx, uint8_t enable);

#endif
