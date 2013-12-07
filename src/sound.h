#ifndef H_SOUND
#define H_SOUND

typedef struct
{
    uint8_t enabled;

    uint8_t sweepTime;
    uint8_t sweepDirection;
    uint8_t sweepShift;

    uint8_t waveDuty;
    uint8_t waveLength;

    uint8_t volume;
    uint8_t volumeSweep;
    uint8_t volumeSweepDir;

    uint16_t frequency;
    uint8_t initial;
    uint8_t stop;
} SquareChannel;

typedef struct
{
    uint8_t enabled;

    uint8_t length;
    uint8_t volume;

    uint8_t frequency;
    uint8_t initial;
    uint8_t stop;

    uint8_t* data;
} VoluntaryChannel;

typedef struct
{
    uint8_t enabled;

    uint8_t volume;
    uint8_t volumeSweep;

    uint8_t shiftClock;
    uint8_t counterStep;
    uint8_t divRatio;

    uint8_t initial;
    uint8_t stop;
} NoiseChannel;

typedef struct
{
    SquareChannel    channel1;
    SquareChannel    channel2;
    VoluntaryChannel channel3;
    NoiseChannel     channel4;

    uint8_t vinSO1;
    uint8_t vinSO2;

    uint8_t volumeSO1;
    uint8_t volumeSO2;

    uint32_t sampleRate;
    uint16_t clockRate;
    uint32_t clock;

    uint8_t outputFlags;
    uint8_t enabled;
} SoundContext;


extern SoundContext* snd;


SoundContext* SoundInit(uint32_t sampleRate);
void SoundDestroy(SoundContext* ctx);
void SoundUpdate(SoundContext* ctx);

#endif

