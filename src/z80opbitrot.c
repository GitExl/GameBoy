#include "renderlib.h"
#include "z80.h"
#include "z80opcodes.h"
#include "mmu.h"


inline uint8_t Z80_RLCHelper(Z80Context* ctx, const uint8_t value);
inline uint8_t Z80_RLHelper(Z80Context* ctx, const uint8_t value);
inline uint8_t Z80_RRCHelper(Z80Context* ctx, const uint8_t value);
inline uint8_t Z80_RRHelper(Z80Context* ctx, const uint8_t value);

inline uint8_t Z80_SLAHelper(Z80Context* ctx, uint8_t value);
inline uint8_t Z80_SRAHelper(Z80Context* ctx, uint8_t value);
inline uint8_t Z80_SRLHelper(Z80Context* ctx, uint8_t value);


// RLCn
// Shift into carry
// Carry flag goes to bit 0
// Bit 7 goes into carry flag
//
// C  7 6 5 4 3 2 1 0
// ------------------
// 0  1 1 1 1 0 1 1 0 <<
// 1  1 1 1 0 1 1 0 1
void Z80_RLCAa(Z80Context* ctx)
{
    ctx->F = 0;
    if (ctx->A > 0x7F)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    ctx->A = (ctx->A << 1) | (ctx->A >> 7);
}

void Z80_RLCA(Z80Context* ctx)
{
    ctx->A = Z80_RLCHelper(ctx, ctx->A);
}
void Z80_RLCB(Z80Context* ctx)
{
    ctx->B = Z80_RLCHelper(ctx, ctx->B);
}
void Z80_RLCC(Z80Context* ctx)
{
    ctx->C = Z80_RLCHelper(ctx, ctx->C);
}
void Z80_RLCD(Z80Context* ctx)
{
    ctx->D = Z80_RLCHelper(ctx, ctx->D);
}
void Z80_RLCE(Z80Context* ctx)
{
    ctx->E = Z80_RLCHelper(ctx, ctx->E);
}
void Z80_RLCH(Z80Context* ctx)
{
    ctx->H = Z80_RLCHelper(ctx, ctx->H);
}
void Z80_RLCL(Z80Context* ctx)
{
    ctx->L = Z80_RLCHelper(ctx, ctx->L);
}
void Z80_RLCHL(Z80Context* ctx)
{
    uint8_t result = Z80_RLCHelper(ctx, MMUReadByte(mmu, HL_ADDR));
    MMUWriteByte(mmu, HL_ADDR, result);
}

inline uint8_t Z80_RLCHelper(Z80Context* ctx, const uint8_t value)
{
    if (value > 0x7F)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    uint8_t result = (value << 1) | ((ctx->F & FLAG_CARRY) ? 1 : 0);

    ctx->F &= ~FLAG_HALF;
    ctx->F &= ~FLAG_NEG;

    if (result == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    return result;
}

// RL n
// Shift into carry
// Bit 7 goes into carry and bit 0
//
// C  7 6 5 4 3 2 1 0
// ------------------
// 1  0 1 1 1 0 1 1 0 <<
// 0  1 1 1 0 1 1 0 1
void Z80_RLAa(Z80Context* ctx)
{
    uint8_t carry = (ctx->F & FLAG_CARRY) ? 1 : 0;

    ctx->F = 0;
    if (ctx->A > 0x7F)
        ctx->F |= FLAG_CARRY;

    ctx->A = (ctx->A << 1) | carry;
}
void Z80_RLA(Z80Context* ctx)
{
    ctx->A = Z80_RLHelper(ctx, ctx->A);
}
void Z80_RLB(Z80Context* ctx)
{
    ctx->B = Z80_RLHelper(ctx, ctx->B);
}
void Z80_RLC(Z80Context* ctx)
{
    ctx->C = Z80_RLHelper(ctx, ctx->C);
}
void Z80_RLD(Z80Context* ctx)
{
    ctx->D = Z80_RLHelper(ctx, ctx->D);
}
void Z80_RLE(Z80Context* ctx)
{
    ctx->E = Z80_RLHelper(ctx, ctx->E);
}
void Z80_RLH(Z80Context* ctx)
{
    ctx->H = Z80_RLHelper(ctx, ctx->H);
}
void Z80_RLL(Z80Context* ctx)
{
    ctx->L = Z80_RLHelper(ctx, ctx->L);
}
void Z80_RLHL(Z80Context* ctx)
{
    uint8_t result = Z80_RLHelper(ctx, MMUReadByte(mmu, HL_ADDR));
    MMUWriteByte(mmu, HL_ADDR, result);
}

inline uint8_t Z80_RLHelper(Z80Context* ctx, const uint8_t value)
{
    uint8_t carry = (value > 0x7F) ? 1 : 0;
    uint8_t result = (value << 1) | ((ctx->F & FLAG_CARRY) ? 1 : 0);

    ctx->F = 0;
    if (carry == 1)
        ctx->F |= FLAG_CARRY;
    if (result == 0)
        ctx->F |= FLAG_ZERO;

    return result;
}

// RRC n
// Shift into carry
// Carry flag goes to bit 7
// Bit 0 goes into carry flag
//
//    7 6 5 4 3 2 1 0  C
//    ------------------
// >> 1 1 1 1 0 1 1 0  1
//    1 1 1 1 1 0 1 1  0
void Z80_RRCAa(Z80Context* ctx)
{
    ctx->A = (ctx->A >> 1) | ((ctx->A & 1) << 7);

    ctx->F = 0;
    if (ctx->A > 0x7F)
        ctx->F |= FLAG_CARRY;
}

void Z80_RRCA(Z80Context* ctx)
{
    ctx->A = Z80_RRCHelper(ctx, ctx->A);
}
void Z80_RRCB(Z80Context* ctx)
{
    ctx->B = Z80_RRCHelper(ctx, ctx->B);
}
void Z80_RRCC(Z80Context* ctx)
{
    ctx->C = Z80_RRCHelper(ctx, ctx->C);
}
void Z80_RRCD(Z80Context* ctx)
{
    ctx->D = Z80_RRCHelper(ctx, ctx->D);
}
void Z80_RRCE(Z80Context* ctx)
{
    ctx->E = Z80_RRCHelper(ctx, ctx->E);
}
void Z80_RRCH(Z80Context* ctx)
{
    ctx->H = Z80_RRCHelper(ctx, ctx->H);
}
void Z80_RRCL(Z80Context* ctx)
{
    ctx->L = Z80_RRCHelper(ctx, ctx->L);
}
void Z80_RRCHL(Z80Context* ctx)
{
    uint8_t result = Z80_RRCHelper(ctx, MMUReadByte(mmu, HL_ADDR));
    MMUWriteByte(mmu, HL_ADDR, result);
}

inline uint8_t Z80_RRCHelper(Z80Context* ctx, const uint8_t value)
{
    uint8_t result = (value >> 1) | ((value & 1) << 7);

    ctx->F = 0;
    if (result > 0x7F)
        ctx->F |= FLAG_CARRY;

    if (result == 0)
        ctx->F |= FLAG_ZERO;

    return result;
}

// RR n
// Shift into carry
// Bit 0 goes into carry and bit 7
//
//    7 6 5 4 3 2 1 0  C
//    ------------------
// >> 1 1 1 1 0 1 1 0  1
//    0 1 1 1 1 0 1 1  0
void Z80_RRAa(Z80Context* ctx)
{
    uint8_t carry = (ctx->F & FLAG_CARRY) ? 0x80 : 0;

    ctx->F = 0;
    if ((ctx->A & 1) == 1)
        ctx->F |= FLAG_CARRY;

    ctx->A = (ctx->A >> 1) | carry;
}
void Z80_RRA(Z80Context* ctx)
{
    ctx->A = Z80_RRHelper(ctx, ctx->A);
}
void Z80_RRB(Z80Context* ctx)
{
    ctx->B = Z80_RRHelper(ctx, ctx->B);
}
void Z80_RRC(Z80Context* ctx)
{
    ctx->C = Z80_RRHelper(ctx, ctx->C);
}
void Z80_RRD(Z80Context* ctx)
{
    ctx->D = Z80_RRHelper(ctx, ctx->D);
}
void Z80_RRE(Z80Context* ctx)
{
    ctx->E = Z80_RRHelper(ctx, ctx->E);
}
void Z80_RRH(Z80Context* ctx)
{
    ctx->H = Z80_RRHelper(ctx, ctx->H);
}
void Z80_RRL(Z80Context* ctx)
{
    ctx->L = Z80_RRHelper(ctx, ctx->L);
}
void Z80_RRHL(Z80Context* ctx)
{
    uint8_t result = Z80_RRHelper(ctx, MMUReadByte(mmu, HL_ADDR));
    MMUWriteByte(mmu, HL_ADDR, result);
}

inline uint8_t Z80_RRHelper(Z80Context* ctx, const uint8_t value)
{
    uint8_t carry = (ctx->F & FLAG_CARRY) ? 0x80 : 0;
    ctx->F = 0;
    if ((value & 1) == 1)
        ctx->F |= FLAG_CARRY;

    uint8_t result = (value >> 1) | carry;
    if (result == 0)
        ctx->F |= FLAG_ZERO;

    return result;
}

// SLA n
void Z80_SLAA(Z80Context* ctx)
{
    ctx->A = Z80_SLAHelper(ctx, ctx->A);
}
void Z80_SLAB(Z80Context* ctx)
{
    ctx->B = Z80_SLAHelper(ctx, ctx->B);
}
void Z80_SLAC(Z80Context* ctx)
{
    ctx->C = Z80_SLAHelper(ctx, ctx->C);
}
void Z80_SLAD(Z80Context* ctx)
{
    ctx->D = Z80_SLAHelper(ctx, ctx->D);
}
void Z80_SLAE(Z80Context* ctx)
{
    ctx->E = Z80_SLAHelper(ctx, ctx->E);
}
void Z80_SLAH(Z80Context* ctx)
{
    ctx->H = Z80_SLAHelper(ctx, ctx->H);
}
void Z80_SLAL(Z80Context* ctx)
{
    ctx->L = Z80_SLAHelper(ctx, ctx->L);
}
void Z80_SLAHL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, Z80_SLAHelper(ctx, MMUReadByte(mmu, HL_ADDR)));
}

inline uint8_t Z80_SLAHelper(Z80Context* ctx, uint8_t value)
{
    ctx->F = (value & 0x80) ? FLAG_CARRY : 0;
    value = value << 1;
    ctx->F |= (value == 0) ? FLAG_ZERO : 0;

    return value;
}

// SRA n
void Z80_SRAA(Z80Context* ctx)
{
    ctx->A = Z80_SRAHelper(ctx, ctx->A);
}
void Z80_SRAB(Z80Context* ctx)
{
    ctx->B = Z80_SRAHelper(ctx, ctx->B);
}
void Z80_SRAC(Z80Context* ctx)
{
    ctx->C = Z80_SRAHelper(ctx, ctx->C);
}
void Z80_SRAD(Z80Context* ctx)
{
    ctx->D = Z80_SRAHelper(ctx, ctx->D);
}
void Z80_SRAE(Z80Context* ctx)
{
    ctx->E = Z80_SRAHelper(ctx, ctx->E);
}
void Z80_SRAH(Z80Context* ctx)
{
    ctx->H = Z80_SRAHelper(ctx, ctx->H);
}
void Z80_SRAL(Z80Context* ctx)
{
    ctx->L = Z80_SRAHelper(ctx, ctx->L);
}
void Z80_SRAHL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, Z80_SRAHelper(ctx, MMUReadByte(mmu, HL_ADDR)));
}

inline uint8_t Z80_SRAHelper(Z80Context* ctx, uint8_t value)
{
    ctx->F = (value & 0x01) ? FLAG_CARRY : 0;
    value = (value & 0x80) | (value >> 1);
    ctx->F |= (value == 0) ? FLAG_ZERO : 0;

    return value;
}

// SRL n
void Z80_SRLA(Z80Context* ctx)
{
    ctx->A = Z80_SRLHelper(ctx, ctx->A);
}
void Z80_SRLB(Z80Context* ctx)
{
    ctx->B = Z80_SRLHelper(ctx, ctx->B);
}
void Z80_SRLC(Z80Context* ctx)
{
    ctx->C = Z80_SRLHelper(ctx, ctx->C);
}
void Z80_SRLD(Z80Context* ctx)
{
    ctx->D = Z80_SRLHelper(ctx, ctx->D);
}
void Z80_SRLE(Z80Context* ctx)
{
    ctx->E = Z80_SRLHelper(ctx, ctx->E);
}
void Z80_SRLH(Z80Context* ctx)
{
    ctx->H = Z80_SRLHelper(ctx, ctx->H);
}
void Z80_SRLL(Z80Context* ctx)
{
    ctx->L = Z80_SRLHelper(ctx, ctx->L);
}
void Z80_SRLHL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, Z80_SRLHelper(ctx, MMUReadByte(mmu, HL_ADDR)));
}

inline uint8_t Z80_SRLHelper(Z80Context* ctx, uint8_t value)
{
    ctx->F = (value & 0x01) ? FLAG_CARRY : 0;
    value = value >> 1;
    ctx->F |= (value == 0) ? FLAG_ZERO : 0;

    return value;
}
