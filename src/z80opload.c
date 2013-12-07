#include "renderlib.h"
#include "z80.h"
#include "z80opcodes.h"
#include "mmu.h"


// LD nn,n
void Z80_LDnnB(Z80Context* ctx)
{
    ctx->B = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDnnC(Z80Context* ctx)
{
    ctx->C = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDnnD(Z80Context* ctx)
{
    ctx->D = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDnnE(Z80Context* ctx)
{
    ctx->E = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDnnH(Z80Context* ctx)
{
    ctx->H = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDnnL(Z80Context* ctx)
{
    ctx->L = MMUReadByte(mmu, ctx->PC++);
}

// LD r1,r2
// A
void Z80_LDAA(Z80Context* ctx)
{
    ctx->A = ctx->A;
}
void Z80_LDAB(Z80Context* ctx)
{
    ctx->A = ctx->B;
}
void Z80_LDAC(Z80Context* ctx)
{
    ctx->A = ctx->C;
}
void Z80_LDAD(Z80Context* ctx)
{
    ctx->A = ctx->D;
}
void Z80_LDAE(Z80Context* ctx)
{
    ctx->A = ctx->E;
}
void Z80_LDAH(Z80Context* ctx)
{
    ctx->A = ctx->H;
}
void Z80_LDAL(Z80Context* ctx)
{
    ctx->A = ctx->L;
}
void Z80_LDAHL(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, HL_ADDR);
}

// B
void Z80_LDBB(Z80Context* ctx)
{
    ctx->B = ctx->B;
}
void Z80_LDBC(Z80Context* ctx)
{
    ctx->B = ctx->C;
}
void Z80_LDBD(Z80Context* ctx)
{
    ctx->B = ctx->D;
}
void Z80_LDBE(Z80Context* ctx)
{
    ctx->B = ctx->E;
}
void Z80_LDBH(Z80Context* ctx)
{
    ctx->B = ctx->H;

}
void Z80_LDBL(Z80Context* ctx)
{
    ctx->B = ctx->L;
}
void Z80_LDBHL(Z80Context* ctx)
{
    ctx->B = MMUReadByte(mmu, HL_ADDR);
}

// C
void Z80_LDCB(Z80Context* ctx)
{
    ctx->C = ctx->B;
}
void Z80_LDCC(Z80Context* ctx)
{
    ctx->C = ctx->C;
}
void Z80_LDCD(Z80Context* ctx)
{
    ctx->C = ctx->D;
}
void Z80_LDCE(Z80Context* ctx)
{
    ctx->C = ctx->E;
}
void Z80_LDCH(Z80Context* ctx)
{
    ctx->C = ctx->H;
}
void Z80_LDCL(Z80Context* ctx)
{
    ctx->C = ctx->L;
}
void Z80_LDCHL(Z80Context* ctx)
{
    ctx->C = MMUReadByte(mmu, HL_ADDR);
}

// D
void Z80_LDDB(Z80Context* ctx)
{
    ctx->D = ctx->B;
}
void Z80_LDDC(Z80Context* ctx)
{
    ctx->D = ctx->C;
}
void Z80_LDDD(Z80Context* ctx)
{
    ctx->D = ctx->D;
}
void Z80_LDDE(Z80Context* ctx)
{
    ctx->D = ctx->E;
}
void Z80_LDDH(Z80Context* ctx)
{
    ctx->D = ctx->H;
}
void Z80_LDDL(Z80Context* ctx)
{
    ctx->D = ctx->L;
}
void Z80_LDDHL(Z80Context* ctx)
{
    ctx->D = MMUReadByte(mmu, HL_ADDR);
}

// E
void Z80_LDEB(Z80Context* ctx)
{
    ctx->E = ctx->B;
}
void Z80_LDEC(Z80Context* ctx)
{
    ctx->E = ctx->C;
}
void Z80_LDED(Z80Context* ctx)
{
    ctx->E = ctx->D;
}
void Z80_LDEE(Z80Context* ctx)
{
    ctx->E = ctx->E;
}
void Z80_LDEH(Z80Context* ctx)
{
    ctx->E = ctx->H;
}
void Z80_LDEL(Z80Context* ctx)
{
    ctx->E = ctx->L;
}
void Z80_LDEHL(Z80Context* ctx)
{
    ctx->E = MMUReadByte(mmu, HL_ADDR);
}

// H
void Z80_LDHB(Z80Context* ctx)
{
    ctx->H = ctx->B;
}
void Z80_LDHC(Z80Context* ctx)
{
    ctx->H = ctx->C;
}
void Z80_LDHD(Z80Context* ctx)
{
    ctx->H = ctx->D;
}
void Z80_LDHE(Z80Context* ctx)
{
    ctx->H = ctx->E;
}
void Z80_LDHH(Z80Context* ctx)
{
    ctx->H = ctx->H;
}
void Z80_LDHL(Z80Context* ctx)
{
    ctx->H = ctx->L;
}
void Z80_LDHHL(Z80Context* ctx)
{
    ctx->H = MMUReadByte(mmu, HL_ADDR);
}

// L
void Z80_LDLB(Z80Context* ctx)
{
    ctx->L = ctx->B;
}
void Z80_LDLC(Z80Context* ctx)
{
    ctx->L = ctx->C;
}
void Z80_LDLD(Z80Context* ctx)
{
    ctx->L = ctx->D;
}
void Z80_LDLE(Z80Context* ctx)
{
    ctx->L = ctx->E;
}
void Z80_LDLH(Z80Context* ctx)
{
    ctx->L = ctx->H;
}
void Z80_LDLL(Z80Context* ctx)
{
    ctx->L = ctx->L;
}
void Z80_LDLHL(Z80Context* ctx)
{
    ctx->L = MMUReadByte(mmu, HL_ADDR);
}

// HL
void Z80_LDHLB(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->B);
}
void Z80_LDHLC(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->C);
}
void Z80_LDHLD(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->D);
}
void Z80_LDHLE(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->E);
}
void Z80_LDHLH(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->H);
}
void Z80_LDHLL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->L);
}
void Z80_LDHLn(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, ctx->PC++));
}

// LD a,n
void Z80_LDaBC(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, BC_ADDR);
}
void Z80_LDaDE(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, DE_ADDR);
}
void Z80_LDan(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, ctx->PC);
    ctx->PC += 1;
}
void Z80_LDann(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, MMUReadByte(mmu, ctx->PC++));
}

// LD n,a
void Z80_LDBA(Z80Context* ctx)
{
    ctx->B = ctx->A;
}
void Z80_LDCA(Z80Context* ctx)
{
    ctx->C = ctx->A;
}
void Z80_LDDA(Z80Context* ctx)
{
    ctx->D = ctx->A;
}
void Z80_LDEA(Z80Context* ctx)
{
    ctx->E = ctx->A;
}
void Z80_LDHA(Z80Context* ctx)
{
    ctx->H = ctx->A;
}
void Z80_LDLA(Z80Context* ctx)
{
    ctx->L = ctx->A;
}

void Z80_LDBCA(Z80Context* ctx)
{
    MMUWriteByte(mmu, BC_ADDR, ctx->A);
}
void Z80_LDDEA(Z80Context* ctx)
{
    MMUWriteByte(mmu, DE_ADDR, ctx->A);
}
void Z80_LDHLA(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->A);
}
void Z80_LDnnA(Z80Context* ctx)
{
    MMUWriteByte(mmu, MMUReadWord(mmu, ctx->PC), ctx->A);
    ctx->PC += 2;
}

// LD a,mm
void Z80_LDAmm(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, MMUReadWord(mmu, ctx->PC));
    ctx->PC += 2;
}

// LDH A,(C)
void Z80_LDHAC(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, 0xFF00 + ctx->C);
}

// LDH (C),A
void Z80_LDHCA(Z80Context* ctx)
{
    MMUWriteByte(mmu, 0xFF00 + ctx->C, ctx->A);
}

// LDD A,(HL)
void Z80_LDDAHL(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, HL_ADDR);

    if (ctx->L == 0)
    {
        ctx->L = 0xFF;
        ctx->H--;
    }
    else
        ctx->L--;
}

// LDD (HL),A
void Z80_LDDHLA(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->A);

    if (ctx->L == 0)
    {
        ctx->L = 0xFF;
        ctx->H--;
    }
    else
        ctx->L--;
}

// LDI A,(HL)
void Z80_LDIAHL(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, HL_ADDR);

    if (ctx->L == 0xFF)
    {
        ctx->L = 0;
        ctx->H++;
    }
    else
        ctx->L++;
}

// LDI (HL),A
void Z80_LDIHLA(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, ctx->A);

    if (ctx->L == 255)
    {
        ctx->L = 0;
        ctx->H++;
    }
    else
        ctx->L++;
}

// LDH (n),A
void Z80_LDHnA(Z80Context* ctx)
{
    MMUWriteByte(mmu, 0xFF00 + MMUReadByte(mmu, ctx->PC++), ctx->A);
}

// LDH A,(n)
void Z80_LDHAn(Z80Context* ctx)
{
    ctx->A = MMUReadByte(mmu, 0xFF00 + MMUReadByte(mmu, ctx->PC++));
}

// LD n,nn
void Z80_LDBCnn(Z80Context* ctx)
{
    ctx->C = MMUReadByte(mmu, ctx->PC++);
    ctx->B = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDDEnn(Z80Context* ctx)
{
    ctx->E = MMUReadByte(mmu, ctx->PC++);
    ctx->D = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDHLnn(Z80Context* ctx)
{
    ctx->L = MMUReadByte(mmu, ctx->PC++);
    ctx->H = MMUReadByte(mmu, ctx->PC++);
}
void Z80_LDSPnn(Z80Context* ctx)
{
    ctx->SP = MMUReadWord(mmu, ctx->PC);
    ctx->PC += 2;
}

// LDHL SP,n
void Z80_LDHLSPn(Z80Context* ctx)
{
    int8_t offset = MMUReadByte(mmu, ctx->PC++);
    uint16_t newHL = ctx->SP + offset;

    ctx->L = (uint8_t)newHL;
    ctx->H = (uint8_t)(newHL >> 8);

    int16_t result = ctx->SP ^ offset ^ HL_ADDR;
    ctx->F = 0;
    if ((result & 0x100) == 0x100)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;
    if ((result & 0x10) == 0x10)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;
}

// LD (nn),SP
void Z80_LDnnSP(Z80Context* ctx)
{
    MMUWriteWord(mmu, MMUReadWord(mmu, ctx->PC), ctx->SP);
    ctx->PC += 2;
}

// LD SP, HL
void Z80_LDSPHL(Z80Context* ctx)
{
    ctx->SP = HL_ADDR;
}

// PUSH nn
void Z80_PUSHAF(Z80Context* ctx)
{
    MMUWriteByte(mmu, --ctx->SP, ctx->A);
    MMUWriteByte(mmu, --ctx->SP, ctx->F);
}
void Z80_PUSHBC(Z80Context* ctx)
{
    MMUWriteByte(mmu, --ctx->SP, ctx->B);
    MMUWriteByte(mmu, --ctx->SP, ctx->C);
}
void Z80_PUSHDE(Z80Context* ctx)
{
    MMUWriteByte(mmu, --ctx->SP, ctx->D);
    MMUWriteByte(mmu, --ctx->SP, ctx->E);
}
void Z80_PUSHHL(Z80Context* ctx)
{
    MMUWriteByte(mmu, --ctx->SP, ctx->H);
    MMUWriteByte(mmu, --ctx->SP, ctx->L);
}

// POP nn
void Z80_POPAF(Z80Context* ctx)
{
    uint8_t input = MMUReadByte(mmu, ctx->SP++);

    if (input > 0x7F)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    if ((input & 0x40) == 0x040)
        ctx->F |= FLAG_NEG;
    else
        ctx->F &= ~FLAG_NEG;

    if ((input & 0x20) == 0x20)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;

    if ((input & 0x10) == 0x10)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    ctx->A = MMUReadByte(mmu, ctx->SP++);
}
void Z80_POPBC(Z80Context* ctx)
{
    ctx->C = MMUReadByte(mmu, ctx->SP++);
    ctx->B = MMUReadByte(mmu, ctx->SP++);
}
void Z80_POPDE(Z80Context* ctx)
{
    ctx->E = MMUReadByte(mmu, ctx->SP++);
    ctx->D = MMUReadByte(mmu, ctx->SP++);
}
void Z80_POPHL(Z80Context* ctx)
{
    ctx->L = MMUReadByte(mmu, ctx->SP++);
    ctx->H = MMUReadByte(mmu, ctx->SP++);
}
