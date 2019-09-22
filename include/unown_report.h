#ifndef GUARD_UNOWN_REPORT_H
#define GUARD_UNOWN_REPORT_H

#define PAGE_NEXT 0
#define PAGE_PREV 1
#define FRONT_PAGE 0
#define FIRST_UNOWN_LIST_PAGE 1
#define SECOND_UNOWN_LIST_PAGE 2
#define THIRD_UNOWN_LIST_PAGE 3
#define LAST_UNOWN_LIST_PAGE 4
#define FIRST_REPORT_PAGE 5
#define UNOWN_PER_PAGE 8
#define UNOWN_FORMS 28
#define currentPage data[0]
#define bitToStartFrom data

u8 *sTilemapBuffer;
bool8 sOpenedFromOW;

void SetCaughtUnown(u16 UnownForm);
void atkF1_TrySetCaughtMonDexFlags(void);
u32 UnownFormToPID(u8 form);
void PrintUnownIcon(u8 form, u16 x, u16 y);
void PrintUnown(u8 form, u8 row, u8 col);
u32 GetCaughtUnown(void);
bool32 UnownFormIsCaught(u8 form);
u8 UnownCount(void);
void PrintUnownReportText(const u8 *text, u8 x, u8 y);
void PrintReportPage(u8 ReportPageNumber);
void PrintUnownList(u8 taskId, u8 PageNumber);
void PrintFrontPage(void);
u8 GetNewPage(u8 taskId, u8 SwapDirection);
void SwapPage(u8 taskId, u8 SwapDirection);
void Task_UnownReportFadeOut(u8 taskId);
void Task_UnownReportWaitForKeyPress(u8 taskId);
void Task_UnownReportFadeIn(u8 taskId);
void PrintInstructionsBar(void);
void MainCB2(void);
void VBlankCB(void);
void InitUnownReportFrontPage(void);
void InitUnownReportWindow(void);
void InitUnownReportBg(void);
void CB2_ShowUnownReport(void);
void UnownReport_Execute(bool8 src);
void Task_UnownReportFromOW(u8 taskId);
void StartUnownReportFromBag(void);
void ItemUseOutOfBattle_UnownReport(u8 taskId);

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
        .tilemapLeft = 4,
        .tilemapTop = 6,
        .width = 22,
        .height = 11,
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

#endif // GUARD_UNOWN_REPORT_H