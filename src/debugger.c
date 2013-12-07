#include "renderlib.h"
#include "render.h"
#include "font.h"
#include "utils.h"
#include "z80.h"
#include "z80opcodes.h"
#include "mmu.h"
#include "display.h"
#include "debugger.h"


extern Font* defaultFont;
extern uint8_t mode;
extern uint8_t modeRender;

Debugger* dbg = NULL;


Debugger* DBGInit()
{
    Debugger* ctx = calloc(1, sizeof(Debugger));

    ctx->breakpointEnabled = 0;
    ctx->breakpoint = 0xFF84;

    ctx->imgBreakpoint = surfaceReadFromPNG("images/breakpoint.png");

    return ctx;
}

void DBGDestroy(Debugger* ctx)
{
    DBGFreeDisasm(ctx);

    surfaceDestroy(ctx->imgBreakpoint);

    free(ctx);
}

void DBGUpdate(Debugger* ctx, uint8_t* keyStates)
{
}

#define UI_TOPDIST 26
#define UI_LINEDIST 15
#define UI_COL0 16
#define UI_COL1 64
#define UI_COL2 144
#define UI_COL3 188
#define UI_INFO 360

#define DISASM_LEN 35

void DBGRender(Debugger* ctx, Surface* screen)
{
    uint8_t i = 0;
    char* string = NULL;
    Z80Opcode* op = NULL;
    uint8_t opIdx = 0;
    uint16_t pc = 0;
    uint8_t opCB = 0;
    int32_t startOffset = ctx->opIndex - (DISASM_LEN / 2);


    string = malloc(32);

    // Render disassembly
    if (startOffset < 0)
        startOffset = 0;

    // Columns
    renderBoxFill(screen, UI_COL0 - 4, 16, UI_COL1 - UI_COL0 - 8, screen->Height - 32, RGBA(0, 0, 0, 255), BLEND_ALPHA50);
    renderBoxFill(screen, UI_COL1 - 4, 16, UI_COL2 - UI_COL1 - 8, screen->Height - 32, RGBA(0, 0, 0, 255), BLEND_ALPHA50);
    renderBoxFill(screen, UI_COL2 - 4, 16, 180, screen->Height - 32, RGBA(0, 0, 0, 255), BLEND_ALPHA50);

    for (i = 0; i < DISASM_LEN; i++)
    {
        pc = ctx->memOps[i + startOffset];

        // Highlight
        if (pc == cpu->PC)
            renderBoxFill(screen, 0, UI_TOPDIST + i * UI_LINEDIST - 1, 320, UI_LINEDIST, TOOLC_ACTIVE, BLEND_ALPHA50);
        if (i + startOffset == ctx->opIndex)
            renderBoxFill(screen, 0, UI_TOPDIST + i * UI_LINEDIST - 1, 320, UI_LINEDIST, TOOLC_HIGHLIGHT, BLEND_ALPHA50);

        // Breakpoints
        if (ctx->breakpointEnabled && ctx->breakpoint == pc)
            renderBlitAlphaFast(screen, ctx->imgBreakpoint, 1, UI_TOPDIST + i * UI_LINEDIST - 2, BLEND_ALPHA);

        // Fetch opcode
        opIdx = mmu->memory[pc];
        if (opIdx == 0xCB)
        {
            opIdx = mmu->memory[++pc];
            op = &Z80CBOpcodeList[opIdx];
            opCB = 1;
        }
        else
        {
            op = &Z80OpcodeList[opIdx];
            opCB = 0;
        }

        // PC
        snprintf(string, 32, "%04X\0", pc);
        renderText(screen, defaultFont, UI_COL0, UI_TOPDIST + i * UI_LINEDIST, string, TOOLC_TEXT);

        // Opcode
        if (opCB == 1)
            snprintf(string, 32, "CB %02X", opIdx);
        else
        {
            if (op->length == 0)
                snprintf(string, 32, "%02X", opIdx);
            else if (op->length == 1)
                snprintf(string, 32, "%02X %02X", opIdx, mmu->memory[pc + 1]);
            else if (op->length == 2)
                snprintf(string, 32, "%02X %02X %02X", opIdx, mmu->memory[pc + 1], mmu->memory[pc + 2]);
        }
        renderText(screen, defaultFont, UI_COL1, UI_TOPDIST + i * UI_LINEDIST, string, TOOLC_TEXT);

        // Instruction
        snprintf(string, 32, "%-4s\0", op->instruction);
        renderText(screen, defaultFont, UI_COL2, UI_TOPDIST + i * UI_LINEDIST, string, TOOLC_TEXT);

        // Parameters
        if (op->parameters)
        {
            if (op->length == 0)
                snprintf(string, 32, op->parameters);
            else if (op->length == 1)
                snprintf(string, 32, op->parameters, mmu->memory[pc + 1]);
            else if (op->length == 2)
                snprintf(string, 32, op->parameters, COMB16(mmu->memory[pc + 2], mmu->memory[pc + 1]));
            renderText(screen, defaultFont, UI_COL3, UI_TOPDIST + i * UI_LINEDIST, string, TOOLC_TEXT);
        }
    }

    // Registers
    snprintf(string, 32, "AF $%02X%02X\0", cpu->A, cpu->F);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 0, string, TOOLC_TEXT);
    snprintf(string, 32, "BC $%02X%02X\0", cpu->B, cpu->C);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 1, string, TOOLC_TEXT);
    snprintf(string, 32, "DE $%02X%02X\0", cpu->D, cpu->E);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 2, string, TOOLC_TEXT);
    snprintf(string, 32, "HL $%02X%02X\0", cpu->H, cpu->L);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 3, string, TOOLC_TEXT);

    // CPU state
    snprintf(string, 32, "PC $%04X\0", cpu->PC);
    renderText(screen, defaultFont, UI_INFO + 128, UI_TOPDIST + UI_LINEDIST * 0, string, TOOLC_TEXT);
    snprintf(string, 32, "SP $%04X\0", cpu->SP);
    renderText(screen, defaultFont, UI_INFO + 128, UI_TOPDIST + UI_LINEDIST * 1, string, TOOLC_TEXT);

    // Flags
    snprintf(string, 32, "%s Zero\0", cpu->F & FLAG_ZERO ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO + 128, UI_TOPDIST + UI_LINEDIST * 3, string, TOOLC_TEXT);
    snprintf(string, 32, "%s Negative\0", cpu->F & FLAG_NEG ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO + 128, UI_TOPDIST + UI_LINEDIST * 4, string, TOOLC_TEXT);
    snprintf(string, 32, "%s Half carry\0", cpu->F & FLAG_HALF ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO + 128, UI_TOPDIST + UI_LINEDIST * 5, string, TOOLC_TEXT);
    snprintf(string, 32, "%s Carry\0", cpu->F & FLAG_CARRY ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO + 128, UI_TOPDIST + UI_LINEDIST * 6, string, TOOLC_TEXT);

    // Interrupts
    uint8_t intMaster = mmu->memory[0xFF00 + IO_INTENABLE];
    uint8_t intWaiting = mmu->memory[0xFF00 + IO_INTFLAGS];
    snprintf(string, 32, "        M  W\0");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 8, string, TOOLC_TEXT);
    snprintf(string, 32, "VSync   %s  %s\0", intMaster & Z80INT_VBLANK ? "X" : " ", intWaiting & Z80INT_VBLANK ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 9, string, TOOLC_TEXT);
    snprintf(string, 32, "Joypad  %s  %s\0", intMaster & Z80INT_JOYPAD ? "X" : " ", intWaiting & Z80INT_JOYPAD ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 10, string, TOOLC_TEXT);
    snprintf(string, 32, "LCDST   %s  %s\0", intMaster & Z80INT_LCDSTAT ? "X" : " ", intWaiting & Z80INT_LCDSTAT ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 11, string, TOOLC_TEXT);
    snprintf(string, 32, "Timer   %s  %s\0", intMaster & Z80INT_TIMER ? "X" : " ", intWaiting & Z80INT_TIMER ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 12, string, TOOLC_TEXT);
    snprintf(string, 32, "Serial  %s  %s\0", intMaster & Z80INT_SERIAL ? "X" : " ", intWaiting & Z80INT_SERIAL ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 13, string, TOOLC_TEXT);
    snprintf(string, 32, "Master  %s\0", cpu->interruptStatus ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 14, string, TOOLC_TEXT);

    // LCD control
    uint8_t intControl = mmu->memory[0xFF00 + IO_LCDCONTROL];
    snprintf(string, 32, "LCD display    %s\0", intControl & LCD_DISPLAY ? "ON" : "OFF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 16, string, TOOLC_TEXT);
    snprintf(string, 32, "Window tilemap %s\0", intControl & LCD_WINDOWTILEMAP ? "$9C00-$9FFF" : "$9800-$9BFF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 17, string, TOOLC_TEXT);
    snprintf(string, 32, "Window display %s\0", intControl & LCD_DISPLAYWINDOW ? "ON" : "OFF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 18, string, TOOLC_TEXT);
    snprintf(string, 32, "BG/window gfx  %s\0", intControl & LCD_BGWTILEGFX ? "$8000-$8FFF" : "$8800-$97FF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 19, string, TOOLC_TEXT);
    snprintf(string, 32, "BG tilemap     %s\0", intControl & LCD_BGTILEMAP ? "$9C00-$9FFF" : "$9800-$9BFF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 20, string, TOOLC_TEXT);
    snprintf(string, 32, "Sprite size    %s\0", intControl & LCD_SPRITESIZE ? "8x16" : "8x8");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 21, string, TOOLC_TEXT);
    snprintf(string, 32, "Sprite display %s\0", intControl & LCD_DISPLAYSPRITES ? "ON" : "OFF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 22, string, TOOLC_TEXT);
    snprintf(string, 32, "BG display     %s\0", intControl & LCD_DISPLAYBG ? "ON" : "OFF");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 23, string, TOOLC_TEXT);

    snprintf(string, 32, "Keypad   $%02X $%02X\0", mmu->keyRows[0], mmu->keyRows[1]);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 25, string, TOOLC_TEXT);
    snprintf(string, 32, "Timer    $%02X\\$%02X\0", mmu->io[IO_TMR_TAC], mmu->io[IO_TMR_TIMA]);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 26, string, TOOLC_TEXT);
    snprintf(string, 32, "LCD Y    %i\0", disp->line);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 27, string, TOOLC_TEXT);
    snprintf(string, 32, "Window   %i, %i\0", mmu->io[IO_WINDOWX], mmu->io[IO_WINDOWY]);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 28, string, TOOLC_TEXT);

    uint8_t lcdStat = mmu->io[IO_LCDSTATUS];
    snprintf(string, 32, "LCDY int   %s\0", (lcdStat & LCDST_INT_CDC) ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 30, string, TOOLC_TEXT);
    snprintf(string, 32, "OAM int    %s\0", (lcdStat & LCDST_INT_OAM) ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 31, string, TOOLC_TEXT);
    snprintf(string, 32, "VBLANK int %s\0", (lcdStat & LCDST_INT_VBLANK) ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 32, string, TOOLC_TEXT);
    snprintf(string, 32, "HBLANK int %s\0", (lcdStat & LCDST_INT_HBLANK) ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 33, string, TOOLC_TEXT);
    snprintf(string, 32, "Y coincid. %s\0", (lcdStat & LCDST_CDC) ? "X" : " ");
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 34, string, TOOLC_TEXT);
    snprintf(string, 32, "LCD mode   %i\0", lcdStat & LCDST_MODE_MASK);
    renderText(screen, defaultFont, UI_INFO, UI_TOPDIST + UI_LINEDIST * 35, string, TOOLC_TEXT);

    free(string);

    modeRender = 0;
}

void DBGEventInput(Debugger* ctx, SDL_Event* event)
{
    uint16_t keyCode = event->key.keysym.sym;
    switch (event->type)
    {
        case SDL_KEYDOWN:
            if (keyCode == SDLK_MINUS)
            {
                if (ctx->opIndex - DISASM_LEN < 0)
                    ctx->opIndex = 0;
                else
                    ctx->opIndex -= 0x500;
            }
            else if (keyCode == SDLK_PLUS)
            {
                if (ctx->opIndex + DISASM_LEN + DISASM_LEN > 0xFFFF)
                    ctx->opIndex = 0xFFFF;
                else
                    ctx->opIndex += 0x500;
            }

            if (keyCode == SDLK_PAGEUP)
            {
                if (ctx->opIndex - DISASM_LEN < 0)
                    ctx->opIndex = 0;
                else
                    ctx->opIndex -= DISASM_LEN;
            }
            else if (keyCode == SDLK_PAGEDOWN)
            {
                if (ctx->opIndex + DISASM_LEN + DISASM_LEN > 0xFFFF)
                    ctx->opIndex = 0xFFFF;
                else
                    ctx->opIndex += DISASM_LEN;
            }

            else if (keyCode == SDLK_UP && ctx->opIndex > 0)
                ctx->opIndex--;
            else if (keyCode == SDLK_DOWN && ctx->opIndex < 0xFFFF)
                ctx->opIndex++;

            else if (keyCode == SDLK_s)
                mode = MODE_GAMEBOY_STEP;
            else if (keyCode == SDLK_RETURN)
                mode = MODE_GAMEBOY;

            else if (keyCode == SDLK_SPACE)
            {
                uint16_t pc = ctx->memOps[ctx->opIndex];
                if (ctx->breakpointEnabled && ctx->breakpoint == pc)
                    ctx->breakpointEnabled = 0;
                else
                {
                    ctx->breakpointEnabled = 1;
                    ctx->breakpoint = pc;
                }
            }

            modeRender = 1;
            break;
        default:
            break;
    }
}

void DBGEnable(Debugger* ctx)
{
    DBGFreeDisasm(ctx);

    // Disassemble PC start locations in executable memory areas
    ctx->memOps = DBGDisasm(ctx, mmu->memory, 0x10000, cpu->PC, &ctx->memOpsSize);

    modeRender = 1;
}

void DBGFreeDisasm(Debugger* ctx)
{
    free(ctx->memOps);
}

uint16_t* DBGDisasm(Debugger* ctx, uint8_t* data, uint32_t size, uint16_t srcPC, uint16_t* listSize)
{
    uint32_t pc;
    Z80Opcode* op;
    uint8_t opIdx = 0;
    uint16_t* opList = calloc(size, sizeof(uint16_t));
    uint16_t listIndex = 0;

    pc = 0;
    while(pc < size)
    {
        // If this is the current emulation PC, focus the view on it
        if (pc == srcPC)
            ctx->opIndex = listIndex;

        opList[listIndex] = pc;
        listIndex++;

        opIdx = data[pc];
        if (opIdx == 0xCB)
            op = &Z80CBOpcodeList[data[++pc]];
        else
            op = &Z80OpcodeList[opIdx];

        // If we pass the current Program Counter, restart at the Program Counter instead
        if (pc < srcPC && pc + op->length + 1 > srcPC)
            pc = srcPC;
        else
            pc += op->length + 1;
    }

    *listSize = listIndex;
    return opList;
}
