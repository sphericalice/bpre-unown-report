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
#define FIRST_REPORT_PAGE 5
#define UNOWN_PER_PAGE 8
#define UNOWN_FORMS 28
#define currentPage data[0]

void sub_80A1184();
void sub_80CCB68();
void overworld_free_bgmaps();
void fade_screen(u8,u8);
TaskFunc sub_80A1CC0;
void set_bag_callback(void *);
void unknown_ItemMenu_Confirm(u8);
bool8 OpenedFromOW;

void atkF1_trysetcaughtmondexflags(void);
void PrintUnownReportText(u8 *text, u8 x, u8 y);
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

void sub_80A1184();
void sub_80CCB68();
void overworld_free_bgmaps();
void fade_screen(u8,u8);
TaskFunc sub_80A1CC0;
void set_bag_callback(void *);
void unknown_ItemMenu_Confirm(u8);

u8 *sTilemapBuffer;
bool8 OpenedFromOW;

static const struct BgTemplate sUnownReportBgTemplates[3] = {
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

static const struct WindowTemplate sUnownReportWinTemplates[3] = {
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
