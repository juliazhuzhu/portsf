

#include "breakpoint.h"

BREAKPOINT* get_breakpoints (FILE* fp ,unsigned long* size){

    float time, val;
    int ret;
    ret = fscanf(fp,"%f%f\n", &time, &val);
    int n = 0;
    BREAKPOINT* points = (BREAKPOINT*)malloc(sizeof(BREAKPOINT)*4096);
    //BREAKPOINT** addr_points = &points;
    while (ret > 0){
        BREAKPOINT* point = &points[n];
        point->time = time;
        point->value = val;
        if (n == 1) {
         printf("%f %f \n", points[n].time, val);
        }
        n++;
        ret = fscanf(fp,"%f%f\n", &time, &val);
    }

    *size = n;
    
    return points;
}