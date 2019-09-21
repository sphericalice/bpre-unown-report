.gba
.thumb

.include "macros.s"
.include "constants.s"

.open "rom.gba", "test.gba", 0x08000000

.org allocation
.area allocation_size
    .importobj "build/linked.o"
.endarea

.org gSpecials + SPECIAL_UnownReport * SIZEOF_PTR
.word Special_ShowUnownReport |1

.org gMoveCommands + 0xF1 * SIZEOF_PTR
.word atkF1_trysetcaughtmondexflags |1

.close
