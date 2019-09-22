#include "global.h"
#include "main.h"
#include "macro.h"
#include "gpu_regs.h"
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
#include "bag.h"
#include "pokemon_icon.h"
#include "constants/flags.h"
#include "constants/vars.h"
#include "constants/songs.h"
#include "constants/species.h"
#include "graphics/unown.c"
#include "unown_report.h"

void SetCaughtUnown(u16 UnownForm) {
    u32 CaughtUnown = GetCaughtUnown();
    CaughtUnown |= (1 << UnownForm);
    VarSet(VAR_UNOWNCAUGHT_PT1, (CaughtUnown & 0x0000FFFF));
    VarSet(VAR_UNOWNCAUGHT_PT2, ((CaughtUnown & 0xFFFF0000) >> 16));
}

void atkF1_TrySetCaughtMonDexFlags(void) {
    u16 species = GetMonData(&gEnemyParty[0], MON_DATA_SPECIES, NULL);
    u32 personality = GetMonData(&gEnemyParty[0], MON_DATA_PERSONALITY, NULL);

    if (species == SPECIES_UNOWN) {
        SetCaughtUnown(GetUnownLetterByPersonality(personality));
    }

    u16 dexNum = SpeciesToNationalPokedexNum(species);
    if (GetSetPokedexFlag(dexNum, FLAG_GET_CAUGHT)) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    } else {
        HandleSetPokedexFlag(dexNum, FLAG_SET_CAUGHT, personality);
        gBattlescriptCurrInstr += 5;
    }
}

u32 UnownFormToPID(u8 form) {
    // In Generation III, Unown's form is determined by the composite value of
    // the least significant 2 bits of each byte in its PID, so we need to
    // "reverse engineer" this PID for the wanted Unown icon
    return ((form & 0b00110000) << 12)
         | ((form & 0b00001100) << 6)
         | ((form & 0b00000011));
}

void PrintUnownIcon(u8 form, u16 x, u16 y) {
    u8 spriteId;
    LoadMonIconPalettes();
    spriteId = CreateMonIcon(SPECIES_UNOWN,
                             SpriteCallbackDummy,
                             12 + x,
                             52 + y,
                             0,
                             UnownFormToPID(form),
                             TRUE);
    gSprites[spriteId].oam.priority = 0;
}

void PrintUnown(u8 form, u8 row, u8 col) {
    u16 x = col * 88 + 36;
    u16 y = (row - col) * 12;
    PrintUnownIcon(form, x, y);
    PrintUnownReportText(UnownStrings[form], x, y);
}

u32 GetCaughtUnown(void) {
    return (VarGet(VAR_UNOWNCAUGHT_PT2) << 16) | VarGet(VAR_UNOWNCAUGHT_PT1);
}

bool32 UnownFormIsCaught(u8 form) {
    u32 CaughtUnown = GetCaughtUnown();
    return (CaughtUnown & (1 << form));
}

u8 UnownCount(void) {
    u8 UniqueForms = 0;
    for (u8 form = 0; form < UNOWN_FORMS; form++) {
        if (UnownFormIsCaught(form)) {
            UniqueForms++;
        }
    }

    return UniqueForms;
}

void PrintUnownReportText(const u8 *text, u8 x, u8 y) {
    u8 col[3] = {0, 2, 3};
    AddTextPrinterParameterized4(0, 1, x, y, 0, 0, col, -1, text);
}

void PrintReportPage(u8 ReportPageNumber) {
    PrintUnownReportText(ReportPages[ReportPageNumber].str, 0, 0);
}

void PrintUnownList(u8 taskId, u8 PageNumber) {
    struct Task *task = &gTasks[taskId];

    u8 bit = 0;
    if (PageNumber > FIRST_UNOWN_LIST_PAGE) {
        bit = task->bitToStartFrom[PageNumber];
    }

    u8 row;
    u8 col;
    for (row = 0, col = 0; row < UNOWN_PER_PAGE && bit < UNOWN_FORMS; bit++) {
        if (UnownFormIsCaught(bit)) {
            PrintUnown(bit, row, col);
            row++;
            col ^= 1;
        }
    }

    if (PageNumber < LAST_UNOWN_LIST_PAGE) {
        task->bitToStartFrom[PageNumber + 1] = bit;
    }
}

void PrintFrontPage(void) {
    StringExpandPlaceholders(gStringVar4, gText_PlayersUnownReport);
    u16 width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    PrintUnownReportText(gStringVar4, 88 - (width >> 1), 0);

    ConvertIntToDecimalStringN(gStringVar1,
                               UnownCount(),
                               STR_CONV_MODE_LEFT_ALIGN,
                               4);
    StringExpandPlaceholders(gStringVar4, gText_CurrentKinds);
    width = GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF);
    PrintUnownReportText(gStringVar4, 88 - (width >> 1), 24);
}

u8 GetNewPage(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];
    u8 PageNumber = task->currentPage;
    u8 count = UnownCount();

    if (SwapDirection == PAGE_NEXT && PageNumber != LAST_PAGE) {
        if (PageNumber < FIRST_REPORT_PAGE
            && count <= PageNumber * UNOWN_PER_PAGE) {
            return FIRST_REPORT_PAGE;
        }

        if (PageNumber >= FIRST_REPORT_PAGE
            && !FlagGet(ReportPages[PageNumber - FIRST_REPORT_PAGE].flag)) {
            return PageNumber;
        }

        return PageNumber + 1;
    }

    if (SwapDirection == PAGE_PREV && PageNumber != FRONT_PAGE) {
        if (count == 0) return FRONT_PAGE;
        if (PageNumber == FIRST_REPORT_PAGE) {
            return 1 + ((count - 1) / UNOWN_PER_PAGE);
        }

        return PageNumber - 1;
    }

    return PageNumber;
}

void SwapPage(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];

    u8 PageNumber = task->currentPage;
    task->currentPage = GetNewPage(taskId, SwapDirection);

    if (task->currentPage != PageNumber) {
        PageNumber = task->currentPage;

        ResetSpriteData();
        FillWindowPixelBuffer(0, 0);
        ClearWindowTilemap(0);
        CopyWindowToVram(0, 3);

        switch (PageNumber) {
            case FRONT_PAGE:
                PrintFrontPage();
                break;
            case FIRST_UNOWN_LIST_PAGE:
            case SECOND_UNOWN_LIST_PAGE:
            case THIRD_UNOWN_LIST_PAGE:
            case LAST_UNOWN_LIST_PAGE:
                PrintUnownList(taskId, PageNumber);
                break;
            default:
                PrintReportPage(PageNumber - FIRST_REPORT_PAGE);
                break;
        }

        PutWindowTilemap(0);
        CopyWindowToVram(0, 3);

        PlaySE(SE_SWAP_PAGE);
    }
}

void Task_UnownReportFadeOut(u8 taskId) {
    if (!gPaletteFade.active) {
        Free(sTilemapBuffer);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        if (sOpenedFromOW == TRUE) {
            SetMainCallback2(CB2_ReturnToField);
        } else {
            SetMainCallback2(CB2_ReturnToBag);
        }
    }
}

void Task_UnownReportWaitForKeyPress(u8 taskId) {
    if (gMain.newAndRepeatedKeys & (DPAD_RIGHT | DPAD_DOWN)
        || gMain.newKeys & A_BUTTON) {
        SwapPage(taskId, PAGE_NEXT);
    }

    if (gMain.newAndRepeatedKeys & (DPAD_LEFT | DPAD_UP)) {
        SwapPage(taskId, PAGE_PREV);
    }

    if (gMain.newKeys & (B_BUTTON)) {
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_UnownReportFadeOut;
    }
}

void Task_UnownReportFadeIn(u8 taskId) {
    if (!gPaletteFade.active) {
        gTasks[taskId].func = Task_UnownReportWaitForKeyPress;
    }
}

void PrintInstructionsBar(void) {
    u8 col[3] = {0, 1, 2};
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    AddTextPrinterParameterized3(1, 0, 2, 1, col, 0, gText_Instructions);
    PutWindowTilemap(1);
    CopyWindowToVram(1, 3);
}

void MainCB2(void) {
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void VBlankCB(void) {
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void InitUnownReportFrontPage(void) {
    PrintInstructionsBar();
    PrintFrontPage();
    PutWindowTilemap(0);
    CopyWindowToVram(0, 3);
}

void InitUnownReportWindow(void) {
    InitWindows(sUnownReportWinTemplates);
    DeactivateAllTextPrinters();
    LoadPalette(unownPal, 0xC0, 0x20);
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    PutWindowTilemap(0);
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
    ResetTempTileDataBuffers();
    DecompressAndCopyTileDataToVram(2, &unownTiles, 0, 0, 0);
    while (TryToFreeTempTileDataBuffers());
    LZDecompressWram(unownMap, sTilemapBuffer);
    CopyBgTilemapBufferToVram(2);
    InitUnownReportFrontPage();
    BlendPalettes(-1, 16, 0);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
    EnableInterrupts(1);
    SetVBlankCallback(VBlankCB);
    SetMainCallback2(MainCB2);
    CreateTask(Task_UnownReportFadeIn, 0);
}

void UnownReport_Execute(bool8 src) {
    sOpenedFromOW = src;
    SetMainCallback2(CB2_ShowUnownReport);
}

void Task_UnownReportFromOW(u8 taskId) {
    if (!gPaletteFade.active) {
        FreeOverworldBGs();
        PrepareOverworldReturn();
        UnownReport_Execute(TRUE);
        DestroyTask(taskId);
    }    
}

void StartUnownReportFromBag(void) {
    UnownReport_Execute(FALSE);
}

void ItemUseOutOfBattle_UnownReport(u8 taskId) {
    struct Task *task = &gTasks[taskId];
    if (task->invokedFromOW) {
        FadeScreen(1,0);
        task->func = Task_UnownReportFromOW;
    } else {
        SetBagCallback(StartUnownReportFromBag);
        ReturnFromItemToBag(taskId);
    }
}
