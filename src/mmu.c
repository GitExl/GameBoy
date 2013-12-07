#include "renderlib.h"
#include "z80.h"
#include "mmu.h"
#include "display.h"
#include "mbc.h"
#include "sound.h"


MMUContext*     mmu;

inline uint8_t MMUReadIO(MMUContext* ctx, const uint16_t address);
inline uint8_t MMUWriteIO(MMUContext* ctx, const uint16_t address, const uint8_t value);


// Gameboy BIOS
uint8_t BIOS_GMB[256];

// ROM type list
static uint8_t MMUROMTypes[0x30][6] =
{
      // TODO: Convert into switch or elaborate with constants\flags
      //MBC, RAM, BAT, MMM, TIMR, RMBL
      // 0x00
      {0,   0,   0,   0,   0,    0},
      {1,   0,   0,   0,   0,    0},
      {1,   1,   0,   0,   0,    0},
      {1,   1,   1,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {2,   0,   0,   0,   0,    0},
      {2,   0,   1,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   1,   0,   0,   0,    0},
      {0,   1,   1,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   1,   0,    0},
      {0,   1,   0,   1,   0,    0},
      {0,   1,   1,   1,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {3,   0,   1,   0,   1,    0},

      // 0x10
      {3,   1,   1,   0,   1,    0},
      {3,   0,   0,   0,   0,    0},
      {3,   1,   0,   0,   0,    0},
      {3,   1,   1,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {4,   0,   0,   0,   0,    0},
      {4,   1,   0,   0,   0,    0},
      {4,   1,   1,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {5,   0,   0,   0,   0,    0},
      {5,   1,   0,   0,   0,    0},
      {5,   1,   1,   0,   0,    0},
      {6,   0,   0,   0,   0,    1},
      {6,   1,   0,   0,   0,    1},
      {6,   1,   1,   0,   0,    1},

      // Camera
      {0,   0,   0,   0,   0,    0},

      // 0x20
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {7,   1,   1,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
      {0,   0,   0,   0,   0,    0},
};


// Enable or disable the internal BIOS code
//
void MMUSetBIOS(MMUContext* ctx, uint8_t enable)
{
    if (enable)
    {
        ctx->inBIOS = 1;
        memcpy(ctx->memory, &BIOS_GMB, 0x100);
    }
    else
    {
        ctx->inBIOS = 0;
        memcpy(ctx->memory, ctx->cartridge, 0x100);
    }
}


// Initialise a new MMU context
//
MMUContext* MMUInit()
{
    MMUContext* ctx;

    ctx = calloc(1, sizeof(MMUContext));

    ctx->cartridge = NULL;
    ctx->cartName = NULL;

    // Allocate one 64k slab of memory and index the other areas into it
    ctx->memory = malloc(0x10000);
    ctx->rom0 = ctx->memory + 0x0000;
    ctx->rom1 = ctx->memory + 0x4000;

    ctx->vram = ctx->memory + 0x8000;
    ctx->extram = ctx->memory + 0xA000;
    ctx->ram0 = ctx->memory + 0xC000;
    ctx->ram1 = ctx->memory + 0xD000;
    ctx->echoram = ctx->memory + 0xE000;

    ctx->oam = ctx->memory + 0xFE00;
    ctx->unusable = ctx->memory + 0xFEA0;
    ctx->io = ctx->memory + 0xFF00;
    ctx->hram = ctx->memory + 0xFF80;

    // Load BIOS ROM
    FILE* f = fopen("dmg_rom.bin", "rb");
    fread(&BIOS_GMB, 1, 256, f);
    fclose(f);

    ctx->accessMode = ACCESS_ALL;
    ctx->inBIOS = 0;

    return ctx;
}

// Destroy an MMU context
//
void MMUDestroy(MMUContext* ctx)
{
    if (ctx->cartridge)
        free(ctx->cartridge);

    free(ctx->memory);
    free(ctx);
}

// Load a cartridge file
//
uint8_t MMULoadCartridge(MMUContext* ctx, char* fileName)
{
    FILE* fp = fopen(fileName, "rb");

    // Get filesize
    fseek(fp, 0, SEEK_END);
    size_t fileLen = ftell(fp);

    // Do not read files bigger than 8Mbyte, the maximum cart size with any MBC
    if (fileLen <= 0x800000)
    {
        if (ctx->cartridge)
            free(ctx->cartridge);

        ctx->cartridge = malloc(fileLen);
        fseek(fp, 0, SEEK_SET);
        fread(ctx->cartridge, fileLen, 1, fp);
        fclose(fp);

        // Read cart info
        ctx->cartName = (char*)(ctx->cartridge + 0x134);

        // ROM size
        mbc->romSize = 0x8000 << *(ctx->cartridge + 0x148);

        // ROM type information
        uint8_t romtype = *(ctx->cartridge + 0x147);
        if (romtype < 31)
        {
            mbc->type = MMUROMTypes[romtype][MMU_ROM_MBC];
            mbc->battery = MMUROMTypes[romtype][MMU_ROM_BAT];
            mbc->timer = MMUROMTypes[romtype][MMU_ROM_TIMR];

            // RAM size
            switch (*(ctx->cartridge + 0x149))
            {
                case 0x01:
                    mbc->ramSize = 0x800;
                    break;
                case 0x02:
                    mbc->ramSize = 0x2000;
                    break;
                case 0x03:
                    mbc->ramSize = 0x8000;
                    break;
                case 0x04:
                    mbc->ramSize = 0x20000;
                    break;
                default:
                    mbc->ramSize = 0;
                    break;
            }

            // Copy initial ROM bank data
            memcpy(ctx->rom0, ctx->cartridge, 0x4000);
            memcpy(ctx->rom1, ctx->cartridge + 0x4000, 0x4000);

            // Allocate RAM
            if (mbc->ram)
                free(mbc->ram);
            mbc->ram = malloc(mbc->ramSize);

            // Output cart info
            printf("MMU: Loaded %.*s\n", 16, ctx->cartName);
            printf("MBC: ROM size: %i Kbyte\n", mbc->romSize / 1024);
            printf("MBC: RAM size: %i Kbyte\n", mbc->ramSize / 1024);
            printf("MBC: Battery: %s\n", mbc->battery ? "Yes" : "No");
            printf("MBC: Timer: %s\n", mbc->timer ? "Yes" : "No");
            printf("MBC: Type: %i\n", mbc->type);
        }
        else
        {
            printf("Invalid ROM type %i.\n", romtype);
            return 1;
        }
    }
    else
    {
        printf("MMU: %s is too big to be a GameBoy cartridge.\n", fileName);
        return 1;
    }

    return 0;
}

// Memory reads
//
uint8_t MMUReadByte(MMUContext* ctx, const uint16_t address)
{
    if (ctx->accessMode == ACCESS_ALL)
    {
        switch (address & 0xF000)
        {
            // ROM bank 0
            case 0x0000:
            case 0x1000:
            case 0x2000:
            case 0x3000:
                return ctx->rom0[address];
                break;

            // ROM bank 1
            case 0x4000:
            case 0x5000:
            case 0x6000:
            case 0x7000:
                return ctx->rom1[address - 0x4000];
                break;

            // VRAM
            case 0x8000:
            case 0x9000:
                if (disp->VRAMenabled)
                    return ctx->vram[address - 0x8000];
                else
                    return 0xFF;
                break;

            // External RAM
            case 0xA000:
            case 0xB000:
                return MBCReadRAM(mbc, address);
                break;

            // RAM bank 0
            case 0xC000:
                return ctx->ram0[address - 0xC000];
                break;

            // RAM bank 1
            case 0xD000:
                return ctx->ram1[address - 0xD000];
                break;

            // RAM bank 0 echo
            case 0xE000:
                return ctx->echoram[address - 0xE000];
                break;

            // Other
            case 0xF000:
                // OAM
                if (address >= 0xFE00 && address <= 0xFE9F)
                {
                    if (disp->OAMenabled)
                        return ctx->oam[address - 0xFE00];
                    else
                        return 0xFF;
                }

                // IO
                else if (address >= 0xFF00 && address <= 0xFF7F || address == 0xFFFF)
                    return MMUReadIO(ctx, address - 0xFF00);

                // High RAM
                else if (address >= 0xFF80 && address <= 0xFFFE)
                    return ctx->hram[address - 0xFF80];

                // Invalid
                else
                    return 0xFF;
                break;

            default:
                return 0xFF;
                break;
        }
    }

    // Only VRAM and HRAM is accessible
    else if (ctx->accessMode == ACCESS_VRAMHRAM)
    {
        if (address >= 0x8000 && address <= 0x9FFF && disp->VRAMenabled)
            return ctx->vram[address - 0x8000];
        else if (address >= 0xFF80 && address <= 0xFFFE)
            return ctx->hram[address - 0xFF80];
        else
            return 0xFF;
    }

    return 0xFF;
}

inline uint16_t MMUReadWord(MMUContext* ctx, const uint16_t address)
{
    return MMUReadByte(ctx, address) + (MMUReadByte(ctx, address + 1) << 8);
}

// Memory writes
//
void MMUWriteByte(MMUContext* ctx, const uint16_t address, const uint8_t data)
{
    if (ctx->accessMode == ACCESS_ALL)
    {
        // ROM area, map to MBC
        if (address <= 0x7FFF)
            MBCWrite(mbc, address, data);
        else
        {
            switch (address & 0xF000)
            {
                // VRAM
                case 0x8000:
                case 0x9000:
                    if (disp->VRAMenabled)
                        ctx->vram[address - 0x8000] = data;
                    break;

                // External RAM, map to MBC
                case 0xA000:
                case 0xB000:
                    MBCWriteRAM(mbc, address, data);
                    break;

                // RAM bank 0
                case 0xC000:
                    ctx->ram0[address - 0xC000] = data;
                    break;

                // RAM bank 1
                case 0xD000:
                    ctx->ram1[address - 0xD000] = data;
                    break;

                // RAM bank 0 echo
                case 0xE000:
                    ctx->echoram[address - 0xE000] = data;
                    break;

                // Other
                case 0xF000:
                    // OAM
                    if (disp->OAMenabled && address >= 0xFE00 && address <= 0xFE9F)
                        ctx->oam[address - 0xFE00] = data;

                    // IO
                    else if (address >= 0xFF00 && address <= 0xFF7F || address == 0xFFFF)
                        MMUWriteIO(ctx, address - 0xFF00, data);

                    // High RAM
                    else if (address >= 0xFF80 && address <= 0xFFFE)
                        ctx->hram[address - 0xFF80] = data;

                    else if (address >= 0xFEA0 && address < 0xFEFF)
                        printf("MMU: Invalid write to area $%04X\n", address);

                    break;

                default:
                    printf("Unhandled write to 0x%X.\n", address);
                    break;
            }
        }
    }

    // Only VRAM is accessible
    else if (ctx->accessMode == ACCESS_VRAMHRAM)
    {
        if (address >= 0x8000 && address <= 0x9FFF && disp->VRAMenabled)
            ctx->vram[address - 0x8000] = data;
        else if (address >= 0xFF80 && address <= 0xFFFE)
            ctx->hram[address -0xFF80] = data;
    }
}

inline void MMUWriteWord(MMUContext* ctx, const uint16_t address, const uint16_t data)
{
    MMUWriteByte(ctx, address, (uint8_t)data);
    MMUWriteByte(ctx, address + 1, (uint8_t)(data >> 8));
}

// Read from IO area
//
inline uint8_t MMUReadIO(MMUContext* ctx, const uint16_t address)
{
    switch(address)
    {
        // Joypad output
        case IO_JOYPAD:
            if (mmu->keypadColumn)
                return mmu->keyRows[mmu->keypadColumn - 1];
            else
                return 0x00;
            break;

        // LCD line
        case IO_LCDY:
            return disp->line;

        // Write only
        case IO_SND_C1FREQLO:
        case IO_SND_C2FREQLO:
        case IO_SND_C3FREQLO:
        case IO_OAMDMA:
            return 0xFF;

        default:
            return ctx->io[address];
    }
}

// Write to IO area
//
uint8_t MMUWriteIO(MMUContext* ctx, const uint16_t address, const uint8_t value)
{
    switch(address)
    {
        // Joypad output select
        case IO_JOYPAD:
            mmu->keypadColumn = 0;
            if (value & 0x10)
                mmu->keypadColumn = 1;
            else if (value & 0x20)
                mmu->keypadColumn = 2;
            break;

        // Sound channel 1
        // TODO: Verify
        // TODO: What is read\write, what is write only and what is read only
        case IO_SND_C1ENV:
            snd->channel1.volume = (value & 0xF) << 4;
            snd->channel1.volumeSweep = value & 0x7;
            snd->channel1.volumeSweepDir = (value & 0x8) ? 1 : 0;
            printf("C1 volume                : %i\n", snd->channel1.volume);
            printf("C1 volume sweep          : %i\n", snd->channel1.volumeSweep);
            printf("C1 volume sweep direction: %i\n", snd->channel1.volumeSweepDir);
            break;
        case IO_SND_C1FREQHI:
            snd->channel1.initial = (value & 0x80) ? 1 : 0;
            snd->channel1.stop = (value & 0x40) ? 1 : 0;
            snd->channel1.frequency &= 0xFF;
            snd->channel1.frequency |= (value & 0x7) << 8;
            printf("C1 initial     : %i\n", snd->channel1.initial);
            printf("C1 stop        : %i\n", snd->channel1.stop);
            printf("C1 frequency hi: %i\n", snd->channel1.frequency & 0xFF);
            break;
        case IO_SND_C1FREQLO:
            snd->channel1.frequency &= ~0xFF;
            snd->channel1.frequency |= value;
            printf("C1 frequency lo: %i\n", snd->channel1.frequency & ~0xFF);
            break;
        case IO_SND_C1SWEEP:
            snd->channel1.sweepTime = (value & 0x70) << 4;
            snd->channel1.sweepDirection = (value & 0x8) << 3;
            snd->channel1.sweepShift = (value & 0x7);
            printf("C1 sweeptime  : %i\n", snd->channel1.sweepTime);
            printf("C1 sweep dir  : %i\n", snd->channel1.sweepDirection);
            printf("C1 sweep shift: %i\n", snd->channel1.sweepShift);
            break;
        case IO_SND_C1SLWP:
            snd->channel1.waveDuty = (value & 0xC0) << 6;
            snd->channel1.waveLength = (64 - (value & 0x3F)) * (1 / 256);
            printf("C1 wave duty: %i\n", snd->channel1.initial);
            printf("C1 wave len : %i\n", snd->channel1.stop);
            break;

        // LCD status
        case IO_LCDSTATUS:
            ctx->io[address] = value & 0xF8;
            break;

        // LCD Y
        case IO_LCDY:
            DisplaySetLineIndirect(disp, 0);
            break;

        // Timer reset
        case IO_TMR_DIV:
            ctx->io[IO_TMR_DIV] = 0;

        // Palettes
        case IO_BGPAL:
            if ((mmu->io[IO_LCDCONTROL] & LCD_DISPLAY) && disp->mode == 3)
                break;
            DisplaySetPalette(disp, disp->palBG, value);
            ctx->io[address] = value;
            break;
        case IO_OBJPAL1:
            if ((mmu->io[IO_LCDCONTROL] & LCD_DISPLAY) && disp->mode == 3)
                break;
            DisplaySetPalette(disp, disp->palOBJ1, value);
            ctx->io[address] = value;
            break;
        case IO_OBJPAL2:
            if ((mmu->io[IO_LCDCONTROL] & LCD_DISPLAY) && disp->mode == 3)
                break;
            DisplaySetPalette(disp, disp->palOBJ2, value);
            ctx->io[address] = value;
            break;

        // DMA transfer from 0xXX00 to OAM memory
        case IO_OAMDMA:
            if (value < 0xF2 && disp->OAMenabled)
            {
                // Disable memory access for 168 - 43 cycles
                ctx->accessMode = ACCESS_VRAMHRAM;
                cpu->dmaTimer = 125;

                memcpy(ctx->oam, ctx->memory + (value * 0x100), 0x100);
            }
            break;

        // Leave BIOS
        case IO_BIOS:
            if (value == 1)
                MMUSetBIOS(ctx, 0);
            break;

        default:
            ctx->io[address] = value;
            break;
    }
}
