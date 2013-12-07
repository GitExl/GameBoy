#ifndef H_MBC
#define H_MBC

#define MBC_SWITCHMODE_RAM 0
#define MBC_SWITCHMODE_ROM 1

typedef struct
{
    uint8_t type;

    uint32_t romSize;
    uint8_t romBank;

    uint32_t ramSize;
    uint8_t* ram;
    uint8_t ramBank;
    uint8_t ramEnabled;

    uint8_t battery;
    uint8_t timer;
    uint8_t rumble;

    uint8_t switchMode;
} MBCContext;


extern MBCContext* mbc;


MBCContext* MBCInit();
void        MBCDestroy(MBCContext* ctx);
void        MBCWrite(MBCContext* ctx, uint16_t address, uint8_t data);
uint8_t     MBCReadRAM(MBCContext* ctx, uint16_t address);
void        MBCWriteRAM(MBCContext* ctx, uint16_t address, uint8_t data);

#endif
