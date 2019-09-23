#ifndef GUARD_EVENT_DATA_H
#define GUARD_EVENT_DATA_H

#include "constants/flags.h"
#include "constants/vars.h"

u16 VarGet(u16 id);
bool8 VarSet(u16 id, u16 value);
bool8 FlagGet(u16 id);

#endif // GUARD_EVENT_DATA_H