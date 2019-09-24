#ifndef GUARD_LIST_MENU_H
#define GUARD_LIST_MENU_H

u8 AddScrollIndicatorArrowPairParameterized(u32 arrowType, s32 commonPos, s32 firstPos, s32 secondPos, s32 fullyDownThreshold, s32 tileTag, s32 palTag, u16 *currItemPtr);
void RemoveScrollIndicatorArrowPair(u8 taskId);

#endif // GUARD_LIST_MENU_H