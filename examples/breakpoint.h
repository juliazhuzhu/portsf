
#include <stdio.h>
typedef struct break_point {
    float time;
    float value;
}BREAKPOINT;

typedef struct breakpoint_stream {
    BREAKPOINT* points;
    BREAKPOINT leftpoint,rightpoint;
    unsigned long npoints;
    double curpos;
    double incr;
    double width;
    double height;
    unsigned long ileft, iright;
    int more_points;

}BRKSTREAM;

BRKSTREAM* bps_newstream(FILE* fp, unsigned long srate, unsigned long *size);

BREAKPOINT* get_breakpoints(FILE* fp ,unsigned long* size);

void bps_freepoints(BRKSTREAM* stream);

double bps_tick(BRKSTREAM* stream);