#include "unown_report.h"

u8 UnownCount() {
    // TODO: Actually count the kinds of Unown the player has caught
    return 28;
}

static u8 GetPage(u8 PageNumber, u8 SwapDirection) {
    if (SwapDirection == PAGE_NEXT) {
        if (PageNumber == MAX_PAGE_COUNT) return PageNumber;
        if (PageNumber == 0) {
            if (UnownCount() == 0) return 4;
            if (UnownCount() > 1) return 1;
        }
        if (PageNumber == 1) {
            if (UnownCount() <= 10) return 4;
            if (UnownCount() > 10) return 2;
        }
        if (PageNumber == 2) {
            if (UnownCount() <= 20) return 4;
            if (UnownCount() > 20) return 3;
        }
        return PageNumber + 1;
    }
    if (SwapDirection == PAGE_PREV) {
        if (PageNumber == 0) return PageNumber;
        if (PageNumber == 4) {
            if (UnownCount() == 0) return 0;
            if (UnownCount() <= 10) return 1;
            if (UnownCount() <= 20) return 2;
            if (UnownCount() > 20) return 3;
        }        
        return PageNumber - 1;
    }
    return PageNumber;
}

static s8 GetPageNumber(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];
    if (GetPage(task->currentPage, SwapDirection) != task->currentPage) {
        task->currentPage = GetPage(task->currentPage, SwapDirection);            
        FillWindowPixelBuffer(0, 0);
        ClearWindowTilemap(0);
        CopyWindowToVram(0, 3);
        return task->currentPage;
    }
    return -1;
}

static void PrintOnUnownReportWindow(void) {
    FillWindowPixelRect(0, PIXEL_FILL(0), 0, 0, 0xD8, 0x90);
    PrintFirstPage();
    PutWindowTilemap(0);
    CopyWindowToVram(0, 3);
}

static void PrintFirstPage() {
    u16 width = 0;
    StringExpandPlaceholders(gStringVar4, gText_PlayersUnownReport);
    width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    AddTextPrinterParameterized3(0, 2, 0x68 - (width >> 1), 40, sTextColors[0], TEXT_SPEED_FF, gStringVar4);

    ConvertIntToDecimalStringN(gStringVar1, UnownCount(), STR_CONV_MODE_LEFT_ALIGN, 4);
    StringExpandPlaceholders(gStringVar4, gText_CurrentKinds);
    width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    AddTextPrinterParameterized3(0, 2, 0x68 - (width >> 1), 64, sTextColors[0], TEXT_SPEED_FF, gStringVar4);
    return;
}

static void PrintUnown(u8 PageNumber, u8 row, u8 col) {
    u8 spriteId;
    LoadMonIconPalettes();
    spriteId = CreateMonIcon(PKMN_UNOWN, SpriteCallbackDummy, 48, 40, 0, 0, TRUE);
    gSprites[spriteId].oam.priority = 3;
    AddTextPrinterParameterized3(0,
                                 2,
                                 48 + col*88,
                                 40 + (row - col)*8,
                                 sTextColors[0],
                                 TEXT_SPEED_FF,
                                 UnownStrings[row + (PageNumber-1) * UNOWN_PER_PAGE]);
}

static void PrintUnownList(u8 PageNumber) {
    u8 row, col, max;
    max = UnownCount();
    if (max > (PageNumber * UNOWN_PER_PAGE)) max = PageNumber * UNOWN_PER_PAGE;
    if (max > 28) max = 28;
    for (row = 0, col = 0; row < max-(PageNumber-1) * UNOWN_PER_PAGE; row++, col ^= 1) {
        PrintUnown(PageNumber, row, col);
    }
    return;
}

static void PrintReportPage(u8 PageNumber) {
    AddTextPrinterParameterized3(0, 2, 16, 40, sTextColors[0], TEXT_SPEED_FF, ReportStrings[PageNumber]);
    return;
}

static void SwapPage(u8 taskId, u8 SwapDirection) {
    s8 PageNumber = GetPageNumber(taskId, SwapDirection);
    if (PageNumber != -1) {
        switch (PageNumber) {
            default:
            case 0:
                PrintFirstPage();
                break;
            case 1:
            case 2:
            case 3:
                PrintUnownList(PageNumber);
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                PrintReportPage(PageNumber-4);
                break;
        }

        PlaySE(SE_SELECT);
        PutWindowTilemap(0);
        CopyWindowToVram(0, 3);
    }
}

static void Task_CheckUnownReportInput(u8 taskId) {
    struct Task *task = &gTasks[taskId];

    if (gMain.newAndRepeatedKeys & DPAD_RIGHT || gMain.newAndRepeatedKeys & DPAD_DOWN || gMain.newKeys & A_BUTTON)
        SwapPage(taskId, PAGE_NEXT);

    if (gMain.newAndRepeatedKeys & DPAD_LEFT || gMain.newAndRepeatedKeys & DPAD_UP)
        SwapPage(taskId, PAGE_PREV);

    if (gMain.newKeys & B_BUTTON)
        task->func = Task_BeginPaletteFade;
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
                gMain.state++;
                break;
            case 4:
                if (IsDma3ManagerBusyWithBgCopy() != TRUE) {
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
    if (!gPaletteFade.active) {
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

void UnownReport() {
    SetVBlankCallback(NULL);
    SetMainCallback2(CB2_UnownReport);
    return;
}