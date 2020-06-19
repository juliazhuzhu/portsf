#include "wave.h"

typedef struct  t_gtable
{
    double* table;
    unsigned long length;
    /* data */
}GTABLE;

typedef struct  t_tab_oscil
{
    OSCIL osc;
    const GTABLE* gtable;
    double dtablen;
    double sizeovrsr;
    /* data */
}OSCILT;


GTABLE* new_sine(unsigned long length);
void gtable_free(GTABLE** gtable);
OSCILT* new_oscilt(double srate, const GTABLE* gtable,double phase);
double tabitick(OSCILT* p_osc, double freq);
void norm_gtable(GTABLE* gtable);
GTABLE* new_gtable(unsigned long length);
GTABLE* new_triangle(unsigned long length, unsigned long nharms);
GTABLE* new_square(unsigned long length, unsigned long nharms);
GTABLE* new_pluse(unsigned long length, unsigned long nharms);
