

#include "breakpoint.h"

BREAKPOINT* get_breakpoints(FILE* fp ,unsigned long* size){

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
        n++;
        ret = fscanf(fp,"%f%f\n", &time, &val);
    }

    *size = n;
    
    return points;
}


BRKSTREAM* bps_newstream(FILE* fp, unsigned long srate, unsigned long *size){
    
    BRKSTREAM* stream;
    BREAKPOINT *points;
    unsigned long npoints;

    if (srate == 0){
        printf("Error creating stream - srate can not be zero \n");
        return NULL;
    }

    stream = (BRKSTREAM*) malloc(sizeof(BRKSTREAM));
    if (stream  == NULL)
        return NULL;
    
    points = get_breakpoints(fp, &npoints);
    if (points == NULL){
        free(stream);
        return NULL;
    }

    if (stream->npoints < 2){
        free(stream);
        return NULL;
    }

    stream->points = points;
    stream->npoints = npoints;
    stream->curpos = 0.0;
    stream->ileft = 0;
    stream->iright = 1;
    stream->incr = 1.0/srate;
    stream->leftpoint = stream->points[stream->ileft];
    stream->rightpoint = stream->points[stream->iright];

    stream->width = stream->rightpoint.time - stream->leftpoint.time;
    stream->height = stream->rightpoint.value - stream->leftpoint.value;
    stream->more_points = 1;
    if (size)
        *size = stream->npoints;
    return stream;
}

void bps_freepoints(BRKSTREAM* stream){

    if (stream && stream->points){
        free(stream->points);
        stream->points = NULL;
    }
}


double bps_tick(BRKSTREAM* stream){
    double thisval, frac;

    if (stream->more_points == 0){
        return stream->rightpoint.value;
    }

    if (stream->width == 0.0)
        thisval = stream->rightpoint.value;
    else{
        frac = (stream->curpos- stream->leftpoint.value)/stream->width;
        thisval = stream->leftpoint.value + stream->height*frac;
    }
    
    stream->curpos = stream->curpos + stream->incr;
    if (stream->curpos > stream->rightpoint.time){
        stream->ileft++;
        stream->iright++;
        if (stream->iright < stream->npoints){
            stream->leftpoint = stream->points[stream->ileft];
            stream->rightpoint = stream->points[stream->iright];
            stream->width = stream->rightpoint.time - stream->rightpoint.time;
            stream->height = stream->rightpoint.value - stream->rightpoint.value;
        }
    }else
    {
            stream->more_points = 0;
    }
    return thisval;    
}