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
#include "string_util.h"
#include "international_string_util.h"
#include "strings.h"
#include "pokemon_icon.h"
#include "constants/songs.h"
#include "graphics/unown.c"

static void SetDispcntReg(void);
static void ClearVramOamPlttRegs(void);
static void ClearTasksAndGraphicalStructs(void);
static void ResetBgCoordinates(void);
static void LoadUnownReportWindowGfx(u8 bgId);
static void VblankCB_UnownReport(void);
static void RemoveUnownReportWindow(u8 windowId);
static void Task_ExitUnownReport(u8 taskId);
static void Task_BeginPaletteFade(u8 taskId);
static void Task_UnownReportWaitForPaletteFade(u8 taskId);
static void MainCB2_UnownReport(void);
u8 UnownCount();
static void PrintOnUnownReportWindow(void);
static u8 GetPage(u8 PageNumber, u8 SwapDirection);
static void PrintUnownList(u8 PageNumber);
static void PrintReportPage(u8 PageNumber);
static void PrintFirstPage();
static s8 GetPageNumber(u8 taskId, u8 SwapDirection);
static void SwapPage(u8 taskId, u8 SwapDirection);
static void Task_CheckUnownReportInput(u8 taskId);
void CB2_UnownReport();
void UnownReport();
void sub_80CD240(void);
void dp13_810BB8C();

#define MAX_PAGE_COUNT 9
#define UNOWN_PER_PAGE 10
#define PAGE_NEXT 0
#define PAGE_PREV 1
#define currentPage data[0]

u8 *sTilemapBuffer;

static const u8 sTextColors[2][3] = { {0, 2, 3}, {15, 14, 13} };
static const struct BgTemplate sUnownReportBgTemplates[] = { { 0, 0, 31, 0, 0, 0, 0 }, { 3, 1, 30, 0, 0, 3, 0 } };
static const struct WindowTemplate sUnownReportWindowTemplates[] = { { 0, 2, 1, 26, 18, 15, 20 }, DUMMY_WIN_TEMPLATE };
static const u8 *UnownStrings[] = {
    gText_UnownA, gText_UnownB, gText_UnownC, gText_UnownD, gText_UnownE,
    gText_UnownF, gText_UnownG, gText_UnownH, gText_UnownI, gText_UnownJ,
    gText_UnownK, gText_UnownL, gText_UnownM, gText_UnownN, gText_UnownO,
    gText_UnownP, gText_UnownQ, gText_UnownR, gText_UnownS, gText_UnownT,
    gText_UnownU, gText_UnownV, gText_UnownW, gText_UnownX, gText_UnownY,
    gText_UnownZ, gText_UnownQuestionMark, gText_UnownExclamationMark
};
static const u8 *ReportStrings[] = {
    gText_Report1, gText_Report2, gText_Report3, gText_Report4, gText_Report5, gText_Report6
};
