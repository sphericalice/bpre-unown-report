.gba
.thumb

.include "macros.s"
.include "constants.s"

.open "rom.gba", "test.gba", 0x08000000

.org allocation
.area allocation_size
    .importobj "build/linked.o"
.endarea

// Update the move command which tries to set the Pokédex flag upon capture
.org gMoveCommands + 0xF1 * SIZEOF_PTR
.word atkF1_TrySetCaughtMonDexFlags |1

// Change the given item's Field Script to the Unown Report
.org gItems + ITEM_UnownReport * SIZEOF_ITEM + FIELD_SCRIPT_OFFSET
.word ItemUseOutOfBattle_UnownReport |1

.close

.definelabel gItems, readu32("rom.gba", 0x080001C8 & 0x1FFFFFF)
.definelabel gMoveCommands, readu32("rom.gba", 0x08014C1C & 0x1FFFFFF)
