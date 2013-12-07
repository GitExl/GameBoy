#include "renderlib.h"
#include "z80.h"
#include "z80opcodes.h"
#include "mmu.h"
#include "display.h"


// Unknown
void Z80_Unknown(Z80Context* ctx)
{
    printf("Z80: Unknown opcode (0x%X) 0x%X at 0x%X. Stopping.\n", MMUReadByte(mmu, ctx->PC - 2), MMUReadByte(mmu, ctx->PC - 1), ctx->PC);
    ctx->halted = 1;
    ctx->interruptStatus = INTSTATE_DISABLED;
}

// NOP
void Z80_NOP(Z80Context* ctx)
{
    return;
}

// HALT
void Z80_HALT(Z80Context* ctx)
{
    if (ctx->interruptStatus == INTSTATE_DISABLED)
        ctx->bugHALT = 1;
    else
        ctx->halted = 1;
}

// STOP
void Z80_STOP(Z80Context* ctx)
{
    ctx->stopped = 1;
    mmu->io[IO_LCDCONTROL] &= ~LCD_DISPLAY;
}

// DI\EI
void Z80_DI(Z80Context* ctx)
{
    ctx->interruptStatus = INTSTATE_DISABLED;
}
void Z80_EI(Z80Context* ctx)
{
    if (ctx->interruptStatus == INTSTATE_ENABLENEXT || ctx->halted)
        ctx->interruptStatus = INTSTATE_ENABLED;
    else
        ctx->interruptStatus = INTSTATE_ENABLENEXT;
}

// 0xCB mapping
void Z80_CB(Z80Context* ctx)
{
    Z80CBOpcodeList[MMUReadByte(mmu, ctx->PC++)].op(ctx);
    ctx->PC &= 0xFFFF;
}

// SWAP
void Z80_SWAPA(Z80Context* ctx)
{
    ctx->A = NIBBLESWAP(ctx->A);
    ctx->F = ctx->A ? 0 : FLAG_ZERO;
}
void Z80_SWAPB(Z80Context* ctx)
{
    ctx->B = NIBBLESWAP(ctx->B);
    ctx->F = ctx->B ? 0 : FLAG_ZERO;
}
void Z80_SWAPC(Z80Context* ctx)
{
    ctx->C = NIBBLESWAP(ctx->C);
    ctx->F = ctx->C ? 0 : FLAG_ZERO;
}
void Z80_SWAPD(Z80Context* ctx)
{
    ctx->D = NIBBLESWAP(ctx->D);
    ctx->F = ctx->D ? 0 : FLAG_ZERO;
}
void Z80_SWAPE(Z80Context* ctx)
{
    ctx->E = NIBBLESWAP(ctx->E);
    ctx->F = ctx->E ? 0 : FLAG_ZERO;
}
void Z80_SWAPH(Z80Context* ctx)
{
    ctx->H = NIBBLESWAP(ctx->H);
    ctx->F = ctx->H ? 0 : FLAG_ZERO;
}
void Z80_SWAPL(Z80Context* ctx)
{
    ctx->L = NIBBLESWAP(ctx->L);
    ctx->F = ctx->L ? 0 : FLAG_ZERO;
}
void Z80_SWAPHL(Z80Context* ctx)
{
    uint8_t result = NIBBLESWAP(MMUReadByte(mmu, HL_ADDR));
    MMUWriteByte(mmu, HL_ADDR, result);
    ctx->F = result ? 0 : FLAG_ZERO;
}

// DAA
void Z80_DAA(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_NEG))
    {
        if ((ctx->F & FLAG_CARRY) || ctx->A > 0x99)
        {
            ctx->A += 0x60;
            ctx->F |= FLAG_CARRY;
        }
        if (ctx->F & FLAG_HALF || (ctx->A & 0xF) > 0x9)
        {
            ctx->A += 0x06;
            ctx->F &= ~FLAG_HALF;
        }
    }
    else if ((ctx->F & FLAG_CARRY) && (ctx->F & FLAG_HALF))
    {
        ctx->A += 0x9A;
        ctx->F &= ~FLAG_HALF;
    }
    else if (ctx->F & FLAG_CARRY)
        ctx->A += 0xA0;
    else if (ctx->F & FLAG_HALF)
    {
        ctx->A += 0xFA;
        ctx->F &= ~FLAG_HALF;
    }

    if (ctx->A == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;
}

// CPL
void Z80_CPL(Z80Context* ctx)
{
    ctx->A ^= 0xFF;
    ctx->F |= FLAG_HALF | FLAG_NEG;
}

// CCF
void Z80_CCF(Z80Context* ctx)
{
    if (ctx->F & FLAG_CARRY)
        ctx->F &= ~FLAG_CARRY;
    else
        ctx->F |= FLAG_CARRY;

    ctx->F &= ~FLAG_NEG;
    ctx->F &= ~FLAG_HALF;
}

// SCF
void Z80_SCF(Z80Context* ctx)
{
    ctx->F |= FLAG_CARRY;
    ctx->F &= ~FLAG_NEG;
    ctx->F &= ~FLAG_HALF;
}

// JP nn
void Z80_JPnn(Z80Context* ctx)
{
    ctx->PC = MMUReadWord(mmu, ctx->PC);
}

// JP cc,nn
void Z80_JPNZnn(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_ZERO))
    {
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles++;
    }
    else
        ctx->PC += 2;
}
void Z80_JPZnn(Z80Context* ctx)
{
    if (ctx->F & FLAG_ZERO)
    {
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles++;
    }
    else
        ctx->PC += 2;
}
void Z80_JPNCnn(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_CARRY))
    {
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles++;
    }
    else
        ctx->PC += 2;
}
void Z80_JPCnn(Z80Context* ctx)
{
    if (ctx->F & FLAG_CARRY)
    {
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles++;
    }
    else
        ctx->PC += 2;
}

// JP (HL)
void Z80_JPHL(Z80Context* ctx)
{
    ctx->PC = HL_ADDR;
}

// JR n
void Z80_JRn(Z80Context* ctx)
{
    ctx->PC += (int8_t)MMUReadByte(mmu, ctx->PC);
    ctx->PC++;
}

// JR cc,n
void Z80_JRNZn(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_ZERO))
    {
        ctx->PC += (int8_t)MMUReadByte(mmu, ctx->PC);
        ctx->cycles++;
    }

    ctx->PC++;
}
void Z80_JRZn(Z80Context* ctx)
{
    if (ctx->F & FLAG_ZERO)
    {
        ctx->PC += (int8_t)MMUReadByte(mmu, ctx->PC);
        ctx->cycles++;
    }

    ctx->PC++;
}
void Z80_JRNCn(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_CARRY))
    {
        ctx->PC += (int8_t)MMUReadByte(mmu, ctx->PC);
        ctx->cycles++;
    }

    ctx->PC++;
}
void Z80_JRCn(Z80Context* ctx)
{
    if (ctx->F & FLAG_CARRY)
    {
        ctx->PC += (int8_t)MMUReadByte(mmu, ctx->PC);
        ctx->cycles++;
    }

    ctx->PC++;
}

// CALL nn
void Z80_CALLnn(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC + 2);
    ctx->PC = MMUReadWord(mmu, ctx->PC);
}

// CALL cc,nn
void Z80_CALLNZnn(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_ZERO))
    {
        ctx->SP -= 2;
        MMUWriteWord(mmu, ctx->SP, ctx->PC + 2);
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles += 3;
    }
    else
        ctx->PC += 2;
}
void Z80_CALLZnn(Z80Context* ctx)
{
    if (ctx->F & FLAG_ZERO)
    {
        ctx->SP -= 2;
        MMUWriteWord(mmu, ctx->SP, ctx->PC + 2);
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles += 3;
    }
    else
        ctx->PC += 2;

}
void Z80_CALLNCnn(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_CARRY))
    {
        ctx->SP -= 2;
        MMUWriteWord(mmu, ctx->SP, ctx->PC + 2);
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles += 3;
    }
    else
        ctx->PC += 2;
}
void Z80_CALLCnn(Z80Context* ctx)
{
    if (ctx->F & FLAG_CARRY)
    {
        ctx->SP -= 2;
        MMUWriteWord(mmu, ctx->SP, ctx->PC + 2);
        ctx->PC = MMUReadWord(mmu, ctx->PC);
        ctx->cycles += 3;
    }
    else
        ctx->PC += 2;
}

// RST n
void Z80_RST00(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x00;
}
void Z80_RST08(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x08;
}
void Z80_RST10(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x10;
}
void Z80_RST18(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x18;
}
void Z80_RST20(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x20;
}
void Z80_RST28(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x28;
}
void Z80_RST30(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x30;
}
void Z80_RST38(Z80Context* ctx)
{
    ctx->SP -= 2;
    MMUWriteWord(mmu, ctx->SP, ctx->PC);
    ctx->PC = 0x38;
}

// RET
void Z80_RET(Z80Context* ctx)
{
    ctx->PC = MMUReadWord(mmu, ctx->SP);
    ctx->SP += 2;
}

// RET cc
void Z80_RETNZ(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_ZERO))
    {
        ctx->PC = MMUReadWord(mmu, ctx->SP);
        ctx->SP += 2;
        ctx->cycles += 3;
    }
}
void Z80_RETZ(Z80Context* ctx)
{
    if (ctx->F & FLAG_ZERO)
    {
        ctx->PC = MMUReadWord(mmu, ctx->SP);
        ctx->SP += 2;
        ctx->cycles += 3;
    }
}
void Z80_RETNC(Z80Context* ctx)
{
    if (!(ctx->F & FLAG_CARRY))
    {
        ctx->PC = MMUReadWord(mmu, ctx->SP);
        ctx->SP += 2;
        ctx->cycles += 3;
    }
}
void Z80_RETC(Z80Context* ctx)
{
    if (ctx->F & FLAG_CARRY)
    {
        ctx->PC = MMUReadWord(mmu, ctx->SP);
        ctx->SP += 2;
        ctx->cycles += 3;
    }
}

// RETI
void Z80_RETI(Z80Context* ctx)
{
    ctx->interruptStatus = INTSTATE_ENABLENEXT;

    ctx->PC = MMUReadWord(mmu, ctx->SP);
    ctx->SP += 2;
}
