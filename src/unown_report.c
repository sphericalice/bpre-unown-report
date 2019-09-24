#include "global.h"

#include "alloc.h"
#include "bag.h"
#include "bg.h"
#include "decompress.h"
#include "event_data.h"
#include "gpu_regs.h"
#include "international_string_util.h"
#include "list_menu.h"
#include "main.h"
#include "overworld.h"
#include "palette.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "rgb.h"
#include "scanline_effect.h"
#include "sound.h"
#include "string_util.h"
#include "strings.h"
#include "window.h"

#include "unown_report.h"

#include "constants/songs.h"
#include "constants/species.h"
#include "graphics/unown.c"

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

void SetCaughtUnown(u16 UnownForm) {
    u32 CaughtUnown = GetCaughtUnown();
    CaughtUnown |= (1 << UnownForm);
    VarSet(VAR_UNOWNCAUGHT_PT1, (CaughtUnown & 0x0000FFFF));
    VarSet(VAR_UNOWNCAUGHT_PT2, (CaughtUnown & 0xFFFF0000) >> 16);
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
    u8 unownTag = form + 2;
    for (spriteId = 0; spriteId < 30; spriteId++) {
        struct Sprite *sprite = &gSprites[spriteId];
        if (sprite->data[0] == unownTag) {
            sprite->invisible = FALSE;
            return;
        }
    }
    spriteId = 0;
    LoadMonIconPalettes();
    spriteId = CreateMonIcon(SPECIES_UNOWN,
                             SpriteCallbackDummy,
                             12 + x,
                             52 + y,
                             0,
                             UnownFormToPID(form),
                             TRUE);
    gSprites[spriteId].oam.priority = 0;
    gSprites[spriteId].invisible = FALSE;
    gSprites[spriteId].data[0] = unownTag;
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

void PrintReportPage(u8 page) {
    PrintUnownReportText(ReportPages[page].str, 0, 0);
}

void PrintUnownList(u8 taskId, u8 page) {
    struct Task *task = &gTasks[taskId];

    u8 bit = 0;
    if (page > FIRST_UNOWN_LIST_PAGE) {
        bit = task->bitToStartFrom[page];
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

    if (page < LAST_UNOWN_LIST_PAGE) {
        task->bitToStartFrom[page + 1] = bit;
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

void RemoveUnownIconSprites(void) {
    for (u8 spriteId = 0; spriteId < 30; spriteId++) {
        struct Sprite *sprite = &gSprites[spriteId];
        if (sprite->data[0] != 0) sprite->invisible = TRUE;
    }
}

u8 GetNewPage(u8 page, u8 SwapDirection) {
    u8 count = UnownCount();

    if (SwapDirection == PAGE_NEXT && page < LAST_PAGE) {
        if (page < FIRST_REPORT_PAGE) {
            if (count <= page * UNOWN_PER_PAGE) {
                return FIRST_REPORT_PAGE;
            }
        } else if (!FlagGet(ReportPages[page - FIRST_REPORT_PAGE].flag)) {
            return page;
        }

        return page + 1;
    }

    if (SwapDirection == PAGE_PREV && page > FRONT_PAGE) {
        if (count == 0) {
            return FRONT_PAGE;
        }

        if (page == FIRST_REPORT_PAGE) {
            return 1 + ((count - 1) / UNOWN_PER_PAGE);
        }

        return page - 1;
    }

    return page;
}

void SwapPage(u8 taskId, u8 SwapDirection) {
    struct Task *task = &gTasks[taskId];

    u8 page = task->currentPage;
    task->currentPage = GetNewPage(task->currentPage, SwapDirection);

    if (task->currentPage != page) {
        page = task->currentPage;

        FillWindowPixelBuffer(0, 0);
        ClearWindowTilemap(0);
        CopyWindowToVram(0, 3);
        RemoveUnownIconSprites();

        switch (page) {
            case FRONT_PAGE:
                PrintFrontPage();
                break;
            case FIRST_UNOWN_LIST_PAGE:
            case SECOND_UNOWN_LIST_PAGE:
            case THIRD_UNOWN_LIST_PAGE:
            case LAST_UNOWN_LIST_PAGE:
                PrintUnownList(taskId, page);
                break;
            default:
                PrintReportPage(page - FIRST_REPORT_PAGE);
                break;
        }

        PutWindowTilemap(0);
        CopyWindowToVram(0, 3);
        PlaySE(SE_SWAP_PAGE);

        if (page == FRONT_PAGE) {
            RemoveScrollIndicatorArrowPair(task->data[10]);
            task->data[8] = 1;
            task->data[10] = AddScrollIndicatorArrowPairParameterized(0, 88, -1, 224, 1, 110, 110, 0);
        } else if (GetNewPage(page, PAGE_NEXT) == page) {
            RemoveScrollIndicatorArrowPair(task->data[10]);
            task->data[8] = 1;
            task->data[10] = AddScrollIndicatorArrowPairParameterized(0, 88, 16, -1, 1, 110, 110, 0);
        } else {
            RemoveScrollIndicatorArrowPair(task->data[10]);
            task->data[8] = 1;
            task->data[10] = AddScrollIndicatorArrowPairParameterized(0, 88, 16, 224, 1, 110, 110, 0);
        }

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
        struct Task *task = &gTasks[taskId];

        task->data[8] = 1;
        task->data[10] = AddScrollIndicatorArrowPairParameterized(0, 88, -1, 224, 1, 110, 110, 0);

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

void MainCB2_UnownReport(void) {
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

void VBlankCB_UnownReport(void) {
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void InitUnownReportFrontPage(void) {
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    PrintInstructionsBar();
    PrintFrontPage();
    PutWindowTilemap(0);
    CopyWindowToVram(0, 3);
}

void LoadUnownReportGfx(void) {
    DecompressAndCopyTileDataToVram(2, &unownTiles, 0, 0, 0);
    LZDecompressWram(unownMap, sTilemapBuffer);
    LoadPalette(unownPal, 0, 0x20);
}

void ClearTasksAndGraphicalStructs(void) {
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    ResetTempTileDataBuffers();
    ResetPaletteFade();
    FreeAllSpritePalettes();
}

void ClearVramOamPlttRegs(void) {
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
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
}

void CB2_UnownReport(void) {
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
                sTilemapBuffer = Alloc(0x1000);
                ResetBgsAndClearDma3BusyFlags(0);
                InitBgsFromTemplates(0, sUnownReportBgTemplates, 3);
                SetBgTilemapBuffer(2, sTilemapBuffer);
                gMain.state++;
                break;
            case 3:
                LoadUnownReportGfx();
                gMain.state++;
                break;
            case 4:
                if (IsDma3ManagerBusyWithBgCopy() != TRUE) {
                    ShowBg(0);
                    ShowBg(1);
                    ShowBg(2);
                    CopyBgTilemapBufferToVram(2);
                    gMain.state++;
                }
                break;
            case 5:
                InitWindows(sUnownReportWinTemplates);
                DeactivateAllTextPrinters();
                gMain.state++;
                break;
            case 6:
                BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
                gMain.state++;
                break;
            case 7:
                SetVBlankCallback(VBlankCB_UnownReport);
                InitUnownReportFrontPage();
                CreateTask(Task_UnownReportFadeIn, 0);
                SetMainCallback2(MainCB2_UnownReport);
                gMain.state = 0;
                break;
        }
    }
}

void UnownReport_Execute(bool8 src) {
    sOpenedFromOW = src;
    SetMainCallback2(CB2_UnownReport);
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
