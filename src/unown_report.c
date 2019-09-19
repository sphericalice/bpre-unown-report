#include "global.h"
#include "main.h"
#include "unown_report.h"
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
#include <stddef.h>

static const u8 sTextColors[2][3] =
{
    {0, 2, 3},
    {15, 14, 13}
};

u8 GetTotalPages() {
    return 9;
}

u8 CurrentKindsOfUnownCaught() {
    // TODO: Actually count the kinds of Unown the player has caught
    return 0;
}

static void PrintOnUnownReportWindow(void) {
    FillWindowPixelRect(0, PIXEL_FILL(0), 0, 0, 256, 0x90);
    u16 width = 0;
    StringExpandPlaceholders(gStringVar4, gText_PlayersUnownReport);
    width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    AddTextPrinterParameterized3(0, 2, 0x68 - (width >> 1), 40, sTextColors[0], TEXT_SPEED_FF, gStringVar4);

    ConvertIntToDecimalStringN(gStringVar1, CurrentKindsOfUnownCaught(), STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gStringVar4, gText_CurrentKinds);
    width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    AddTextPrinterParameterized3(0, 2, 0x68 - (width >> 1), 64, sTextColors[0], TEXT_SPEED_FF, gStringVar4);    

    PutWindowTilemap(0);
    CopyWindowToVram(0, 3);
}

#define PAGE_NEXT 0
#define PAGE_PREV 1
#define currentPage data[0]

static u8 GetPage(u8 PageNumber, u8 SwapDirection) {
    if (SwapDirection == PAGE_NEXT) {
        if (PageNumber == GetTotalPages()) return PageNumber;
        if (PageNumber == 0) {
            if (CurrentKindsOfUnownCaught() == 0) return 4;
            if (CurrentKindsOfUnownCaught() > 1) return 1;
        }
        if (PageNumber == 1) {
            if (CurrentKindsOfUnownCaught() <= 10) return 4;
            if (CurrentKindsOfUnownCaught() > 10) return 2;
        }
        if (PageNumber == 2) {
            if (CurrentKindsOfUnownCaught() <= 20) return 4;
            if (CurrentKindsOfUnownCaught() > 20) return 3;
        }
        return PageNumber + 1;
    }
    if (SwapDirection == PAGE_PREV) {
        if (PageNumber == 0) return PageNumber;
        if (PageNumber == 4) {
            if (CurrentKindsOfUnownCaught() == 0) return 0;
            if (CurrentKindsOfUnownCaught() <= 10) return 1;
            if (CurrentKindsOfUnownCaught() <= 20) return 2;
            if (CurrentKindsOfUnownCaught() > 20) return 3;
        }        
        return PageNumber - 1;
    }
    return PageNumber;
}

static void SwapPage(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];
    s8 PageNumber = -1;

    if (GetPage(task->currentPage, SwapDirection) != task->currentPage) {
        task->currentPage = GetPage(task->currentPage, SwapDirection);            
        FillWindowPixelBuffer(0, 0);
        ClearWindowTilemap(0);
        CopyWindowToVram(0, 3);
        PageNumber = task->currentPage;
    }

    if (PageNumber != -1) {
        u16 width = 0;
        switch (PageNumber) {
            case 0:
                StringExpandPlaceholders(gStringVar4, gText_PlayersUnownReport);
                width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
                AddTextPrinterParameterized3(0, 2, 0x68 - (width >> 1), 40, sTextColors[0], TEXT_SPEED_FF, gStringVar4);

                ConvertIntToDecimalStringN(gStringVar1, CurrentKindsOfUnownCaught(), STR_CONV_MODE_LEFT_ALIGN, 4);
                StringExpandPlaceholders(gStringVar4, gText_CurrentKinds);
                width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
                AddTextPrinterParameterized3(0, 2, 0x68 - (width >> 1), 64, sTextColors[0], TEXT_SPEED_FF, gStringVar4);
                break;
            case 1: 
                AddTextPrinterParameterized3(0, 2, 48, 40, sTextColors[0], TEXT_SPEED_FF, gText_UnownA);
                AddTextPrinterParameterized3(0, 2, 48, 56, sTextColors[0], TEXT_SPEED_FF, gText_UnownC);
                AddTextPrinterParameterized3(0, 2, 48, 72, sTextColors[0], TEXT_SPEED_FF, gText_UnownE);
                AddTextPrinterParameterized3(0, 2, 48, 88, sTextColors[0], TEXT_SPEED_FF, gText_UnownG);
                AddTextPrinterParameterized3(0, 2, 48, 104, sTextColors[0], TEXT_SPEED_FF, gText_UnownI);
                
                AddTextPrinterParameterized3(0, 2, 136, 40, sTextColors[0], TEXT_SPEED_FF, gText_UnownB);
                AddTextPrinterParameterized3(0, 2, 136, 56, sTextColors[0], TEXT_SPEED_FF, gText_UnownD);
                AddTextPrinterParameterized3(0, 2, 136, 72, sTextColors[0], TEXT_SPEED_FF, gText_UnownF);
                AddTextPrinterParameterized3(0, 2, 136, 88, sTextColors[0], TEXT_SPEED_FF, gText_UnownH);
                AddTextPrinterParameterized3(0, 2, 136, 104, sTextColors[0], TEXT_SPEED_FF, gText_UnownJ);
                break;
            case 2:
                AddTextPrinterParameterized3(0, 2, 48, 40, sTextColors[0], TEXT_SPEED_FF, gText_UnownK);
                AddTextPrinterParameterized3(0, 2, 48, 56, sTextColors[0], TEXT_SPEED_FF, gText_UnownM);
                AddTextPrinterParameterized3(0, 2, 48, 72, sTextColors[0], TEXT_SPEED_FF, gText_UnownO);
                AddTextPrinterParameterized3(0, 2, 48, 88, sTextColors[0], TEXT_SPEED_FF, gText_UnownQ);
                AddTextPrinterParameterized3(0, 2, 48, 104, sTextColors[0], TEXT_SPEED_FF, gText_UnownS);
                
                AddTextPrinterParameterized3(0, 2, 136, 40, sTextColors[0], TEXT_SPEED_FF, gText_UnownL);
                AddTextPrinterParameterized3(0, 2, 136, 56, sTextColors[0], TEXT_SPEED_FF, gText_UnownN);
                AddTextPrinterParameterized3(0, 2, 136, 72, sTextColors[0], TEXT_SPEED_FF, gText_UnownP);
                AddTextPrinterParameterized3(0, 2, 136, 88, sTextColors[0], TEXT_SPEED_FF, gText_UnownR);
                AddTextPrinterParameterized3(0, 2, 136, 104, sTextColors[0], TEXT_SPEED_FF, gText_UnownT);
                break;
            case 3:
                AddTextPrinterParameterized3(0, 2, 48, 40, sTextColors[0], TEXT_SPEED_FF, gText_UnownU);
                AddTextPrinterParameterized3(0, 2, 48, 56, sTextColors[0], TEXT_SPEED_FF, gText_UnownW);
                AddTextPrinterParameterized3(0, 2, 48, 72, sTextColors[0], TEXT_SPEED_FF, gText_UnownY);
                AddTextPrinterParameterized3(0, 2, 48, 88, sTextColors[0], TEXT_SPEED_FF, gText_UnownQuestionMark);
                
                AddTextPrinterParameterized3(0, 2, 136, 40, sTextColors[0], TEXT_SPEED_FF, gText_UnownV);
                AddTextPrinterParameterized3(0, 2, 136, 56, sTextColors[0], TEXT_SPEED_FF, gText_UnownX);
                AddTextPrinterParameterized3(0, 2, 136, 72, sTextColors[0], TEXT_SPEED_FF, gText_UnownZ);
                AddTextPrinterParameterized3(0, 2, 136, 88, sTextColors[0], TEXT_SPEED_FF, gText_UnownExclamationMark);
                break;
            case 4:
                AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, gText_Report1);
                break;
            case 5:
                AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, gText_Report2);
                break;
            case 6:
                AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, gText_Report3);
                break;
            case 7:
                AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, gText_Report4);
                break;
            case 8:
                AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, gText_Report5);
                break;
            case 9:
                AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, gText_Report6);
                break;
        }

        PlaySE(SE_SELECT);
        PutWindowTilemap(0);
        CopyWindowToVram(0, 3);
    }
}

u8 *sTilemapBuffer;
static const struct BgTemplate sUnownReportBgTemplates[] = { { 0, 0, 31, 0, 0, 0, 0 }, { 3, 1, 30, 0, 0, 3, 0 } };

static const struct WindowTemplate sUnownReportWindowTemplates[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 1,
        .width = 26,
        .height = 18,
        .paletteNum = 15,
        .baseBlock = 20
    },
    DUMMY_WIN_TEMPLATE
};

static void SetDispcntReg(void) {
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_BG0_ON | DISPCNT_BG3_ON | DISPCNT_OBJ_1D_MAP);
}

static void ClearVramOamPlttRegs(void) {
    DmaClearLarge16(3, (void*)(VRAM), VRAM_SIZE, 0x1000);
    DmaClear32(3, OAM, OAM_SIZE);
    DmaClear16(3, PLTT, PLTT_SIZE);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
}

static void ClearTasksAndGraphicalStructs(void) {
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    dp13_810BB8C();
    ResetPaletteFade();
    FreeAllSpritePalettes();
}

static void ResetBgCoordinates(void) {
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
}

static void LoadUnownReportWindowGfx(u8 bgId) {
    LoadBgTiles(bgId, unownTiles, sizeof(unownTiles), 0);
    CopyToBgTilemapBufferRect(bgId, unownMap, 0,0, 0x20, 0x20);
    LoadPalette(unownPal, 0, 0x20);
}

static void VblankCB_UnownReport(void) {
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void RemoveUnownReportWindow(u8 windowId) {
    FillWindowPixelBuffer(windowId, PIXEL_FILL(0));
    ClearWindowTilemap(windowId);
    CopyWindowToVram(windowId, 2);
    RemoveWindow(windowId);
}

static void Task_ExitUnownReport(u8 taskId) {
    if (!gPaletteFade.active)
    {
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        Free(sTilemapBuffer);
        RemoveUnownReportWindow(0);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
    }
}

static void Task_BeginPaletteFade(u8 taskId) {
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 0x10, RGB_BLACK);
    gTasks[taskId].func = Task_ExitUnownReport;
}

static void Task_CheckUnownReportInput(u8 taskId) {
    struct Task *task = &gTasks[taskId];

    if (gMain.newAndRepeatedKeys & DPAD_RIGHT || gMain.newAndRepeatedKeys & DPAD_DOWN)
    {
        SwapPage(taskId, PAGE_NEXT);
    }
    
    if (gMain.newAndRepeatedKeys & DPAD_LEFT || gMain.newAndRepeatedKeys & DPAD_UP) {      
        SwapPage(taskId, PAGE_PREV);
    }
    
    if (gMain.newKeys & B_BUTTON) {
        task->func = Task_BeginPaletteFade;
    }
}

static void Task_UnownReportWaitForPaletteFade(u8 taskId) {
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_CheckUnownReportInput;
}

static void MainCB2_UnownReport(void) {
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void CB2_UnownReport() {
    if (gMain.state <= 7) {
        switch (gMain.state) {
            case 0:
                SetVBlankCallback(NULL);
                ClearVramOamPlttRegs();
                gMain.state++;
                break;
            case 1:
                ClearTasksAndGraphicalStructs();
                gMain.state++;
                break;
            case 2:
                sTilemapBuffer = AllocZeroed(0x1200);
                ResetBgsAndClearDma3BusyFlags(0);
                InitBgsFromTemplates(0, sUnownReportBgTemplates, ARRAY_COUNT(sUnownReportBgTemplates));
                SetBgTilemapBuffer(3, sTilemapBuffer);
                ResetBgCoordinates();
                gMain.state++;
                break;
            case 3:
                LoadUnownReportWindowGfx(3);
                LoadPalette(stdpal_get(0), 0xF0, 0x20);
                gMain.state++;
                break;
            case 4:
                if (IsDma3ManagerBusyWithBgCopy() != TRUE)
                {
                    ShowBg(0);
                    ShowBg(3);
                    CopyBgTilemapBufferToVram(3);
                    gMain.state++;
                }
                break;
            case 5:
                InitWindows(sUnownReportWindowTemplates);
                DeactivateAllTextPrinters();
                gMain.state++;
                break;
            case 6:
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 0x10, 0, RGB_BLACK);
                gMain.state++;
                break;
            case 7:
                SetDispcntReg();
                SetVBlankCallback(VblankCB_UnownReport);
                PrintOnUnownReportWindow();
                CreateTask(Task_UnownReportWaitForPaletteFade, 8);
                SetMainCallback2(MainCB2_UnownReport);
                gMain.state = 0;
                break;
        }
    }
}

void UnownReport() {
    SetVBlankCallback(NULL);
    SetMainCallback2(CB2_UnownReport);
    
    return;
}
