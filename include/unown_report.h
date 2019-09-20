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
#include "decompress.h"
#include "pokemon_icon.h"
#include "constants/songs.h"
#include "graphics/unown.c"

u8 UnownCount();
static u8 GetPage(u8 PageNumber, u8 SwapDirection);
static void PrintUnownList(u8 PageNumber);
static void PrintReportPage(u8 PageNumber);
static void PrintFirstPage();
static s8 GetPageNumber(u8 taskId, u8 SwapDirection);
static void SwapPage(u8 taskId, u8 SwapDirection);
void CB2_UnownReport();
void UnownReport();
void sub_80CD240(void);
void dp13_810BB8C();
static void MainCB2(void);
static void Task_UnownReportFadeIn(u8);
static void Task_UnownReportWaitForKeyPress(u8);
static void Task_UnownReportFadeOut(u8);
static void DisplayUnownReportText(void);
static void DisplayUnownIcon(u8 form, u16 x, u16 y);
static void InitUnownReportBg(void);
static void InitUnownReportWindow(void);
static void PrintUnownReportText(u8 *, u8, u8);

#define MAX_PAGE_COUNT 10
#define UNOWN_PER_PAGE 8
#define PAGE_NEXT 0
#define PAGE_PREV 1
#define currentPage data[0]
#define PKMN_UNOWN 201

u8 *sTilemapBuffer;

static const u8 sTextColors[2][3] = { {0, 2, 3}, {15, 14, 13} };

static const struct BgTemplate sUnownReportBgTemplates[2] =
{
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0,
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 6,
        .screenSize = 1,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0,
    },
};

static const struct WindowTemplate sUnownReportWinTemplates[2] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE,
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
