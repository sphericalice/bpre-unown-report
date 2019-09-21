#pragma once

#include "global.h"
#include "main.h"
#include "gpu_regs.h"
#include "macro.h"
#include "scanline_effect.h"
#include "task.h"
#include "sprite.h"
#include "palette.h"
#include "alloc.h"
#include "bg.h"
#include "text_window.h"
#include "text.h"
#include "window.h"
#include "rgb.h"
#include "overworld.h"
#include "sound.h"
#include "menu.h"
#include "script.h"
#include "string_util.h"
#include "international_string_util.h"
#include "strings.h"
#include "event_data.h"
#include "decompress.h"
#include "pokemon.h"
#include "pokedex.h"
#include "battle.h"
#include "pokemon_icon.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "graphics/unown.c"

#define PAGE_NEXT 0
#define PAGE_PREV 1
#define UNOWN_PER_PAGE 8
#define MAX_PAGE_COUNT 10
#define currentPage data[0]

void Special_ShowUnownReport(void);
void PrintUnownReportText(u8 *text, u8 var1, u8 var2);
void Task_UnownReportFadeOut(u8 taskId);
void Task_UnownReportWaitForKeyPress(u8 taskId);
void SwapPage(u8 taskId, u8 SwapDirection);
void PrintReportPage(u8 PageNumber);
void PrintUnownList(u8 taskId, u8 PageNumber);
void PrintUnown(u8 form, u8 row, u8 col);
void DisplayUnownIcon(u8 form, u16 x, u16 y);
u32 UnownFormToPID(u8 form);
void PrintFirstPage(void);
s8 GetPageNumber(u8 taskId, u8 SwapDirection);
u8 GetPage(u8 taskId, u8 SwapDirection);
u8 UnownCount(void);
void atkF1_trysetcaughtmondexflags(void);
void SetCaughtUnown(u16 UnownForm);
u32 GetCaughtUnown(void);
void Task_UnownReportFadeIn(u8 taskId);
void MainCB2(void);
void CB2_ShowUnownReport(void);
void InitUnownReportDisplay(void);
void InitUnownReportWindow(void);
void PrintInstructionsBar(void);
void InitUnownReportBg(void);
void VBlankCB(void);

u8 *sTilemapBuffer;

static const struct BgTemplate sUnownReportBgTemplates[3] =
{
    { // Unown background
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    },
    { // Text
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 6,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
    { // Instructions
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 24,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    }
};

static const struct WindowTemplate sUnownReportWinTemplates[3] =
{
    { // Text
        .bg = 1,
        .tilemapLeft = 2,
        .tilemapTop = 0,
        .width = 26,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 1
    },
    { // Instructions
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 0,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};

static const u8 *UnownStrings[] = {
    gText_UnownA, gText_UnownB, gText_UnownC, gText_UnownD, gText_UnownE,
    gText_UnownF, gText_UnownG, gText_UnownH, gText_UnownI, gText_UnownJ,
    gText_UnownK, gText_UnownL, gText_UnownM, gText_UnownN, gText_UnownO,
    gText_UnownP, gText_UnownQ, gText_UnownR, gText_UnownS, gText_UnownT,
    gText_UnownU, gText_UnownV, gText_UnownW, gText_UnownX, gText_UnownY,
    gText_UnownZ, gText_UnownExclamationMark, gText_UnownQuestionMark
};

static const u8 *ReportStrings[] = {
    gText_Report1, gText_Report2, gText_Report3, gText_Report4, gText_Report5, gText_Report6
};

static const u16 ReportFlags[] = {
    FLAG_REPORT2, FLAG_REPORT3, FLAG_REPORT4, FLAG_REPORT5, FLAG_REPORT6
};
