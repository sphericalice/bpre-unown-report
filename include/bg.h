#ifndef GUARD_BG_H
#define GUARD_BG_H

struct BgTemplate
{
    u16 bg:2;                   // 0x1, 0x2 -> 0x3
    u16 charBaseIndex:2;        // 0x4, 0x8 -> 0xC
    u16 mapBaseIndex:5;         // 0x10, 0x20, 0x40, 0x80, 0x100 -> 0x1F0
    u16 screenSize:2;           // 0x200, 0x400 -> 0x600
    u16 paletteMode:1;          // 0x800
    u16 priority:2;             // 0x1000, 0x2000 > 0x3000
    u16 baseTile:10;
};

void ResetBgsAndClearDma3BusyFlags(u32 leftoverFireRedLeafGreenVariable);
void InitBgsFromTemplates(u8 bgMode, const struct BgTemplate *templates, u8 numTemplates);
bool8 IsDma3ManagerBusyWithBgCopy(void);
void ShowBg(u8 bg);
void SetBgTilemapBuffer(u8 bg, void *tilemap);
void CopyBgTilemapBufferToVram(u8 bg);

#endif // GUARD_BG_H