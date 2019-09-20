#ifndef GUARD_EVENT_DATA_H
#define GUARD_EVENT_DATA_H

#include "constants/flags.h"

u8 *GetFlagPointer(u16 id);
u8 FlagSet(u16 id);
u8 FlagClear(u16 id);
bool8 FlagGet(u16 id);

#endif // GUARD_EVENT_DATA_H