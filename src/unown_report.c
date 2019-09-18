#include "global.h"
#include "main.h"
#include "unown_report.h"
#include <stddef.h>

void UnownReport_callback() {
    
}

void UnownReport()
{
    SetVBlankCallback(NULL);
    SetMainCallback2(sub_80CD240);
    
    return;
}
