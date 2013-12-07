#ifndef H_VRAMTOOL
#define H_VRAMTOOL

#include "surface.h"

typedef struct
{
} VRAMTool;


extern VRAMTool* vrt;


VRAMTool*  VRTInit();
void       VRTDestroy(VRAMTool* ctx);
void       VRTEnable(VRAMTool* ctx);
void       VRTUpdate(VRAMTool* ctx, uint8_t* keyStates);
void       VRTRender(VRAMTool* ctx, Surface* screen);
void       VRTEventInput(VRAMTool* ctx, SDL_Event* event);

#endif
