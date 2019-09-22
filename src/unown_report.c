#include "unown_report.h"

void VBlankCB(void) {
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void InitUnownReportBg(void) {
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sUnownReportBgTemplates, 3);
    SetBgTilemapBuffer(2, sTilemapBuffer);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
    SetGpuReg(REG_OFFSET_BLDCNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BLDALPHA, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BLDY, DISPCNT_MODE_0);
}

void PrintInstructionsBar(void) {
    u8 col[3] = {0, 1, 2};

    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    AddTextPrinterParameterized3(1, 0, 2, 1, col, 0, (u8 *)gText_Instructions);
    PutWindowTilemap(1);
    CopyWindowToVram(1, 3);
}

void InitUnownReportWindow(void) {
    InitWindows(sUnownReportWinTemplates);
    DeactivateAllTextPrinters();
    LoadPalette(unownPal, 0xC0, 0x20);
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    PutWindowTilemap(0);
}

void InitUnownReportDisplay(void) {
    PrintInstructionsBar();
    PrintFirstPage();
    PutWindowTilemap(0);
    CopyWindowToVram(0, 3);
}

void CB2_ShowUnownReport(void) {
    SetVBlankCallback(NULL);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0CNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG3VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG2VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG1VOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0HOFS, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BG0VOFS, DISPCNT_MODE_0);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    LoadPalette(unownPal, 0, 64);
    sTilemapBuffer = Alloc(0x1000);
    InitUnownReportBg();
    InitUnownReportWindow();
    reset_temp_tile_data_buffers();
    decompress_and_copy_tile_data_to_vram(2, &unownTiles, 0, 0, 0);
    while (free_temp_tile_data_buffers_if_possible());
    LZDecompressWram(unownMap, sTilemapBuffer);
    CopyBgTilemapBufferToVram(2);
    InitUnownReportDisplay();
    BlendPalettes(-1, 16, 0);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
    EnableInterrupts(1);
    SetVBlankCallback(VBlankCB);
    SetMainCallback2(MainCB2);
    CreateTask(Task_UnownReportFadeIn, 0);
}

void MainCB2(void) {
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void Task_UnownReportFadeIn(u8 taskId) {
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_UnownReportWaitForKeyPress;
}

u32 GetCaughtUnown(void) {
    return VarGet(VAR_UNOWNCAUGHT_PT2) << 16 | VarGet(VAR_UNOWNCAUGHT_PT1);
}

void SetCaughtUnown(u16 UnownForm) {
    u32 CaughtUnown = GetCaughtUnown();
    CaughtUnown |= 1 << UnownForm;
    VarSet(VAR_UNOWNCAUGHT_PT1, (CaughtUnown & 0x0000FFFF));
    VarSet(VAR_UNOWNCAUGHT_PT2, (CaughtUnown & 0xFFFF0000) >> 16);
}

u8 UnownCount(void) {
    u8 UniqueForms = 0;
    u32 CaughtUnown = GetCaughtUnown();
    for (u8 i = 0; i < UNOWN_FORMS; i++)
        if (CaughtUnown & (1 << i)) UniqueForms++;

    return UniqueForms;
}

void PrintFirstPage(void) {
    StringExpandPlaceholders(gStringVar4, gText_PlayersUnownReport);
    u16 width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    PrintUnownReportText(gStringVar4, 104 - (width >> 1), 48);

    ConvertIntToDecimalStringN(gStringVar1,
                               UnownCount(),
                               STR_CONV_MODE_LEFT_ALIGN,
                               4);
    StringExpandPlaceholders(gStringVar4, gText_CurrentKinds);
    width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    PrintUnownReportText(gStringVar4, 104 - (width >> 1), 72);
}

u32 UnownFormToPID(u8 form) {
    // In Generation III, Unown's form is determined by the composite value of
    // the least significant 2 bits of each byte in its PID, so we need to
    // "reverse engineer" this PID for the wanted Unown icon
    return ((form & 0b00110000) << 12)
         | ((form & 0b00001100) << 6)
         | ((form & 0b00000011));
}

void DisplayUnownIcon(u8 form, u16 x, u16 y) {
    u8 spriteId;
    LoadMonIconPalettes();
    spriteId = CreateMonIcon(SPECIES_UNOWN,
                             SpriteCallbackDummy,
                             x,
                             y,
                             0,
                             UnownFormToPID(form),
                             TRUE);
    gSprites[spriteId].oam.priority = 0;
}

void PrintUnown(u8 form, u8 row, u8 col) {
    u16 x = 48 + col * 88;
    u16 y = 48 + (row - col) * 12;
    DisplayUnownIcon(form, x, y + 4);
    PrintUnownReportText((u8 *)UnownStrings[form], x, y);
}

void PrintUnownList(u8 taskId, u8 PageNumber) {
    struct Task *task = &gTasks[taskId];
    u32 CaughtUnown = GetCaughtUnown();
    u8 bit = 0;
    if (PageNumber > 1) bit = task->data[PageNumber];
    for (u8 row = 0, col = 0; row < UNOWN_PER_PAGE && bit < UNOWN_FORMS; bit++) {
        if (CaughtUnown & (1 << bit)) {
            PrintUnown(bit, row, col);
            row++;
            col ^= 1;
        }
    }
    if (PageNumber < 4) task->data[PageNumber + 1] = bit;
}

void PrintReportPage(u8 ReportPageNumber) {
    PrintUnownReportText((u8 *)ReportPages[ReportPageNumber].str, 16, 48);
}

u8 GetPage(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];
    u8 PageNumber = task->currentPage;
    u8 count = UnownCount();

    if (SwapDirection == PAGE_NEXT && PageNumber != MAX_PAGE_COUNT) {
        if (PageNumber < FIRST_REPORT_PAGE
            && count <= PageNumber * UNOWN_PER_PAGE)
            return FIRST_REPORT_PAGE;

        if (PageNumber >= FIRST_REPORT_PAGE
            && FlagGet(ReportPages[PageNumber-FIRST_REPORT_PAGE].flag) == FALSE)
            return PageNumber;

        return PageNumber + 1;
    }

    if (SwapDirection == PAGE_PREV && PageNumber != 0) {
        if (PageNumber == FIRST_REPORT_PAGE)
            return 1 + ((count - 1) / UNOWN_PER_PAGE);

        return PageNumber - 1;
    }

    return PageNumber;
}

void SwapPage(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];
    u8 PageNumber = task->currentPage;
    task->currentPage = GetPage(taskId, SwapDirection);
    if (PageNumber != task->currentPage) {
        PageNumber = task->currentPage;
        ResetSpriteData();
        FillWindowPixelBuffer(0, 0);
        ClearWindowTilemap(0);
        CopyWindowToVram(0, 3);
        switch (PageNumber) {
            case 0:
                PrintFirstPage();
                break;
            case 1:
            case 2:
            case 3:
            case 4:
                PrintUnownList(taskId, PageNumber);
                break;
            default:
                PrintReportPage(PageNumber - FIRST_REPORT_PAGE);
                break;
        }
        PutWindowTilemap(0);
        CopyWindowToVram(0, 3);
        PlaySE(SE_SELECT);
    }
}

void Task_UnownReportWaitForKeyPress(u8 taskId) {
    if (gMain.newAndRepeatedKeys & (DPAD_RIGHT | DPAD_DOWN)
        || gMain.newKeys & A_BUTTON)
        SwapPage(taskId, PAGE_NEXT);

    if (gMain.newAndRepeatedKeys & (DPAD_LEFT | DPAD_UP))
        SwapPage(taskId, PAGE_PREV);

    if (gMain.newKeys & (B_BUTTON)) {
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_UnownReportFadeOut;
    }
}

void Task_UnownReportFadeOut(u8 taskId) {
    if (!gPaletteFade.active) {
        Free(sTilemapBuffer);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        if (OpenedFromOW == TRUE) SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
        else SetMainCallback2(CB2_ReturnToBag);
    }
}

void PrintUnownReportText(u8 *text, u8 x, u8 y) {
    u8 col[3] = {0, 2, 3};
    AddTextPrinterParameterized4(0, 1, x, y, 0, 0, col, -1, text);
}

void UnownReport_Execute(bool8 src) {
    VarSet(VAR_UNOWNCAUGHT_PT1, 0b1111111111111111);
    VarSet(VAR_UNOWNCAUGHT_PT2, 0b1111111111111111);
    FlagSet(FLAG_REPORT2);
    FlagSet(FLAG_REPORT3);
    FlagSet(FLAG_REPORT4);
    FlagSet(FLAG_REPORT5);
    FlagSet(FLAG_REPORT6);
    OpenedFromOW = src;
    SetMainCallback2(CB2_ShowUnownReport);
}

void Task_UnownReportFromOW(u8 taskId) {
    if (!gPaletteFade.active) {
        overworld_free_bgmaps();
        sub_80A1184();
        UnownReport_Execute(TRUE);
        DestroyTask(taskId);
    }    
}

void StartUnownReportFromBag() {
    UnownReport_Execute(FALSE);
}

void ItemUseOutOfBattle_UnownReport(u8 taskId) {    
    struct Task *task = &gTasks[taskId];
    if (task->data[3]) {
        fade_screen(1,0);
        task->func = Task_UnownReportFromOW;
    } else {
        set_bag_callback(StartUnownReportFromBag);
        unknown_ItemMenu_Confirm(taskId);
    }
}

void atkF1_trysetcaughtmondexflags(void) {
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);
    u32 personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY, NULL);

    if (species == SPECIES_UNOWN) {
        SetCaughtUnown(GetUnownLetterByPersonality(personality));
    }

    if (GetSetPokedexFlag(SpeciesToNationalPokedexNum(species),
                          FLAG_GET_CAUGHT)) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    } else {
        HandleSetPokedexFlag(SpeciesToNationalPokedexNum(species),
                             FLAG_SET_CAUGHT,
                             personality);
        gBattlescriptCurrInstr += 5;
    }
}
