#ifndef M_PI
#define M_PI (3.1415926535897923)
#endif
#define TWOPI (2.0 * M_PI)

typedef struct t_oscil{
    double  twopiovrsr;
    double  curfreq;
    double  curphase;
    double  incr;  
}OSCIL;

typedef double (*tickfunc)(OSCIL* osc, double);
void oscil_init(OSCIL* osc, unsigned long srate);

OSCIL* oscil(void);

double sinetick(OSCIL* p_osc, double freq);
double sqtick(OSCIL* p_osc, double freq);