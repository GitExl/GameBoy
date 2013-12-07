#include "renderlib.h"
#include "mbc.h"
#include "mmu.h"
#include "z80.h"


MBCContext* mbc;

inline void MBCCopyROMBank(MBCContext* ctx);
inline void MBCCopyRAMBank(MBCContext* ctx);


MBCContext* MBCInit()
{
    MBCContext* ctx = calloc(1, sizeof(MBCContext));

    ctx->switchMode = MBC_SWITCHMODE_ROM;

    return ctx;
}

void MBCDestroy(MBCContext* ctx)
{
    free(ctx->ram);
    free(ctx);
}

uint8_t MBCReadRAM(MBCContext* ctx, uint16_t address)
{
    if (!ctx->ramEnabled)
        return 0xFF;

    uint16_t maxRam = ctx->ramSize >= 0x2000 ? 0x2000 : ctx->ramSize;
    if (address - 0xA000 > maxRam)
        return 0xFF;

    return mmu->extram[address - 0xA000];
}

void MBCWriteRAM(MBCContext* ctx, uint16_t address, uint8_t data)
{
    if (!ctx->ramEnabled)
        return;

    uint16_t maxRam = ctx->ramSize >= 0x2000 ? 0x2000 : ctx->ramSize;
    if (address - 0xA000 > maxRam)
        return;

    mmu->extram[address - 0xA000] = data;
}

void MBCWrite(MBCContext* ctx, uint16_t address, uint8_t data)
{
    switch (address & 0xF000)
    {
        // RAM enable switch
        case 0x0000:
        case 0x1000:
            if (data & 0x0A)
                ctx->ramEnabled = 1;
            else
                ctx->ramEnabled = 0;
            break;

        // ROM bank switch
        case 0x2000:
        case 0x3000:
            ctx->romBank = (ctx->romBank & 0x60) | (data & 0x1F);
            MBCCopyROMBank(ctx);
            break;

        // ROM\RAM bank switch
        case 0x4000:
        case 0x5000:
            if(ctx->switchMode == MBC_SWITCHMODE_RAM)
            {
                printf("$%02X\n", data & 0x03);
                ctx->ramBank = data & 0x03;
                MBCCopyRAMBank(ctx);
            }
            else if (ctx->switchMode == MBC_SWITCHMODE_ROM)
            {
                ctx->romBank &= 0x1F;
                ctx->romBank |= (data & 0x03) << 5;
                MBCCopyROMBank(ctx);
            }
            break;

        // Switch mode
        case 0x6000:
        case 0x7000:
            if (data & 0x01)
            {
                ctx->switchMode = MBC_SWITCHMODE_RAM;
                ctx->romBank &= 0x1F;
            }
            else
            {
                ctx->switchMode = MBC_SWITCHMODE_ROM;
                ctx->ramBank = 0;
            }
            break;

        default:
            break;
    }
}

void MBCCopyROMBank(MBCContext* ctx)
{
    if (ctx->romBank == 0x00 || ctx->romBank == 0x20 || ctx->romBank == 0x40 || ctx->romBank == 0x60)
        ctx->romBank++;

    if (ctx->romBank > ctx->romSize / 0x4000)
        printf("MBC: Tried to set invalid ROM bank %i at $%04X\n", ctx->romBank, cpu->PC);
    else
        memcpy(mmu->rom1, mmu->cartridge + (ctx->romBank * 0x4000), 0x4000);
}

void MBCCopyRAMBank(MBCContext* ctx)
{
    if (ctx->ramSize <= 0x2000)
        memcpy(mmu->extram, ctx->ram, ctx->ramSize);
    else
    {
        if (ctx->ramBank >= ctx->ramSize / 0x2000)
            printf("MBC: Tried to set invalid RAM bank %i at $%04X\n", ctx->ramBank, cpu->PC);
        else
            memcpy(mmu->extram, ctx->ram + (ctx->ramBank * 0x2000), 0x2000);
    }

}
