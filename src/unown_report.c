#include "global.h"
#include "unown_report.h"

void UnownReport()
{
    vblank_handler_set(0);
    set_callback2(sub_80CD240);
    
    return;
}
