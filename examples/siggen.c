
enum {ARG_PROGNAME , ARG_OUTFILE , ARG_DUR , ARG_SRATE , ARG_AMP , ARG_FREQ , ARG_NARGS } ;
#include <stdio.h>
#include "portsf.h"
#include "wave.h"

int main(int argc, char* argv[]){

    PSF_PROPS props;
    int ofd = - 1 ; 
    unsigned long nbufs,outframes,remainder ;
    unsigned long nframes = 100;
    double dur = 9;
    outframes = ( unsigned long )(44100*dur + 0.5);
    nbufs = outframes/nframes;
    int i = 0, j = 0;
    double amp = 0.5;
    
    tickfunc tick;
    tick = sqtick;
    remainder = outframes - nbufs * nframes;
    if ( remainder > 0 ) 
        nbufs++ ;

    float* outframe = (float*) malloc(nframes*sizeof(float));

    printf("%lu %lu %lu \n", outframes,remainder,nbufs);

    psf_init();
    OSCIL* p_osc = new_oscil(0.0);
    oscil_init(p_osc,44100);
    props.srate = 44100;
    props.chans = 1;
    props.samptype = PSF_SAMP_16;
    props.samptype = PSF_SAMP_IEEE_FLOAT;
    props.format = PSF_STDWAVE;
    int freq = 10;
    ofd = psf_sndCreate(argv[ARG_OUTFILE], &props , 0, 0 ,PSF_CREATE_RDWR);

    for (i=0; i < nbufs; i++){
        if (i == nbufs -1)
            nframes = remainder>0?remainder:nframes;
        for (j=0; j < nframes; j++){
            outframe[j] = (float)(amp*tick(p_osc, freq));
         //   printf(" %f\t", outframe[j]);
        }
//        //printf("\n");
        if (psf_sndWriteFloatFrames(ofd, outframe, nframes) != nframes){
            printf("error writing file \n");
            break;
        }

    }



    
    if (ofd >=0)
		psf_sndClose(ofd);
    if (p_osc)
        free(p_osc);

    psf_finish();
    return 1;

}