#include "renderlib.h"
#include "z80.h"
#include "z80opcodes.h"
#include "mmu.h"


// BIT b,r
// A
void Z80_BIT0A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT0)
}
void Z80_BIT1A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT1)
}
void Z80_BIT2A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT2)
}
void Z80_BIT3A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT3)
}
void Z80_BIT4A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT4)
}
void Z80_BIT5A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT5)
}
void Z80_BIT6A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT6)
}
void Z80_BIT7A(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->A, BIT7)
}

// B
void Z80_BIT0B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT0)
}
void Z80_BIT1B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT1)
}
void Z80_BIT2B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT2)
}
void Z80_BIT3B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT3)
}
void Z80_BIT4B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT4)
}
void Z80_BIT5B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT5)
}
void Z80_BIT6B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT6)
}
void Z80_BIT7B(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->B, BIT7)
}

// C
void Z80_BIT0C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT0)
}
void Z80_BIT1C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT1)
}
void Z80_BIT2C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT2)
}
void Z80_BIT3C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT3)
}
void Z80_BIT4C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT4)
}
void Z80_BIT5C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT5)
}
void Z80_BIT6C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT6)
}
void Z80_BIT7C(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->C, BIT7)
}

// D
void Z80_BIT0D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT0)
}
void Z80_BIT1D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT1)
}
void Z80_BIT2D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT2)
}
void Z80_BIT3D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT3)
}
void Z80_BIT4D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT4)
}
void Z80_BIT5D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT5)
}
void Z80_BIT6D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT6)
}
void Z80_BIT7D(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->D, BIT7)
}

// E
void Z80_BIT0E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT0)
}
void Z80_BIT1E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT1)
}
void Z80_BIT2E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT2)
}
void Z80_BIT3E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT3)
}
void Z80_BIT4E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT4)
}
void Z80_BIT5E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT5)
}
void Z80_BIT6E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT6)
}
void Z80_BIT7E(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->E, BIT7)
}

// H
void Z80_BIT0H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT0)
}
void Z80_BIT1H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT1)
}
void Z80_BIT2H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT2)
}
void Z80_BIT3H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT3)
}
void Z80_BIT4H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT4)
}
void Z80_BIT5H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT5)
}
void Z80_BIT6H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT6)
}
void Z80_BIT7H(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->H, BIT7)
}

// L
void Z80_BIT0L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT0)
}
void Z80_BIT1L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT1)
}
void Z80_BIT2L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT2)
}
void Z80_BIT3L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT3)
}
void Z80_BIT4L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT4)
}
void Z80_BIT5L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT5)
}
void Z80_BIT6L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT6)
}
void Z80_BIT7L(Z80Context* ctx)
{
    ctx->F = BIT_TEST(ctx->L, BIT7)
}

// (HL)
void Z80_BIT0HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT0)
}
void Z80_BIT1HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT1)
}
void Z80_BIT2HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT2)
}
void Z80_BIT3HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT3)
}
void Z80_BIT4HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT4)
}
void Z80_BIT5HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT5)
}
void Z80_BIT6HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT6)
}
void Z80_BIT7HL(Z80Context* ctx)
{
    uint8_t val = MMUReadByte(mmu, HL_ADDR);
    ctx->F = BIT_TEST(val, BIT7)
}

// SET b,r
// A
void Z80_SET0A(Z80Context* ctx)
{
    ctx->A |= BIT0;
}
void Z80_SET1A(Z80Context* ctx)
{
    ctx->A |= BIT1;
}
void Z80_SET2A(Z80Context* ctx)
{
    ctx->A |= BIT2;
}
void Z80_SET3A(Z80Context* ctx)
{
    ctx->A |= BIT3;
}
void Z80_SET4A(Z80Context* ctx)
{
    ctx->A |= BIT4;
}
void Z80_SET5A(Z80Context* ctx)
{
    ctx->A |= BIT5;
}
void Z80_SET6A(Z80Context* ctx)
{
    ctx->A |= BIT6;
}
void Z80_SET7A(Z80Context* ctx)
{
    ctx->A |= BIT7;
}

// B
void Z80_SET0B(Z80Context* ctx)
{
    ctx->B |= BIT0;
}
void Z80_SET1B(Z80Context* ctx)
{
    ctx->B |= BIT1;
}
void Z80_SET2B(Z80Context* ctx)
{
    ctx->B |= BIT2;
}
void Z80_SET3B(Z80Context* ctx)
{
    ctx->B |= BIT3;
}
void Z80_SET4B(Z80Context* ctx)
{
    ctx->B |= BIT4;
}
void Z80_SET5B(Z80Context* ctx)
{
    ctx->B |= BIT5;
}
void Z80_SET6B(Z80Context* ctx)
{
    ctx->B |= BIT6;
}
void Z80_SET7B(Z80Context* ctx)
{
    ctx->B |= BIT7;
}

// C
void Z80_SET0C(Z80Context* ctx)
{
    ctx->C |= BIT0;
}
void Z80_SET1C(Z80Context* ctx)
{
    ctx->C |= BIT1;
}
void Z80_SET2C(Z80Context* ctx)
{
    ctx->C |= BIT2;
}
void Z80_SET3C(Z80Context* ctx)
{
    ctx->C |= BIT3;
}
void Z80_SET4C(Z80Context* ctx)
{
    ctx->C |= BIT4;
}
void Z80_SET5C(Z80Context* ctx)
{
    ctx->C |= BIT5;
}
void Z80_SET6C(Z80Context* ctx)
{
    ctx->C |= BIT6;
}
void Z80_SET7C(Z80Context* ctx)
{
    ctx->C |= BIT7;
}

// D
void Z80_SET0D(Z80Context* ctx)
{
    ctx->D |= BIT0;
}
void Z80_SET1D(Z80Context* ctx)
{
    ctx->D |= BIT1;
}
void Z80_SET2D(Z80Context* ctx)
{
    ctx->D |= BIT2;
}
void Z80_SET3D(Z80Context* ctx)
{
    ctx->D |= BIT3;
}
void Z80_SET4D(Z80Context* ctx)
{
    ctx->D |= BIT4;
}
void Z80_SET5D(Z80Context* ctx)
{
    ctx->D |= BIT5;
}
void Z80_SET6D(Z80Context* ctx)
{
    ctx->D |= BIT6;
}
void Z80_SET7D(Z80Context* ctx)
{
    ctx->D |= BIT7;
}

// E
void Z80_SET0E(Z80Context* ctx)
{
    ctx->E |= BIT0;
}
void Z80_SET1E(Z80Context* ctx)
{
    ctx->E |= BIT1;
}
void Z80_SET2E(Z80Context* ctx)
{
    ctx->E |= BIT2;
}
void Z80_SET3E(Z80Context* ctx)
{
    ctx->E |= BIT3;
}
void Z80_SET4E(Z80Context* ctx)
{
    ctx->E |= BIT4;
}
void Z80_SET5E(Z80Context* ctx)
{
    ctx->E |= BIT5;
}
void Z80_SET6E(Z80Context* ctx)
{
    ctx->E |= BIT6;
}
void Z80_SET7E(Z80Context* ctx)
{
    ctx->E |= BIT7;
}

// H
void Z80_SET0H(Z80Context* ctx)
{
    ctx->H |= BIT0;
}
void Z80_SET1H(Z80Context* ctx)
{
    ctx->H |= BIT1;
}
void Z80_SET2H(Z80Context* ctx)
{
    ctx->H |= BIT2;

}
void Z80_SET3H(Z80Context* ctx)
{
    ctx->H |= BIT3;
}
void Z80_SET4H(Z80Context* ctx)
{
    ctx->H |= BIT4;
}
void Z80_SET5H(Z80Context* ctx)
{
    ctx->H |= BIT5;
}
void Z80_SET6H(Z80Context* ctx)
{
    ctx->H |= BIT6;
}
void Z80_SET7H(Z80Context* ctx)
{
    ctx->H |= BIT7;
}

// L
void Z80_SET0L(Z80Context* ctx)
{
    ctx->L |= BIT0;
}
void Z80_SET1L(Z80Context* ctx)
{
    ctx->L |= BIT1;
}
void Z80_SET2L(Z80Context* ctx)
{
    ctx->L |= BIT2;
}
void Z80_SET3L(Z80Context* ctx)
{
    ctx->L |= BIT3;
}
void Z80_SET4L(Z80Context* ctx)
{
    ctx->L |= BIT4;
}
void Z80_SET5L(Z80Context* ctx)
{
    ctx->L |= BIT5;
}
void Z80_SET6L(Z80Context* ctx)
{
    ctx->L |= BIT6;
}
void Z80_SET7L(Z80Context* ctx)
{
    ctx->L |= BIT7;
}

// (HL)
void Z80_SET0HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT0);
}
void Z80_SET1HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT1);
}
void Z80_SET2HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT2);
}
void Z80_SET3HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT3);
}
void Z80_SET4HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT4);
}
void Z80_SET5HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT5);
}
void Z80_SET6HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT6);
}
void Z80_SET7HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) | BIT7);
}

// RES b,r
// A
void Z80_RES0A(Z80Context* ctx)
{
    ctx->A &= ~BIT0;
}
void Z80_RES1A(Z80Context* ctx)
{
    ctx->A &= ~BIT1;
}
void Z80_RES2A(Z80Context* ctx)
{
    ctx->A &= ~BIT2;
}
void Z80_RES3A(Z80Context* ctx)
{
    ctx->A &= ~BIT3;
}
void Z80_RES4A(Z80Context* ctx)
{
    ctx->A &= ~BIT4;
}
void Z80_RES5A(Z80Context* ctx)
{
    ctx->A &= ~BIT5;
}
void Z80_RES6A(Z80Context* ctx)
{
    ctx->A &= ~BIT6;
}
void Z80_RES7A(Z80Context* ctx)
{
    ctx->A &= ~BIT7;
}

// B
void Z80_RES0B(Z80Context* ctx)
{
    ctx->B &= ~BIT0;
}
void Z80_RES1B(Z80Context* ctx)
{
    ctx->B &= ~BIT1;
}
void Z80_RES2B(Z80Context* ctx)
{
    ctx->B &= ~BIT2;
}
void Z80_RES3B(Z80Context* ctx)
{
    ctx->B &= ~BIT3;
}
void Z80_RES4B(Z80Context* ctx)
{
    ctx->B &= ~BIT4;
}
void Z80_RES5B(Z80Context* ctx)
{
    ctx->B &= ~BIT5;
}
void Z80_RES6B(Z80Context* ctx)
{
    ctx->B &= ~BIT6;
}
void Z80_RES7B(Z80Context* ctx)
{
    ctx->B &= ~BIT7;
}

// C
void Z80_RES0C(Z80Context* ctx)
{
    ctx->C &= ~BIT0;
}
void Z80_RES1C(Z80Context* ctx)
{
    ctx->C &= ~BIT1;
}
void Z80_RES2C(Z80Context* ctx)
{
    ctx->C &= ~BIT2;
}
void Z80_RES3C(Z80Context* ctx)
{
    ctx->C &= ~BIT3;
}
void Z80_RES4C(Z80Context* ctx)
{
    ctx->C &= ~BIT4;
}
void Z80_RES5C(Z80Context* ctx)
{
    ctx->C &= ~BIT5;
}
void Z80_RES6C(Z80Context* ctx)
{
    ctx->C &= ~BIT6;
}
void Z80_RES7C(Z80Context* ctx)
{
    ctx->C &= ~BIT7;
}

// D
void Z80_RES0D(Z80Context* ctx)
{
    ctx->D &= ~BIT0;
}
void Z80_RES1D(Z80Context* ctx)
{
    ctx->D &= ~BIT1;
}
void Z80_RES2D(Z80Context* ctx)
{
    ctx->D &= ~BIT2;
}
void Z80_RES3D(Z80Context* ctx)
{
    ctx->D &= ~BIT3;
}
void Z80_RES4D(Z80Context* ctx)
{
    ctx->D &= ~BIT4;
}
void Z80_RES5D(Z80Context* ctx)
{
    ctx->D &= ~BIT5;
}
void Z80_RES6D(Z80Context* ctx)
{
    ctx->D &= ~BIT6;
}
void Z80_RES7D(Z80Context* ctx)
{
    ctx->D &= ~BIT7;
}

// E
void Z80_RES0E(Z80Context* ctx)
{
    ctx->E &= ~BIT0;
}
void Z80_RES1E(Z80Context* ctx)
{
    ctx->E &= ~BIT1;
}
void Z80_RES2E(Z80Context* ctx)
{
    ctx->E &= ~BIT2;
}
void Z80_RES3E(Z80Context* ctx)
{
    ctx->E &= ~BIT3;
}
void Z80_RES4E(Z80Context* ctx)
{
    ctx->E &= ~BIT4;
}
void Z80_RES5E(Z80Context* ctx)
{
    ctx->E &= ~BIT5;
}
void Z80_RES6E(Z80Context* ctx)
{
    ctx->E &= ~BIT6;
}
void Z80_RES7E(Z80Context* ctx)
{
    ctx->E &= ~BIT7;
}

// H
void Z80_RES0H(Z80Context* ctx)
{
    ctx->H &= ~BIT0;
}
void Z80_RES1H(Z80Context* ctx)
{
    ctx->H &= ~BIT1;
}
void Z80_RES2H(Z80Context* ctx)
{
    ctx->H &= ~BIT2;
}
void Z80_RES3H(Z80Context* ctx)
{
    ctx->H &= ~BIT3;
}
void Z80_RES4H(Z80Context* ctx)
{
    ctx->H &= ~BIT4;
}
void Z80_RES5H(Z80Context* ctx)
{
    ctx->H &= ~BIT5;
}
void Z80_RES6H(Z80Context* ctx)
{
    ctx->H &= ~BIT6;
}
void Z80_RES7H(Z80Context* ctx)
{
    ctx->H &= ~BIT7;
}

// L
void Z80_RES0L(Z80Context* ctx)
{
    ctx->L &= ~BIT0;
}
void Z80_RES1L(Z80Context* ctx)
{
    ctx->L &= ~BIT1;
}
void Z80_RES2L(Z80Context* ctx)
{
    ctx->L &= ~BIT2;
}
void Z80_RES3L(Z80Context* ctx)
{
    ctx->L &= ~BIT3;
}
void Z80_RES4L(Z80Context* ctx)
{
    ctx->L &= ~BIT4;
}
void Z80_RES5L(Z80Context* ctx)
{
    ctx->L &= ~BIT5;
}
void Z80_RES6L(Z80Context* ctx)
{
    ctx->L &= ~BIT6;
}
void Z80_RES7L(Z80Context* ctx)
{
    ctx->L &= ~BIT7;
}

// (HL)
void Z80_RES0HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT0);
}
void Z80_RES1HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT1);
}
void Z80_RES2HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT2);
}
void Z80_RES3HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT3);
}
void Z80_RES4HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT4);
}
void Z80_RES5HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT5);
}
void Z80_RES6HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT6);
}
void Z80_RES7HL(Z80Context* ctx)
{
    MMUWriteByte(mmu, HL_ADDR, MMUReadByte(mmu, HL_ADDR) & ~BIT7);
}
