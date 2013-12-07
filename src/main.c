#include "renderlib.h"
#include "render.h"
#include "timer.h"
#include "utils.h"
#include "z80.h"
#include "mmu.h"
#include "mbc.h"
#include "sound.h"
#include "display.h"
#include "debugger.h"
#include "vramtool.h"

#include <SDL/SDL.h>


uint8_t  videoScale = 4;
uint32_t videoWidth = 160;
uint32_t videoHeight = 144;

Font*     defaultFont = NULL;
uint8_t   mode = MODE_GAMEBOY_STEP;
uint8_t   modeRender = 1;

int file_exists(char*);


int main(int argc, char* argv[])
{
    SDL_Surface* screen = NULL;
    Surface*     backBuffer = NULL;

    // Framerate counter
    uint64_t frameTiming = 0;
    uint32_t frameCount = 0;
    uint32_t FPS = 0;

    // Frame sync
    uint64_t traceTiming = 0;
    int64_t  timeElapsed = 0;

    // State
    uint8_t breakLoop = 0;

    // Keyboard
    int32_t  keyCount;
    uint8_t* keyStates;
    uint16_t keyCode;

    // Mouse
    uint8_t mouseButtons = 0;
    int32_t mouseX = 0;
    int32_t mouseY = 0;

    // SDL
    SDL_Event event;
    char      vidDriverName[32];


    freopen("CON", "w", stdout);
    freopen("CON", "w", stderr);

    if (argc <= 1) {
        printf("No ROM file specified.\n");
        return 0;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        printf("SDL: Failed to initialize SDL.\n");
        return 0;
    }
    else
        printf("SDL: Initialized.\n");

    // Set video mode
    if (!SDL_SetVideoMode(videoWidth * videoScale, videoHeight * videoScale, 32, SDL_HWSURFACE | SDL_ASYNCBLIT))
    {
        printf("SDL: Unable to set video mode.\n");
        return 0;
    }
    else
        printf("SDL: Video initialized (%dx%dx%d).\n", videoWidth, videoHeight, 32);
    SDL_WM_SetCaption("GB", NULL);

    // Enumerate SDL display info
    if (SDL_VideoDriverName(vidDriverName, 32))
        printf("SDL: Video driver: %s\n", vidDriverName);

    // Timer
    timerInit();

    // Setup rendering
    screen = SDL_GetVideoSurface();
    backBuffer = surfaceCreate(screen->w, screen->h);
    defaultFont = fontLoad("fonts/bitstream_mono.png");

    // Allocate space for keystates
    SDL_GetKeyState(&keyCount);
    keyStates = calloc(1, keyCount);

    // Setup emulation contexts
    mbc = MBCInit();
    mmu = MMUInit();
    disp = DisplayInit(screen);
    cpu = Z80Init();
    snd = SoundInit(48000);
    dbg = DBGInit();
    vrt = VRTInit();

    // Load and reset
    if (MMULoadCartridge(mmu, argv[1]))
        return 0;
    Z80Reset(cpu);


    // Update loop
    while(!breakLoop)
    {
        traceTiming = timerTicks();

        // Process SDL events
        while (SDL_PollEvent(&event))
        {
            if (mode == MODE_DEBUG)
                DBGEventInput(dbg, &event);

            keyCode = event.key.keysym.sym;
            switch (event.type)
            {
                // Key states
                case SDL_KEYDOWN:
                    keyStates[keyCode] = 1;

                    // Mode switching
                    if (keyCode == SDLK_ESCAPE)
                    {
                        if (mode == MODE_GAMEBOY)
                            breakLoop = 1;
                        else
                            mode = MODE_GAMEBOY;
                    }
                    else if (keyCode == SDLK_F1)
                    {
                        DBGEnable(dbg);
                        mode = MODE_DEBUG;
                    }
                    else if (keyCode == SDLK_F2)
                    {
                        VRTEnable(vrt);
                        mode = MODE_VRAM_TILES;
                    }
                    else if (keyCode == SDLK_F3)
                        mode = MODE_VRAM_MAPS;
                    else if (keyCode == SDLK_F4)
                        mode = MODE_VRAM_SPRITES;
                    break;

                case SDL_KEYUP:
                    keyStates[keyCode] = 0;
                    break;

                // Mouse button down
                case SDL_MOUSEBUTTONDOWN:
                    mouseButtons |= 1 << (event.button.button - 1);
                    break;

                // Mouse button up
                case SDL_MOUSEBUTTONUP:
                    mouseButtons &= ~1 << (event.button.button - 1);
                    break;

                // Mouse motion
                case SDL_MOUSEMOTION:
                    mouseX = (event.button.x) / videoScale;
                    mouseY = (event.button.y) / videoScale;
                    break;

                // Misc.
                case SDL_QUIT:
                    breakLoop = 1;
                    break;

                default:
                    break;
            }
        }

        // Toolset modes
        if (mode & MODE_TOOLSET)
        {
            if (modeRender)
            {
                // Render gameboy screen
                renderBlit(backBuffer, disp->screen, 0, 0, screen->w, screen->h);

                // Toolset background
                renderBoxFill(backBuffer, 0, 0, backBuffer->Width, backBuffer->Height, RGBA(0, 0, 0, 208), BLEND_ALPHA);

                // Mode header
                renderBoxFill(backBuffer, (mode - MODE_TOOLSET) * 104, 0, 104, 16, TOOLC_ACTIVE, BLEND_SOLID);
                renderText(backBuffer, defaultFont, 4, 2, "F1 Debug", TOOLC_TEXT);
                renderText(backBuffer, defaultFont, 108, 2, "F2 Tiles", TOOLC_TEXT);
                renderText(backBuffer, defaultFont, 212, 2, "F3 Maps", TOOLC_TEXT);
                renderText(backBuffer, defaultFont, 316, 2, "F4 Sprites", TOOLC_TEXT);

                renderBoxFill(backBuffer, 0, backBuffer->Height - 16, backBuffer->Width, 24, TOOLC_ACTIVE, BLEND_SOLID);
            }

            if (mode == MODE_DEBUG)
            {
                DBGUpdate(dbg, keyStates);
                if (modeRender)
                    DBGRender(dbg, backBuffer);
            }
            else if (mode == MODE_VRAM_TILES)
            {
                VRTUpdate(vrt, keyStates);
                if (modeRender)
                    VRTRender(vrt, backBuffer);
            }

            // Copy screen image
            surfaceCopyToSDL(backBuffer, screen);
        }

        // Gameboy modes
        else
        {
            if (mode == MODE_GAMEBOY)
            {
                // Gameboy input
                if (mode == MODE_GAMEBOY)
                {
                    mmu->keyRows[0] = 0xEF;
                    if (keyStates[SDLK_RETURN])
                        mmu->keyRows[0] &= ~0x08;
                    if (keyStates[SDLK_BACKSPACE])
                        mmu->keyRows[0] &= ~0x04;
                    if (keyStates[SDLK_x])
                        mmu->keyRows[0] &= ~0x02;
                    if (keyStates[SDLK_z])
                        mmu->keyRows[0] &= ~0x01;

                    mmu->keyRows[1] = 0xEF;
                    if (keyStates[SDLK_DOWN])
                        mmu->keyRows[1] &= ~0x08;
                    else if (keyStates[SDLK_UP])
                        mmu->keyRows[1] &= ~0x04;
                    if (keyStates[SDLK_LEFT])
                        mmu->keyRows[1] &= ~0x02;
                    else if (keyStates[SDLK_RIGHT])
                        mmu->keyRows[1] &= ~0x01;

                    // Joypad interrupt
                    // Occurs when joypad input is requested through IO_JOYPAD,
                    // and when any of the requested buttons are actually pressed.
                    // Also occurs when IO_JOYPAD is 0 and any button is pressed.
                    if ((mmu->keyRows[0] != 0xEF && mmu->io[IO_JOYPAD] == 0x10) ||
                        (mmu->keyRows[1] != 0xEF && mmu->io[IO_JOYPAD] == 0x20) ||
                        ((mmu->keyRows[0] != 0xEF || mmu->keyRows[1] != 0xEF) && mmu->io[IO_JOYPAD] == 0))
                    {
                        mmu->io[IO_INTFLAGS] |= Z80INT_JOYPAD;

                        // Resume from STOP
                        if(cpu->stopped)
                        {
                            cpu->stopped = 0;
                            mmu->io[IO_LCDCONTROL] |= LCD_DISPLAY;
                        }
                    }
                }

                // Execute frame
                if (Z80Exec(cpu, Z80_FRAMESTEP))
                {
                    DBGEnable(dbg);
                    mode = MODE_DEBUG;
                }

                // Output gameboy frame
                else
                    surfaceCopyToSDLQuadruple(disp->screen, screen);
            }
            else if (mode == MODE_GAMEBOY_STEP)
            {
                mmu->keyRows[0] = 0xEF;
                mmu->keyRows[1] = 0xEF;

                Z80Exec(cpu, 1);
                DBGEnable(dbg);
                mode = MODE_DEBUG;
            }
        }

        // Present
        SDL_Flip(screen);

        // Framerate counter
        /*frameCount++;
        if(timerTicks() - frameTiming >= 1000000)
        {
            frameTiming = timerTicks();

            //printf("%d FPS, %4.2fms\n", FPS, (float)timeElapsed / 1000);

            FPS = frameCount;
            frameCount = 0;
        }*/

        // Delay for framerate limiting
        timeElapsed = timerTicks() - traceTiming;
        if (timeElapsed > 0)
            timerDelay(VID_RATE - timeElapsed);
    }


    // Cleanup
    DBGDestroy(dbg);
    DisplayDestroy(disp);
    SoundDestroy(snd);
    Z80Destroy(cpu);
    MMUDestroy(mmu);
    MBCDestroy(mbc);

    fontDestroy(defaultFont);

    surfaceDestroy(backBuffer);
    SDL_FreeSurface(screen);
    SDL_Quit();

    return 0;
}
