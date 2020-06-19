

#include "wave.h"
#include "portsf.h"
#include "breakpoint.h"
#include <stdio.h>

int main(int argc, char* argv[]){

    OSCIL **oscs = NULL;
    double *oscamps = NULL;
    double *oscfreqs = NULL;
    PSF_PROPS props;
    unsigned long i = 0, j = 0;
    int ofd = - 1 ; 
    double freq = 0.0, amp = 0.0;
    unsigned long noscs;
    noscs = 10;
    double ampfac , freqfac , ampadjust;
    freqfac = 1.0 ; ampadjust = 0.0 ;
   

    oscamps = (double*) malloc(noscs* sizeof(double));
    oscfreqs = (double*) malloc(noscs* sizeof(double));

    oscs = (OSCIL**) malloc (noscs* sizeof(OSCIL*));
    
    for (i =0; i < noscs; i++){
        ampfac = 1.0/ freqfac;
        oscamps[i] = ampfac;
        oscfreqs[i] = freqfac;
        freqfac += 2.0;
        ampadjust += ampfac;
    }

    psf_init();
    unsigned long nbufs,outframes,remainder ;
    unsigned long nframes = 100;
    double dur = 9;
    outframes = ( unsigned long )(44100*dur + 0.5);
    nbufs = outframes/nframes;
    float* outframe = (float*) malloc(nframes*sizeof(float));
    remainder = outframes - nbufs * nframes;
    if ( remainder > 0 ) 
        nbufs++ ;

    //oscil_init(p_osc,44100);
    props.srate = 44100;
    props.chans = 1;
    //props.samptype = PSF_SAMP_16;
    props.samptype = PSF_SAMP_IEEE_FLOAT;
    props.format = PSF_STDWAVE;

   


    ofd = psf_sndCreate(argv[1], &props , 0, 0 ,PSF_CREATE_RDWR);
    printf(" ofd %d \n", ofd);
    if (ofd < 0){
        printf("error create file %s\n", argv[1]);
        goto exit;
    }

    for (i=0; i < noscs; i++){
        oscs[i] = new_oscil(0.0,props.srate);
    }
    FILE* fp = fopen("points.brk","r");
    int size;
    BRKSTREAM* freqstream = bps_newstream(fp,props.srate,&size);
    BRKSTREAM* ampstream = bps_newstream(fp,props.srate,&size);

    for (i=0; i < nbufs; i++){
        if (i == nbufs -1)
            nframes = remainder>0?remainder:nframes;
        for (j=0; j < nframes; j++){

            long k;
            if (freqstream)
                freq = bps_tick(freqstream);
            if (ampstream)
                amp = bps_tick(ampstream);

            double val = 0.0;
            for (k =0; k < noscs; k++){
                val += oscamps[k] * sinetick(oscs[k], 0.5*oscfreqs[k]);
            }

            outframe[j] = (float)(val* 0.5);
        }

        if (psf_sndWriteFloatFrames(ofd, outframe, nframes) != nframes){
            printf("error writing file \n");
            break;
        }
    }

exit:
    if (ofd >=0)
		psf_sndClose(ofd);

    psf_finish();


    double phase = 1.60;
    unsigned long base = (int) phase;

    printf("base %lu \n", base);

    return 1;
}