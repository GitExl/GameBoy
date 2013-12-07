#ifndef H_Z80OPCODES
#define H_Z80OPCODES

#include "z80.h"


#define COMB16(A, B) (uint16_t)(B + (A << 8))

#define BC_ADDR (uint16_t)(ctx->C + ((ctx->B) << 8))
#define DE_ADDR (uint16_t)(ctx->E + ((ctx->D) << 8))
#define HL_ADDR (uint16_t)(ctx->L + ((ctx->H) << 8))

#define NIBBLESWAP(VAL) (((VAL & 0xF) << 4) | ((VAL & 0xF0) >> 4));

#define FLAG_ZERO  0x80
#define FLAG_NEG   0x40
#define FLAG_HALF  0x20
#define FLAG_CARRY 0x10

#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define BIT_TEST(VAL, BIT) (ctx->F & FLAG_CARRY) | FLAG_HALF | ((VAL & BIT) ? 0 : FLAG_ZERO);


typedef void opFunc(Z80Context* ctx);

typedef struct
{
    opFunc* op;

    char* instruction;
    char* parameters;

    uint8_t length;
    uint8_t cycles;
} Z80Opcode;


void Z80_Unknown(Z80Context* ctx);
void Z80_CB(Z80Context* ctx);

void Z80_NOP(Z80Context* ctx);
void Z80_HALT(Z80Context* ctx);
void Z80_STOP(Z80Context* ctx);
void Z80_DI(Z80Context* ctx);
void Z80_EI(Z80Context* ctx);

void Z80_DAA(Z80Context* ctx);
void Z80_CPL(Z80Context* ctx);
void Z80_CCF(Z80Context* ctx);
void Z80_SCF(Z80Context* ctx);

// Jumps
void Z80_JPnn(Z80Context* ctx);
void Z80_JPNZnn(Z80Context* ctx);
void Z80_JPZnn(Z80Context* ctx);
void Z80_JPNCnn(Z80Context* ctx);
void Z80_JPCnn(Z80Context* ctx);

void Z80_JPHL(Z80Context* ctx);

void Z80_JRn(Z80Context* ctx);
void Z80_JRNZn(Z80Context* ctx);
void Z80_JRZn(Z80Context* ctx);
void Z80_JRNCn(Z80Context* ctx);
void Z80_JRCn(Z80Context* ctx);

// Calls
void Z80_CALLnn(Z80Context* ctx);
void Z80_CALLNZnn(Z80Context* ctx);
void Z80_CALLZnn(Z80Context* ctx);
void Z80_CALLNCnn(Z80Context* ctx);
void Z80_CALLCnn(Z80Context* ctx);

// Restarts
void Z80_RST00(Z80Context* ctx);
void Z80_RST08(Z80Context* ctx);
void Z80_RST10(Z80Context* ctx);
void Z80_RST18(Z80Context* ctx);
void Z80_RST20(Z80Context* ctx);
void Z80_RST28(Z80Context* ctx);
void Z80_RST30(Z80Context* ctx);
void Z80_RST38(Z80Context* ctx);

// Returns
void Z80_RET(Z80Context* ctx);
void Z80_RETNZ(Z80Context* ctx);
void Z80_RETZ(Z80Context* ctx);
void Z80_RETNC(Z80Context* ctx);
void Z80_RETC(Z80Context* ctx);

void Z80_RETI(Z80Context* ctx);

// Loads
void Z80_LDAmm(Z80Context* ctx);

void Z80_LDnnB(Z80Context* ctx);
void Z80_LDnnC(Z80Context* ctx);
void Z80_LDnnD(Z80Context* ctx);
void Z80_LDnnE(Z80Context* ctx);
void Z80_LDnnH(Z80Context* ctx);
void Z80_LDnnL(Z80Context* ctx);

void Z80_LDAA(Z80Context* ctx);
void Z80_LDAB(Z80Context* ctx);
void Z80_LDAC(Z80Context* ctx);
void Z80_LDAD(Z80Context* ctx);
void Z80_LDAE(Z80Context* ctx);
void Z80_LDAH(Z80Context* ctx);
void Z80_LDAL(Z80Context* ctx);
void Z80_LDAHL(Z80Context* ctx);

void Z80_LDBB(Z80Context* ctx);
void Z80_LDBC(Z80Context* ctx);
void Z80_LDBD(Z80Context* ctx);
void Z80_LDBE(Z80Context* ctx);
void Z80_LDBH(Z80Context* ctx);
void Z80_LDBL(Z80Context* ctx);
void Z80_LDBHL(Z80Context* ctx);

void Z80_LDCB(Z80Context* ctx);
void Z80_LDCC(Z80Context* ctx);
void Z80_LDCD(Z80Context* ctx);
void Z80_LDCE(Z80Context* ctx);
void Z80_LDCH(Z80Context* ctx);
void Z80_LDCL(Z80Context* ctx);
void Z80_LDCHL(Z80Context* ctx);

void Z80_LDDB(Z80Context* ctx);
void Z80_LDDC(Z80Context* ctx);
void Z80_LDDD(Z80Context* ctx);
void Z80_LDDE(Z80Context* ctx);
void Z80_LDDH(Z80Context* ctx);
void Z80_LDDL(Z80Context* ctx);
void Z80_LDDHL(Z80Context* ctx);

void Z80_LDEB(Z80Context* ctx);
void Z80_LDEC(Z80Context* ctx);
void Z80_LDED(Z80Context* ctx);
void Z80_LDEE(Z80Context* ctx);
void Z80_LDEH(Z80Context* ctx);
void Z80_LDEL(Z80Context* ctx);
void Z80_LDEHL(Z80Context* ctx);

void Z80_LDHB(Z80Context* ctx);
void Z80_LDHC(Z80Context* ctx);
void Z80_LDHD(Z80Context* ctx);
void Z80_LDHE(Z80Context* ctx);
void Z80_LDHH(Z80Context* ctx);
void Z80_LDHL(Z80Context* ctx);
void Z80_LDHHL(Z80Context* ctx);

void Z80_LDLB(Z80Context* ctx);
void Z80_LDLC(Z80Context* ctx);
void Z80_LDLD(Z80Context* ctx);
void Z80_LDLE(Z80Context* ctx);
void Z80_LDLH(Z80Context* ctx);
void Z80_LDLL(Z80Context* ctx);
void Z80_LDLHL(Z80Context* ctx);

void Z80_LDHLB(Z80Context* ctx);
void Z80_LDHLC(Z80Context* ctx);
void Z80_LDHLD(Z80Context* ctx);
void Z80_LDHLE(Z80Context* ctx);
void Z80_LDHLH(Z80Context* ctx);
void Z80_LDHLL(Z80Context* ctx);
void Z80_LDHLn(Z80Context* ctx);

void Z80_LDaBC(Z80Context* ctx);
void Z80_LDaDE(Z80Context* ctx);
void Z80_LDann(Z80Context* ctx);
void Z80_LDan(Z80Context* ctx);

void Z80_LDBA(Z80Context* ctx);
void Z80_LDCA(Z80Context* ctx);
void Z80_LDDA(Z80Context* ctx);
void Z80_LDEA(Z80Context* ctx);
void Z80_LDHA(Z80Context* ctx);
void Z80_LDLA(Z80Context* ctx);

void Z80_LDBCA(Z80Context* ctx);
void Z80_LDDEA(Z80Context* ctx);
void Z80_LDHLA(Z80Context* ctx);
void Z80_LDnnA(Z80Context* ctx);

void Z80_LDHAC(Z80Context* ctx);
void Z80_LDHCA(Z80Context* ctx);

void Z80_LDDAHL(Z80Context* ctx);
void Z80_LDDHLA(Z80Context* ctx);
void Z80_LDIAHL(Z80Context* ctx);
void Z80_LDIHLA(Z80Context* ctx);

void Z80_LDHnA(Z80Context* ctx);
void Z80_LDHAn(Z80Context* ctx);

void Z80_LDBCnn(Z80Context* ctx);
void Z80_LDDEnn(Z80Context* ctx);
void Z80_LDHLnn(Z80Context* ctx);
void Z80_LDSPnn(Z80Context* ctx);

void Z80_LDHLSPn(Z80Context* ctx);

void Z80_LDnnSP(Z80Context* ctx);
void Z80_LDSPHL(Z80Context* ctx);

void Z80_PUSHAF(Z80Context* ctx);
void Z80_PUSHBC(Z80Context* ctx);
void Z80_PUSHDE(Z80Context* ctx);
void Z80_PUSHHL(Z80Context* ctx);

void Z80_POPAF(Z80Context* ctx);
void Z80_POPBC(Z80Context* ctx);
void Z80_POPDE(Z80Context* ctx);
void Z80_POPHL(Z80Context* ctx);

// ALU
void Z80_ADDAA(Z80Context* ctx);
void Z80_ADDAB(Z80Context* ctx);
void Z80_ADDAC(Z80Context* ctx);
void Z80_ADDAD(Z80Context* ctx);
void Z80_ADDAE(Z80Context* ctx);
void Z80_ADDAH(Z80Context* ctx);
void Z80_ADDAL(Z80Context* ctx);
void Z80_ADDAHL(Z80Context* ctx);
void Z80_ADDAn(Z80Context* ctx);

void Z80_ADCAA(Z80Context* ctx);
void Z80_ADCAB(Z80Context* ctx);
void Z80_ADCAC(Z80Context* ctx);
void Z80_ADCAD(Z80Context* ctx);
void Z80_ADCAE(Z80Context* ctx);
void Z80_ADCAH(Z80Context* ctx);
void Z80_ADCAL(Z80Context* ctx);
void Z80_ADCAHL(Z80Context* ctx);
void Z80_ADCAn(Z80Context* ctx);

void Z80_SUBAA(Z80Context* ctx);
void Z80_SUBAB(Z80Context* ctx);
void Z80_SUBAC(Z80Context* ctx);
void Z80_SUBAD(Z80Context* ctx);
void Z80_SUBAE(Z80Context* ctx);
void Z80_SUBAH(Z80Context* ctx);
void Z80_SUBAL(Z80Context* ctx);
void Z80_SUBAHL(Z80Context* ctx);
void Z80_SUBAn(Z80Context* ctx);

void Z80_SBCAA(Z80Context* ctx);
void Z80_SBCAB(Z80Context* ctx);
void Z80_SBCAC(Z80Context* ctx);
void Z80_SBCAD(Z80Context* ctx);
void Z80_SBCAE(Z80Context* ctx);
void Z80_SBCAH(Z80Context* ctx);
void Z80_SBCAL(Z80Context* ctx);
void Z80_SBCAHL(Z80Context* ctx);
void Z80_SBCAn(Z80Context* ctx);

void Z80_ANDA(Z80Context* ctx);
void Z80_ANDB(Z80Context* ctx);
void Z80_ANDC(Z80Context* ctx);
void Z80_ANDD(Z80Context* ctx);
void Z80_ANDE(Z80Context* ctx);
void Z80_ANDH(Z80Context* ctx);
void Z80_ANDL(Z80Context* ctx);
void Z80_ANDHL(Z80Context* ctx);
void Z80_ANDn(Z80Context* ctx);

void Z80_ORA(Z80Context* ctx);
void Z80_ORB(Z80Context* ctx);
void Z80_ORC(Z80Context* ctx);
void Z80_ORD(Z80Context* ctx);
void Z80_ORE(Z80Context* ctx);
void Z80_ORH(Z80Context* ctx);
void Z80_ORL(Z80Context* ctx);
void Z80_ORHL(Z80Context* ctx);
void Z80_ORn(Z80Context* ctx);

void Z80_XORA(Z80Context* ctx);
void Z80_XORB(Z80Context* ctx);
void Z80_XORC(Z80Context* ctx);
void Z80_XORD(Z80Context* ctx);
void Z80_XORE(Z80Context* ctx);
void Z80_XORH(Z80Context* ctx);
void Z80_XORL(Z80Context* ctx);
void Z80_XORHL(Z80Context* ctx);
void Z80_XORn(Z80Context* ctx);

void Z80_CPA(Z80Context* ctx);
void Z80_CPB(Z80Context* ctx);
void Z80_CPC(Z80Context* ctx);
void Z80_CPD(Z80Context* ctx);
void Z80_CPE(Z80Context* ctx);
void Z80_CPH(Z80Context* ctx);
void Z80_CPLc(Z80Context* ctx);
void Z80_CPHL(Z80Context* ctx);
void Z80_CPn(Z80Context* ctx);

void Z80_INCA(Z80Context* ctx);
void Z80_INCB(Z80Context* ctx);
void Z80_INCC(Z80Context* ctx);
void Z80_INCD(Z80Context* ctx);
void Z80_INCE(Z80Context* ctx);
void Z80_INCH(Z80Context* ctx);
void Z80_INCL(Z80Context* ctx);
void Z80_INCHLL(Z80Context* ctx);

void Z80_DECA(Z80Context* ctx);
void Z80_DECB(Z80Context* ctx);
void Z80_DECC(Z80Context* ctx);
void Z80_DECD(Z80Context* ctx);
void Z80_DECE(Z80Context* ctx);
void Z80_DECH(Z80Context* ctx);
void Z80_DECL(Z80Context* ctx);
void Z80_DECHLL(Z80Context* ctx);

void Z80_ADDHLBC(Z80Context* ctx);
void Z80_ADDHLDE(Z80Context* ctx);
void Z80_ADDHLHL(Z80Context* ctx);
void Z80_ADDHLSP(Z80Context* ctx);

void Z80_ADDSPn(Z80Context* ctx);

void Z80_INCBC(Z80Context* ctx);
void Z80_INCDE(Z80Context* ctx);
void Z80_INCHL(Z80Context* ctx);
void Z80_INCSP(Z80Context* ctx);

void Z80_DECBC(Z80Context* ctx);
void Z80_DECDE(Z80Context* ctx);
void Z80_DECHL(Z80Context* ctx);
void Z80_DECSP(Z80Context* ctx);

// 0xCB
// Swaps
void Z80_SWAPA(Z80Context* ctx);
void Z80_SWAPB(Z80Context* ctx);
void Z80_SWAPC(Z80Context* ctx);
void Z80_SWAPD(Z80Context* ctx);
void Z80_SWAPE(Z80Context* ctx);
void Z80_SWAPH(Z80Context* ctx);
void Z80_SWAPL(Z80Context* ctx);
void Z80_SWAPHL(Z80Context* ctx);

// Bit rotation
void Z80_RLCAa(Z80Context* ctx);
void Z80_RLCA(Z80Context* ctx);
void Z80_RLCB(Z80Context* ctx);
void Z80_RLCC(Z80Context* ctx);
void Z80_RLCD(Z80Context* ctx);
void Z80_RLCE(Z80Context* ctx);
void Z80_RLCH(Z80Context* ctx);
void Z80_RLCL(Z80Context* ctx);
void Z80_RLCHL(Z80Context* ctx);

void Z80_RLAa(Z80Context* ctx);
void Z80_RLA(Z80Context* ctx);
void Z80_RLB(Z80Context* ctx);
void Z80_RLC(Z80Context* ctx);
void Z80_RLD(Z80Context* ctx);
void Z80_RLE(Z80Context* ctx);
void Z80_RLH(Z80Context* ctx);
void Z80_RLL(Z80Context* ctx);
void Z80_RLHL(Z80Context* ctx);

void Z80_RRCAa(Z80Context* ctx);
void Z80_RRCA(Z80Context* ctx);
void Z80_RRCB(Z80Context* ctx);
void Z80_RRCC(Z80Context* ctx);
void Z80_RRCD(Z80Context* ctx);
void Z80_RRCE(Z80Context* ctx);
void Z80_RRCH(Z80Context* ctx);
void Z80_RRCL(Z80Context* ctx);
void Z80_RRCHL(Z80Context* ctx);

void Z80_RRAa(Z80Context* ctx);
void Z80_RRA(Z80Context* ctx);
void Z80_RRB(Z80Context* ctx);
void Z80_RRC(Z80Context* ctx);
void Z80_RRD(Z80Context* ctx);
void Z80_RRE(Z80Context* ctx);
void Z80_RRH(Z80Context* ctx);
void Z80_RRL(Z80Context* ctx);
void Z80_RRHL(Z80Context* ctx);

void Z80_SLAA(Z80Context* ctx);
void Z80_SLAB(Z80Context* ctx);
void Z80_SLAC(Z80Context* ctx);
void Z80_SLAD(Z80Context* ctx);
void Z80_SLAE(Z80Context* ctx);
void Z80_SLAH(Z80Context* ctx);
void Z80_SLAL(Z80Context* ctx);
void Z80_SLAHL(Z80Context* ctx);

void Z80_SRAA(Z80Context* ctx);
void Z80_SRAB(Z80Context* ctx);
void Z80_SRAC(Z80Context* ctx);
void Z80_SRAD(Z80Context* ctx);
void Z80_SRAE(Z80Context* ctx);
void Z80_SRAH(Z80Context* ctx);
void Z80_SRAL(Z80Context* ctx);
void Z80_SRAHL(Z80Context* ctx);

void Z80_SRLA(Z80Context* ctx);
void Z80_SRLB(Z80Context* ctx);
void Z80_SRLC(Z80Context* ctx);
void Z80_SRLD(Z80Context* ctx);
void Z80_SRLE(Z80Context* ctx);
void Z80_SRLH(Z80Context* ctx);
void Z80_SRLL(Z80Context* ctx);
void Z80_SRLHL(Z80Context* ctx);

// Bit manipulation
void Z80_BIT0A(Z80Context* ctx);
void Z80_BIT1A(Z80Context* ctx);
void Z80_BIT2A(Z80Context* ctx);
void Z80_BIT3A(Z80Context* ctx);
void Z80_BIT4A(Z80Context* ctx);
void Z80_BIT5A(Z80Context* ctx);
void Z80_BIT6A(Z80Context* ctx);
void Z80_BIT7A(Z80Context* ctx);

void Z80_BIT0B(Z80Context* ctx);
void Z80_BIT1B(Z80Context* ctx);
void Z80_BIT2B(Z80Context* ctx);
void Z80_BIT3B(Z80Context* ctx);
void Z80_BIT4B(Z80Context* ctx);
void Z80_BIT5B(Z80Context* ctx);
void Z80_BIT6B(Z80Context* ctx);
void Z80_BIT7B(Z80Context* ctx);

void Z80_BIT0C(Z80Context* ctx);
void Z80_BIT1C(Z80Context* ctx);
void Z80_BIT2C(Z80Context* ctx);
void Z80_BIT3C(Z80Context* ctx);
void Z80_BIT4C(Z80Context* ctx);
void Z80_BIT5C(Z80Context* ctx);
void Z80_BIT6C(Z80Context* ctx);
void Z80_BIT7C(Z80Context* ctx);

void Z80_BIT0D(Z80Context* ctx);
void Z80_BIT1D(Z80Context* ctx);
void Z80_BIT2D(Z80Context* ctx);
void Z80_BIT3D(Z80Context* ctx);
void Z80_BIT4D(Z80Context* ctx);
void Z80_BIT5D(Z80Context* ctx);
void Z80_BIT6D(Z80Context* ctx);
void Z80_BIT7D(Z80Context* ctx);

void Z80_BIT0E(Z80Context* ctx);
void Z80_BIT1E(Z80Context* ctx);
void Z80_BIT2E(Z80Context* ctx);
void Z80_BIT3E(Z80Context* ctx);
void Z80_BIT4E(Z80Context* ctx);
void Z80_BIT5E(Z80Context* ctx);
void Z80_BIT6E(Z80Context* ctx);
void Z80_BIT7E(Z80Context* ctx);

void Z80_BIT0H(Z80Context* ctx);
void Z80_BIT1H(Z80Context* ctx);
void Z80_BIT2H(Z80Context* ctx);
void Z80_BIT3H(Z80Context* ctx);
void Z80_BIT4H(Z80Context* ctx);
void Z80_BIT5H(Z80Context* ctx);
void Z80_BIT6H(Z80Context* ctx);
void Z80_BIT7H(Z80Context* ctx);

void Z80_BIT0L(Z80Context* ctx);
void Z80_BIT1L(Z80Context* ctx);
void Z80_BIT2L(Z80Context* ctx);
void Z80_BIT3L(Z80Context* ctx);
void Z80_BIT4L(Z80Context* ctx);
void Z80_BIT5L(Z80Context* ctx);
void Z80_BIT6L(Z80Context* ctx);
void Z80_BIT7L(Z80Context* ctx);

void Z80_BIT0HL(Z80Context* ctx);
void Z80_BIT1HL(Z80Context* ctx);
void Z80_BIT2HL(Z80Context* ctx);
void Z80_BIT3HL(Z80Context* ctx);
void Z80_BIT4HL(Z80Context* ctx);
void Z80_BIT5HL(Z80Context* ctx);
void Z80_BIT6HL(Z80Context* ctx);
void Z80_BIT7HL(Z80Context* ctx);

void Z80_SET0A(Z80Context* ctx);
void Z80_SET1A(Z80Context* ctx);
void Z80_SET2A(Z80Context* ctx);
void Z80_SET3A(Z80Context* ctx);
void Z80_SET4A(Z80Context* ctx);
void Z80_SET5A(Z80Context* ctx);
void Z80_SET6A(Z80Context* ctx);
void Z80_SET7A(Z80Context* ctx);

void Z80_SET0B(Z80Context* ctx);
void Z80_SET1B(Z80Context* ctx);
void Z80_SET2B(Z80Context* ctx);
void Z80_SET3B(Z80Context* ctx);
void Z80_SET4B(Z80Context* ctx);
void Z80_SET5B(Z80Context* ctx);
void Z80_SET6B(Z80Context* ctx);
void Z80_SET7B(Z80Context* ctx);

void Z80_SET0C(Z80Context* ctx);
void Z80_SET1C(Z80Context* ctx);
void Z80_SET2C(Z80Context* ctx);
void Z80_SET3C(Z80Context* ctx);
void Z80_SET4C(Z80Context* ctx);
void Z80_SET5C(Z80Context* ctx);
void Z80_SET6C(Z80Context* ctx);
void Z80_SET7C(Z80Context* ctx);

void Z80_SET0D(Z80Context* ctx);
void Z80_SET1D(Z80Context* ctx);
void Z80_SET2D(Z80Context* ctx);
void Z80_SET3D(Z80Context* ctx);
void Z80_SET4D(Z80Context* ctx);
void Z80_SET5D(Z80Context* ctx);
void Z80_SET6D(Z80Context* ctx);
void Z80_SET7D(Z80Context* ctx);

void Z80_SET0E(Z80Context* ctx);
void Z80_SET1E(Z80Context* ctx);
void Z80_SET2E(Z80Context* ctx);
void Z80_SET3E(Z80Context* ctx);
void Z80_SET4E(Z80Context* ctx);
void Z80_SET5E(Z80Context* ctx);
void Z80_SET6E(Z80Context* ctx);
void Z80_SET7E(Z80Context* ctx);

void Z80_SET0H(Z80Context* ctx);
void Z80_SET1H(Z80Context* ctx);
void Z80_SET2H(Z80Context* ctx);
void Z80_SET3H(Z80Context* ctx);
void Z80_SET4H(Z80Context* ctx);
void Z80_SET5H(Z80Context* ctx);
void Z80_SET6H(Z80Context* ctx);
void Z80_SET7H(Z80Context* ctx);

void Z80_SET0L(Z80Context* ctx);
void Z80_SET1L(Z80Context* ctx);
void Z80_SET2L(Z80Context* ctx);
void Z80_SET3L(Z80Context* ctx);
void Z80_SET4L(Z80Context* ctx);
void Z80_SET5L(Z80Context* ctx);
void Z80_SET6L(Z80Context* ctx);
void Z80_SET7L(Z80Context* ctx);

void Z80_SET0HL(Z80Context* ctx);
void Z80_SET1HL(Z80Context* ctx);
void Z80_SET2HL(Z80Context* ctx);
void Z80_SET3HL(Z80Context* ctx);
void Z80_SET4HL(Z80Context* ctx);
void Z80_SET5HL(Z80Context* ctx);
void Z80_SET6HL(Z80Context* ctx);
void Z80_SET7HL(Z80Context* ctx);

void Z80_RES0A(Z80Context* ctx);
void Z80_RES1A(Z80Context* ctx);
void Z80_RES2A(Z80Context* ctx);
void Z80_RES3A(Z80Context* ctx);
void Z80_RES4A(Z80Context* ctx);
void Z80_RES5A(Z80Context* ctx);
void Z80_RES6A(Z80Context* ctx);
void Z80_RES7A(Z80Context* ctx);

void Z80_RES0B(Z80Context* ctx);
void Z80_RES1B(Z80Context* ctx);
void Z80_RES2B(Z80Context* ctx);
void Z80_RES3B(Z80Context* ctx);
void Z80_RES4B(Z80Context* ctx);
void Z80_RES5B(Z80Context* ctx);
void Z80_RES6B(Z80Context* ctx);
void Z80_RES7B(Z80Context* ctx);

void Z80_RES0C(Z80Context* ctx);
void Z80_RES1C(Z80Context* ctx);
void Z80_RES2C(Z80Context* ctx);
void Z80_RES3C(Z80Context* ctx);
void Z80_RES4C(Z80Context* ctx);
void Z80_RES5C(Z80Context* ctx);
void Z80_RES6C(Z80Context* ctx);
void Z80_RES7C(Z80Context* ctx);

void Z80_RES0D(Z80Context* ctx);
void Z80_RES1D(Z80Context* ctx);
void Z80_RES2D(Z80Context* ctx);
void Z80_RES3D(Z80Context* ctx);
void Z80_RES4D(Z80Context* ctx);
void Z80_RES5D(Z80Context* ctx);
void Z80_RES6D(Z80Context* ctx);
void Z80_RES7D(Z80Context* ctx);

void Z80_RES0E(Z80Context* ctx);
void Z80_RES1E(Z80Context* ctx);
void Z80_RES2E(Z80Context* ctx);
void Z80_RES3E(Z80Context* ctx);
void Z80_RES4E(Z80Context* ctx);
void Z80_RES5E(Z80Context* ctx);
void Z80_RES6E(Z80Context* ctx);
void Z80_RES7E(Z80Context* ctx);

void Z80_RES0H(Z80Context* ctx);
void Z80_RES1H(Z80Context* ctx);
void Z80_RES2H(Z80Context* ctx);
void Z80_RES3H(Z80Context* ctx);
void Z80_RES4H(Z80Context* ctx);
void Z80_RES5H(Z80Context* ctx);
void Z80_RES6H(Z80Context* ctx);
void Z80_RES7H(Z80Context* ctx);

void Z80_RES0L(Z80Context* ctx);
void Z80_RES1L(Z80Context* ctx);
void Z80_RES2L(Z80Context* ctx);
void Z80_RES3L(Z80Context* ctx);
void Z80_RES4L(Z80Context* ctx);
void Z80_RES5L(Z80Context* ctx);
void Z80_RES6L(Z80Context* ctx);
void Z80_RES7L(Z80Context* ctx);

void Z80_RES0HL(Z80Context* ctx);
void Z80_RES1HL(Z80Context* ctx);
void Z80_RES2HL(Z80Context* ctx);
void Z80_RES3HL(Z80Context* ctx);
void Z80_RES4HL(Z80Context* ctx);
void Z80_RES5HL(Z80Context* ctx);
void Z80_RES6HL(Z80Context* ctx);
void Z80_RES7HL(Z80Context* ctx);


Z80Opcode Z80OpcodeList[256];
Z80Opcode Z80CBOpcodeList[256];


#endif
