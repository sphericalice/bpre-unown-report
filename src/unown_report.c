#include "unown_report.h"

static void MainCB2(void);
static void Task_UnownReportFadeIn(u8);
static void Task_UnownReportWaitForKeyPress(u8);
static void Task_UnownReportFadeOut(u8);
static void DisplayUnownReportText(void);
static void InitUnownReportBg(void);
static void InitUnownReportWindow(void);
static void PrintUnownReportText(u8 *, u8, u8);

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_ShowUnownReport(void)
{
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
    // why doesn't this one use the dma manager either?
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
    decompress_and_copy_tile_data_to_vram(1, &unownTiles, 0, 0, 0);
    while (free_temp_tile_data_buffers_if_possible())
        ;
    LZDecompressWram(unownMap, sTilemapBuffer);
    CopyBgTilemapBufferToVram(1);
    DisplayUnownReportText();
    BlendPalettes(-1, 16, 0);
    BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
    EnableInterrupts(1);
    SetVBlankCallback(VBlankCB);
    SetMainCallback2(MainCB2);
    CreateTask(Task_UnownReportFadeIn, 0);
}

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void Task_UnownReportFadeIn(u8 taskId)
{
    if (!gPaletteFade.active)
        gTasks[taskId].func = Task_UnownReportWaitForKeyPress;
}

static void Task_UnownReportWaitForKeyPress(u8 taskId)
{
    if (gMain.newKeys & (A_BUTTON | B_BUTTON))
    {
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
        gTasks[taskId].func = Task_UnownReportFadeOut;
    }
}

static void Task_UnownReportFadeOut(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        Free(sTilemapBuffer);
        FreeAllWindowBuffers();
        DestroyTask(taskId);
        SetMainCallback2(CB2_ReturnToFieldContinueScriptPlayMapMusic);
    }
}

static void DisplayUnownReportText(void)
{
    SetGpuReg(REG_OFFSET_BG1HOFS, DISPCNT_MODE_0);
    StringExpandPlaceholders(gStringVar4, gText_PlayersUnownReport);
    PrintUnownReportText(gStringVar4, 80 - (GetStringCenterAlignXOffset(2, gStringVar4, 0xFFFF) >> 1), 36);
    PutWindowTilemap(0);
    CopyWindowToVram(0, 3);
}

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

static void InitUnownReportBg(void)
{
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sUnownReportBgTemplates, 2);
    SetBgTilemapBuffer(1, sTilemapBuffer);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    ShowBg(1);
    SetGpuReg(REG_OFFSET_BLDCNT, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BLDALPHA, DISPCNT_MODE_0);
    SetGpuReg(REG_OFFSET_BLDY, DISPCNT_MODE_0);
}

static const struct WindowTemplate sUnownReportWinTemplates[2] =
{
    {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 2,
        .width = 20,
        .height = 16,
        .paletteNum = 15,
        .baseBlock = 1,
    },
    DUMMY_WIN_TEMPLATE,
};

static void InitUnownReportWindow(void)
{
    InitWindows(sUnownReportWinTemplates);
    DeactivateAllTextPrinters();
    LoadPalette(unownPal, 0xC0, 0x20);
    FillWindowPixelBuffer(0, PIXEL_FILL(0));
    PutWindowTilemap(0);
}

static void PrintUnownReportText(u8 *text, u8 var1, u8 var2)
{
    u8 color[3] = {0, 2, 3};

    AddTextPrinterParameterized4(0, 1, var1, var2, 0, 0, color, -1, text);
}

void Special_ShowUnownReport(void)
{
    SetMainCallback2(CB2_ShowUnownReport);
    ScriptContext2_Enable();
}