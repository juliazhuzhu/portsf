
#include <stdio.h>
typedef struct break_point {
    float time;
    float value;
}BREAKPOINT;


BREAKPOINT* get_breakpoints(FILE* fp ,unsigned long* size);