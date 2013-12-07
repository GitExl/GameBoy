#include "renderlib.h"
#include "z80.h"
#include "z80opcodes.h"
#include "mmu.h"


inline void Z80_AddHelp(Z80Context* ctx, const uint8_t value);
inline void Z80_AddCHelp(Z80Context* ctx, const uint8_t value);

inline void Z80_SubHelp(Z80Context* ctx, const uint8_t value);
inline void Z80_SubCHelp(Z80Context* ctx, const uint8_t value);

inline void Z80_CPHelp(Z80Context* ctx, const uint8_t value);
inline uint8_t Z80_INCHelp(Z80Context* ctx, const uint8_t value);
inline uint8_t Z80_DECHelp(Z80Context* ctx, const uint8_t value);

inline void Z80_ADDHLHelp(Z80Context* ctx, const uint16_t value);
inline void Z80_ORHelp(Z80Context* ctx, const uint8_t value);

inline void Z80_ANDHelp(Z80Context* ctx, const uint8_t value);


// ADD A,n
void Z80_ADDAA(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->A);
}
void Z80_ADDAB(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->B);
}
void Z80_ADDAC(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->C);
}
void Z80_ADDAD(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->D);
}
void Z80_ADDAE(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->E);
}
void Z80_ADDAH(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->H);
}
void Z80_ADDAL(Z80Context* ctx)
{
    Z80_AddHelp(ctx, ctx->L);
}

void Z80_ADDAHL(Z80Context* ctx)
{
    Z80_AddHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_ADDAn(Z80Context* ctx)
{
    Z80_AddHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}

inline void Z80_AddHelp(Z80Context* ctx, const uint8_t value)
{
    int16_t dirtySum = ctx->A + value;

    if ((dirtySum & 0xF) < (ctx->A & 0xF))
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;
    if (dirtySum > 0xFF)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    ctx->A = (dirtySum & 0xFF);

    if (ctx->A == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    ctx->F &= ~FLAG_NEG;
}


// ADC A,n
void Z80_ADCAA(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->A);
}
void Z80_ADCAB(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->B);
}
void Z80_ADCAC(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->C);
}
void Z80_ADCAD(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->D);
}
void Z80_ADCAE(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->E);
}
void Z80_ADCAH(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->H);
}
void Z80_ADCAL(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, ctx->L);
}

void Z80_ADCAHL(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_ADCAn(Z80Context* ctx)
{
    Z80_AddCHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}

inline void Z80_AddCHelp(Z80Context* ctx, const uint8_t value)
{
    int16_t dirtySum = ctx->A + value + ((ctx->F & FLAG_CARRY) ? 1 : 0);

    if ((ctx->A & 0xF) + (value & 0xF) + ((ctx->F & FLAG_CARRY) ? 1 : 0) > 0xF)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;

    if (dirtySum > 0xFF)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    ctx->A = dirtySum & 0xFF;

    if (ctx->A == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    ctx->F &= ~FLAG_NEG;
}

// SUB A,n
void Z80_SUBAA(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->A);
}
void Z80_SUBAB(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->B);
}
void Z80_SUBAC(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->C);
}
void Z80_SUBAD(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->D);
}
void Z80_SUBAE(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->E);
}
void Z80_SUBAH(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->H);
}
void Z80_SUBAL(Z80Context* ctx)
{
    Z80_SubHelp(ctx, ctx->L);
}

void Z80_SUBAHL(Z80Context* ctx)
{
    Z80_SubHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_SUBAn(Z80Context* ctx)
{
    Z80_SubHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}

inline void Z80_SubHelp(Z80Context* ctx, const uint8_t value)
{
    uint8_t old = 0;

    old = ctx->A;
    if (((int16_t)(ctx->A - value)) < 0)
        ctx->F = FLAG_NEG | FLAG_CARRY;
    else
        ctx->F = FLAG_NEG;

    ctx->A -= value;

    if (ctx->A == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    if ((ctx->A ^ value ^ old) & FLAG_CARRY)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;
}


// SBC A,n
void Z80_SBCAA(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->A);
}
void Z80_SBCAB(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->B);
}
void Z80_SBCAC(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->C);
}
void Z80_SBCAD(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->D);
}
void Z80_SBCAE(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->E);
}
void Z80_SBCAH(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->H);
}
void Z80_SBCAL(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, ctx->L);
}
void Z80_SBCAHL(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_SBCAn(Z80Context* ctx)
{
    Z80_SubCHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}

inline void Z80_SubCHelp(Z80Context* ctx, const uint8_t value)
{
    int16_t dirtySum = ctx->A - value - ((ctx->F & FLAG_CARRY) ? 1 : 0);

    if ((ctx->A & 0xF) - (value & 0xF) - ((ctx->F & FLAG_CARRY) ? 1 : 0) < 0)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;

    if (dirtySum < 0)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    ctx->A = dirtySum & 0xFF;

    if (ctx->A == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    ctx->F |= FLAG_NEG;
}

// AND n
void Z80_ANDA(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->A);
}
void Z80_ANDB(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->B);
}
void Z80_ANDC(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->C);
}
void Z80_ANDD(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->D);
}
void Z80_ANDE(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->E);
}
void Z80_ANDH(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->H);
}
void Z80_ANDL(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, ctx->L);
}
void Z80_ANDHL(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_ANDn(Z80Context* ctx)
{
    Z80_ANDHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}

inline void Z80_ANDHelp(Z80Context* ctx, const uint8_t value)
{
    ctx->A &= value;
    ctx->F = (ctx->A == 0) ? FLAG_HALF + FLAG_ZERO : FLAG_HALF;
}


// OR n
void Z80_ORA(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->A);
}
void Z80_ORB(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->B);
}
void Z80_ORC(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->C);
}
void Z80_ORD(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->D);
}
void Z80_ORE(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->E);
}
void Z80_ORH(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->H);
}
void Z80_ORL(Z80Context* ctx)
{
    Z80_ORHelp(ctx, ctx->L);
}
void Z80_ORHL(Z80Context* ctx)
{
    Z80_ORHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_ORn(Z80Context* ctx)
{
    Z80_ORHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}
inline void Z80_ORHelp(Z80Context* ctx, const uint8_t value)
{
    ctx->A |= value;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}

// XOR n
void Z80_XORA(Z80Context* ctx)
{
    ctx->A ^= ctx->A;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORB(Z80Context* ctx)
{
    ctx->A ^= ctx->B;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORC(Z80Context* ctx)
{
    ctx->A ^= ctx->C;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORD(Z80Context* ctx)
{
    ctx->A ^= ctx->D;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORE(Z80Context* ctx)
{
    ctx->A ^= ctx->E;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORH(Z80Context* ctx)
{
    ctx->A ^= ctx->H;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORL(Z80Context* ctx)
{
    ctx->A ^= ctx->L;
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORHL(Z80Context* ctx)
{
    ctx->A ^= MMUReadByte(mmu, HL_ADDR);
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}
void Z80_XORn(Z80Context* ctx)
{
    ctx->A ^= MMUReadByte(mmu, ctx->PC++);
    ctx->F = (ctx->A == 0) ? FLAG_ZERO : 0;
}

// CP n
void Z80_CPA(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->A);
}
void Z80_CPB(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->B);
}
void Z80_CPC(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->C);
}
void Z80_CPD(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->D);
}
void Z80_CPE(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->E);
}
void Z80_CPH(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->H);
}
void Z80_CPLc(Z80Context* ctx)
{
    Z80_CPHelp(ctx, ctx->L);
}
void Z80_CPHL(Z80Context* ctx)
{
    Z80_CPHelp(ctx, MMUReadByte(mmu, HL_ADDR));
}
void Z80_CPn(Z80Context* ctx)
{
    Z80_CPHelp(ctx, MMUReadByte(mmu, ctx->PC++));
}

inline void Z80_CPHelp(Z80Context* ctx, const uint8_t value)
{
    int16_t dirtySum = ctx->A - value;

    ctx->F = 0;
    if ((dirtySum & 0xF) > (ctx->A & 0xF))
        ctx->F |= FLAG_HALF;
    if (dirtySum < 0)
        ctx->F |= FLAG_CARRY;
    if (dirtySum == 0)
        ctx->F |= FLAG_ZERO;
    ctx->F |= FLAG_NEG;
}

// INC n
void Z80_INCA(Z80Context* ctx)
{
    ctx->A = Z80_INCHelp(ctx, ctx->A);
}
void Z80_INCB(Z80Context* ctx)
{
    ctx->B = Z80_INCHelp(ctx, ctx->B);
}
void Z80_INCC(Z80Context* ctx)
{
    ctx->C = Z80_INCHelp(ctx, ctx->C);
}
void Z80_INCD(Z80Context* ctx)
{
    ctx->D = Z80_INCHelp(ctx, ctx->D);
}
void Z80_INCE(Z80Context* ctx)
{
    ctx->E = Z80_INCHelp(ctx, ctx->E);
}
void Z80_INCH(Z80Context* ctx)
{
    ctx->H = Z80_INCHelp(ctx, ctx->H);
}
void Z80_INCL(Z80Context* ctx)
{
    ctx->L = Z80_INCHelp(ctx, ctx->L);
}
void Z80_INCHLL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, Z80_INCHelp(ctx, MMUReadByte(mmu, HL_ADDR)));
}

inline uint8_t Z80_INCHelp(Z80Context* ctx, const uint8_t value)
{
	uint8_t result = value + 1;

	if (result == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    if ((result & 0xF) == 0)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;

    ctx->F &= ~FLAG_NEG;

    return result;
}

// DEC n
void Z80_DECA(Z80Context* ctx)
{
    ctx->A = Z80_DECHelp(ctx, ctx->A);
}
void Z80_DECB(Z80Context* ctx)
{
    ctx->B = Z80_DECHelp(ctx, ctx->B);
}
void Z80_DECC(Z80Context* ctx)
{
    ctx->C = Z80_DECHelp(ctx, ctx->C);
}
void Z80_DECD(Z80Context* ctx)
{
    ctx->D = Z80_DECHelp(ctx, ctx->D);
}
void Z80_DECE(Z80Context* ctx)
{
    ctx->E = Z80_DECHelp(ctx, ctx->E);
}
void Z80_DECH(Z80Context* ctx)
{
    ctx->H = Z80_DECHelp(ctx, ctx->H);
}
void Z80_DECL(Z80Context* ctx)
{
    ctx->L = Z80_DECHelp(ctx, ctx->L);
}
void Z80_DECHLL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, Z80_DECHelp(ctx, MMUReadByte(mmu, HL_ADDR)));
}

inline uint8_t Z80_DECHelp(Z80Context* ctx, uint8_t value)
{
    uint8_t result = value - 1;

    if (result == 0)
        ctx->F |= FLAG_ZERO;
    else
        ctx->F &= ~FLAG_ZERO;

    if ((result & 0xF) == 0xF)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;

    ctx->F |= FLAG_NEG;

    return result;
}

// ADD HL,n
void Z80_ADDHLBC(Z80Context* ctx)
{
    Z80_ADDHLHelp(ctx, BC_ADDR);
}
void Z80_ADDHLDE(Z80Context* ctx)
{
    Z80_ADDHLHelp(ctx, DE_ADDR);
}
void Z80_ADDHLHL(Z80Context* ctx)
{
    Z80_ADDHLHelp(ctx, HL_ADDR);
}
void Z80_ADDHLSP(Z80Context* ctx)
{
    Z80_ADDHLHelp(ctx, ctx->SP);
}

inline void Z80_ADDHLHelp(Z80Context* ctx, const uint16_t value)
{
    int32_t dirtySum = HL_ADDR + value;

    if ((HL_ADDR & 0xFFF) > (dirtySum & 0xFFF))
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;
    if (dirtySum > 0xFFFF)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    ctx->L = (uint8_t)dirtySum;
    ctx->H = (uint8_t)(dirtySum >> 8);

    ctx->F &= ~FLAG_NEG;
}

// ADD SP,n
void Z80_ADDSPn(Z80Context* ctx)
{
    int16_t value = (int8_t)MMUReadByte(mmu, ctx->PC++);
    uint16_t newSP = ctx->SP + value;

    value = ctx->SP ^ value ^ newSP;
    ctx->SP = newSP;

    ctx->F = 0;
    if ((value & 0x100) == 0x100)
        ctx->F |= FLAG_CARRY;
    else
        ctx->F &= ~FLAG_CARRY;

    if ((value & 0x10) == 0x10)
        ctx->F |= FLAG_HALF;
    else
        ctx->F &= ~FLAG_HALF;
}

// INC nn
void Z80_INCBC(Z80Context* ctx)
{
    if (ctx->C == 0xFF)
    {
        ctx->C = 0;
        ctx->B++;
    }
    else
        ctx->C++;
}
void Z80_INCDE(Z80Context* ctx)
{
    if (ctx->E == 0xFF)
    {
        ctx->E = 0;
        ctx->D++;
    }
    else
        ctx->E++;
}
void Z80_INCHL(Z80Context* ctx)
{
    if (ctx->L == 0xFF)
    {
        ctx->L = 0;
        ctx->H++;
    }
    else
        ctx->L++;
}
void Z80_INCSP(Z80Context* ctx)
{
    ctx->SP++;
}

// DEC nn
void Z80_DECBC(Z80Context* ctx)
{
    if (ctx->C == 0)
    {
        ctx->C = 0xFF;
        ctx->B--;
    }
    else
        ctx->C--;
}
void Z80_DECDE(Z80Context* ctx)
{
    if (ctx->E == 0)
    {
        ctx->E = 0xFF;
        ctx->D--;
    }
    else
        ctx->E--;
}
void Z80_DECHL(Z80Context* ctx)
{
    if (ctx->L == 0)
    {
        ctx->L = 0xFF;
        ctx->H--;
    }
    else
        ctx->L--;
}
void Z80_DECSP(Z80Context* ctx)
{
    ctx->SP--;
}
