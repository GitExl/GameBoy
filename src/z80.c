#include "renderlib.h"
#include "z80.h"
#include "z80opcodes.h"
#include "display.h"
#include "mmu.h"
#include "sound.h"
#include "debugger.h"


Z80Context* cpu;

inline void Z80TestInterrupt(Z80Context* ctx, uint8_t intFlag, uint16_t address);
inline void Z80TimerStep(Z80Context* ctx);


// Initialise a new Z80 cpu context
//
Z80Context* Z80Init()
{
    Z80Context* z80 = calloc(1, sizeof(Z80Context));
    return z80;
}

// Destroy a Z80 cpu context
//
void Z80Destroy(Z80Context* ctx)
{
    free(ctx);
}

// Execute a frame's worth of cycles
//
uint8_t Z80Exec(Z80Context* ctx, uint32_t cycles)
{
    if (ctx->stopped)
        return 0;

    uint32_t cycleEnd = ctx->cycleCount + cycles;
    uint8_t op = 0;

    while (ctx->cycleCount < cycleEnd)
    {
        // Execute next opcode
        if (ctx->halted)
            ctx->cycles = 1;
        else
        {
            op = MMUReadByte(mmu, ctx->PC);

            // Gameboy HALT bug
            // TODO: Does not work like real GMB, test 05 in cpu_instrs.gb
            if (ctx->bugHALT)
                ctx->bugHALT = 0;
            else
                ctx->PC++;

            ctx->cycles = Z80OpcodeList[op].cycles;
            Z80OpcodeList[op].op(ctx);
        }
        ctx->cycleCount += ctx->cycles;

        // Display step
        disp->clock += ctx->cycles;
        if (disp->clock >= 20)
            DisplayStep(disp);

        // Sound step
        snd->clock += ctx->cycles;
        if (snd->clock >= snd->clockRate)
            SoundUpdate(snd);

        // Timer
        Z80TimerStep(ctx);

        // DMA timer
        if (ctx->dmaTimer)
        {
            if (ctx->dmaTimer - ctx->cycles <= 0)
            {
                mmu->accessMode = ACCESS_ALL;
                ctx->dmaTimer = 0;
            }
            else
                ctx->dmaTimer -= ctx->cycles;
        }

        // Handle interrupts
        if (ctx->interruptStatus == INTSTATE_ENABLED)
        {
            Z80TestInterrupt(ctx, Z80INT_VBLANK, 0x40);
            Z80TestInterrupt(ctx, Z80INT_LCDSTAT, 0x48);
            Z80TestInterrupt(ctx, Z80INT_TIMER, 0x50);
            Z80TestInterrupt(ctx, Z80INT_SERIAL, 0x58);
            Z80TestInterrupt(ctx, Z80INT_JOYPAD, 0x60);
        }
        if (ctx->interruptStatus == INTSTATE_ENABLENEXT)
            ctx->interruptStatus = INTSTATE_ENABLED;
        if (ctx->interruptStatus == INTSTATE_DISABLENEXT)
            ctx->interruptStatus = INTSTATE_DISABLED;

        // Breakpoints
        if (dbg->breakpointEnabled && ctx->PC == dbg->breakpoint)
            return 1;
    }

    return 0;
}

// If the interrupt is enabled and it's interrupt flag is currently set, execute it
//
inline void Z80TestInterrupt(Z80Context* ctx, uint8_t intFlag, uint16_t address)
{
    if (mmu->io[IO_INTENABLE] & mmu->io[IO_INTFLAGS] & intFlag)
    {
        mmu->io[IO_INTFLAGS] &= ~intFlag;
        ctx->interruptStatus = INTSTATE_DISABLED;
        ctx->halted = 0;

        // Store and jump to address
        ctx->SP -= 2;
        MMUWriteWord(mmu, ctx->SP, ctx->PC);
        ctx->PC = address;
    }
}

// Internal timer update
//
inline void Z80TimerStep(Z80Context* ctx)
{
    ctx->timersub += ctx->cycles;
    if (ctx->timersub >= 4)
    {
        ctx->timersub -= 4;

        // DIV
        // Increment every 16384ms
        ctx->timerdiv++;
        if (ctx->timerdiv == 16)
        {
            mmu->io[IO_TMR_DIV]++;
            ctx->timerdiv = 0;
        }

        // TIMA
        // Increment every TAC ms
        if (mmu->io[IO_TMR_TAC] & 0x04)
        {
            ctx->timermain++;

            // Determine threshold (timer speed)
            uint16_t threshold;
            switch(mmu->io[IO_TMR_TAC] & 0x03)
            {
                case 0: threshold = 64; break;
                case 1: threshold = 1; break;
                case 2: threshold = 4; break;
                case 3: threshold = 16; break;
                default: threshold = 1; break;
            }

            // Timer step
            if (ctx->timermain >= threshold)
            {
                ctx->timermain = 0;
                mmu->io[IO_TMR_TIMA]++;

                // Overflowed
                if (mmu->io[IO_TMR_TIMA] == 0)
                {
                    mmu->io[IO_TMR_TIMA] = mmu->io[IO_TMR_TMA];
                    mmu->io[IO_INTFLAGS] |= Z80INT_TIMER;
                }
            }
        }
    }
}

// Reset CPU
//
void Z80Reset(Z80Context* ctx)
{
    ctx->A = 0x01;
    ctx->B = 0x00;
    ctx->C = 0x13;
    ctx->D = 0x00;
    ctx->E = 0xD8;
    ctx->H = 0x01;
    ctx->L = 0x4D;

    ctx->F = 0xB0;
    ctx->PC = 0x100;
    ctx->SP = 0xFFFE;

    ctx->cycleCount = 0;
    ctx->cycles = 0;

    MMUSetBIOS(mmu, 0);
    mmu->io[IO_LCDCONTROL] |= LCD_DISPLAY;

    printf("Z80: Reset.\n");
}
