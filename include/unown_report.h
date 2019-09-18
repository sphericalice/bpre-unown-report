#pragma once

extern u8 gLastResult[];
void vblank_handler_set(void(*)(void));
void set_callback2(void(*));
u32 sub_80CD240();
