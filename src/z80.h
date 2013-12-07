#ifndef H_Z80
#define H_Z80


#define Z80_CLOCKSPEED 1048576
#define Z80_FRAMESTEP  Z80_CLOCKSPEED / VID_SYNC


// Interrupt status
enum
{
    INTSTATE_DISABLED = 0,
    INTSTATE_ENABLED = 1,
    INTSTATE_ENABLENEXT = 2,
    INTSTATE_DISABLENEXT = 3,
};

// Interrupt flags
enum
{
    Z80INT_VBLANK = 0x01,
    Z80INT_LCDSTAT = 0x02,
    Z80INT_TIMER = 0x04,
    Z80INT_SERIAL = 0x08,
    Z80INT_JOYPAD = 0x10,
};


// A combined Z80 register
typedef union
{
    uint16_t r16;
    struct r8_t
    {
        uint8_t lo;
        uint8_t hi;
    } r8;
} Z80Register;


// Z80 cpu context
typedef struct
{
    // Combined registers
    // TODO: make use of these
    Z80Register AF;
    Z80Register BC;
    Z80Register DE;
    Z80Register HL;

    // Registers
    uint8_t A;
    uint8_t B;
    uint8_t C;
    uint8_t D;
    uint8_t E;

    uint8_t H;
    uint8_t L;
    uint8_t F;

    uint16_t PC;
    uint16_t SP;

    // State
    uint32_t cycleCount;
    uint8_t  cycles;
    uint8_t  halted;
    uint8_t  stopped;
    uint8_t  interruptStatus;
    uint16_t dmaTimer;

    // Timer
    uint32_t timermain;
    uint32_t timersub;
    uint32_t timerdiv;

    // Bugs
    uint8_t bugHALT;
} Z80Context;


extern Z80Context* cpu;


Z80Context* Z80Init();
void Z80Destroy(Z80Context* ctx);
void Z80Reset(Z80Context* ctx);
uint8_t Z80Exec(Z80Context* ctx, uint32_t cycles);

#endif
