
#include "portsf.h"
#include "gtable.h"
#include <stdio.h>



int main(int argc, char* argv[]){

    PSF_PROPS props;
    unsigned long i,k;
    int ofd = - 1 ; 
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
    GTABLE* gtable = new_triangle(1024,20);//new_square(8192, 10);// new_pluse(8192,20);
    OSCILT* p_osc = new_oscilt(props.srate, gtable,0.0);


    ofd = psf_sndCreate(argv[1], &props , 0, 0 ,PSF_CREATE_RDWR);
    printf(" ofd %d \n", ofd);
    if (ofd < 0){
        printf("error create file %s\n", argv[1]);
        goto exit;
    }

    unsigned long count = 0;
    for (i=0; i < nbufs; i++){
        if (i == nbufs -1)
            nframes = remainder>0?remainder:nframes;
        for (k=0; k < nframes; k++){

            outframe[k] = tabitick(p_osc,344.53);
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

    return 1;
}