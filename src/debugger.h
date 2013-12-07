#ifndef H_DEBUGGER
#define H_DEBUGGER

#include "z80.h"
#include "surface.h"


typedef struct
{
    uint16_t* memOps;
    uint16_t memOpsSize;
    uint16_t opIndex;

    uint8_t breakpointEnabled;
    uint16_t breakpoint;

    Surface* imgBreakpoint;
} Debugger;


extern Debugger* dbg;


Debugger*  DBGInit();
void       DBGDestroy(Debugger* ctx);
void       DBGEnable(Debugger* ctx);
void       DBGFreeDisasm(Debugger* ctx);
void       DBGUpdate(Debugger* ctx, uint8_t* keyStates);
void       DBGRender(Debugger* ctx, Surface* screen);
void       DBGEventInput(Debugger* ctx, SDL_Event* event);
uint16_t*  DBGDisasm(Debugger* ctx, uint8_t* start, uint32_t size, uint16_t srcPC, uint16_t* listSize);


#endif
