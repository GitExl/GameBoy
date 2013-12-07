#include "renderlib.h"
#include "sound.h"
#include "z80.h"


SoundContext* snd;


SoundContext* SoundInit(uint32_t sampleRate)
{
    SoundContext* ctx = calloc(1, sizeof(SoundContext));

    ctx->sampleRate = sampleRate;
    ctx->clockRate = Z80_CLOCKSPEED / sampleRate;

    return ctx;
}

void SoundDestroy(SoundContext* ctx)
{
    free(ctx);
}

void SoundUpdate(SoundContext* ctx)
{
}
