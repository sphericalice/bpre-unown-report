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
void MainCB2_UnownReport(void);
void VBlankCB_UnownReport(void);
void InitUnownReportFrontPage(void);
void LoadUnownReportGfx(void);
void ClearTasksAndGraphicalStructs(void);
void ClearVramOamPlttRegs(void);
void CB2_UnownReport(void);
void UnownReport_Execute(bool8 src);
void Task_UnownReportFromOW(u8 taskId);
void StartUnownReportFromBag(void);
void ItemUseOutOfBattle_UnownReport(u8 taskId);

#endif // GUARD_UNOWN_REPORT_H