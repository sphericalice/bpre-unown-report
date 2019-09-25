#ifndef GUARD_STRINGS_H
#define GUARD_STRINGS_H

#include "constants/flags.h"

struct ReportPage {
    const u8 *str;
    const u16 flag;
};

extern const u8 gText_PlayersUnownReport[];
extern const u8 gText_CurrentKinds[];
extern const u8 gText_Instructions[];
extern const u8 gText_UnownA[];
extern const u8 gText_UnownB[];
extern const u8 gText_UnownC[];
extern const u8 gText_UnownD[];
extern const u8 gText_UnownE[];
extern const u8 gText_UnownF[];
extern const u8 gText_UnownG[];
extern const u8 gText_UnownH[];
extern const u8 gText_UnownI[];
extern const u8 gText_UnownJ[];
extern const u8 gText_UnownK[];
extern const u8 gText_UnownL[];
extern const u8 gText_UnownM[];
extern const u8 gText_UnownN[];
extern const u8 gText_UnownO[];
extern const u8 gText_UnownP[];
extern const u8 gText_UnownQ[];
extern const u8 gText_UnownR[];
extern const u8 gText_UnownS[];
extern const u8 gText_UnownT[];
extern const u8 gText_UnownU[];
extern const u8 gText_UnownV[];
extern const u8 gText_UnownW[];
extern const u8 gText_UnownX[];
extern const u8 gText_UnownY[];
extern const u8 gText_UnownZ[];
extern const u8 gText_UnownQuestionMark[];
extern const u8 gText_UnownExclamationMark[];
extern const u8 gText_Report1[];
extern const u8 gText_Report2[];
extern const u8 gText_Report3[];
extern const u8 gText_Report4[];
extern const u8 gText_Report5[];
extern const u8 gText_Report6[];

static const u8 *UnownStrings[] = {
    gText_UnownA, gText_UnownB, gText_UnownC, gText_UnownD, gText_UnownE,
    gText_UnownF, gText_UnownG, gText_UnownH, gText_UnownI, gText_UnownJ,
    gText_UnownK, gText_UnownL, gText_UnownM, gText_UnownN, gText_UnownO,
    gText_UnownP, gText_UnownQ, gText_UnownR, gText_UnownS, gText_UnownT,
    gText_UnownU, gText_UnownV, gText_UnownW, gText_UnownX, gText_UnownY,
    gText_UnownZ, gText_UnownExclamationMark, gText_UnownQuestionMark
};

const struct ReportPage ReportPages[] = {
    { gText_Report1, 0 }, // 1st report page flag isn't checked
    { gText_Report2, FLAG_REPORT2 },
    { gText_Report3, FLAG_REPORT3 },
    { gText_Report4, FLAG_REPORT4 },
    { gText_Report5, FLAG_REPORT5 },
    { gText_Report6, FLAG_REPORT6 },
};

#define LAST_PAGE (4 + (sizeof(ReportPages) / sizeof(struct ReportPage)))

#endif // GUARD_STRINGS_H