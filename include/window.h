#ifndef GUARD_WINDOW_H
#define GUARD_WINDOW_H

#define PIXEL_FILL(num) ((num) | ((num) << 4))

struct WindowTemplate
{
    u8 bg;
    u8 tilemapLeft;
    u8 tilemapTop;
    u8 width;
    u8 height;
    u8 paletteNum;
    u16 baseBlock;
};

#define DUMMY_WIN_TEMPLATE          \
{                                   \
    0xFF,                           \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
    0,                              \
}

struct Window
{
    struct WindowTemplate window;
    u8 *tileData;
};

bool16 InitWindows(const struct WindowTemplate *templates);
void FreeAllWindowBuffers(void);
void CopyWindowToVram(u8 windowId, u8 mode);
void PutWindowTilemap(u8 windowId);
void ClearWindowTilemap(u8 windowId);
void FillWindowPixelBuffer(u8 windowId, u8 fillValue);

#endif // GUARD_WINDOW_H