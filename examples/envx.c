#include <stdio.h>
#include <math.h>
#include "portsf.h"
#define DEFAULT_WINDOW_MSECS    ( 15 )

enum {ARG_PROGNAME , ARG_INFILE , ARG_OUTFILE , ARG_WINDUR,ARG_NARGS } ;

typedef struct panpos {
        double left;
        double right;
} PANPOS;


double maxsamp ( float* buf , unsigned long blocksize ) {

    double absval , peak = 0.0 ; 
    int i ;

    for ( i=0 ; i < blocksize ; i++ ){
        absval = fabs ( buf [ i ] ) ; 
        if ( absval > peak ) 
            peak = absval ;
    }

    return peak ;
}

int main(int argc, char* argv[]){

    PSF_PROPS props;
	long framesread = 0;
	int ifd = - 1 , ofd = - 1 ; 
	int error = 0 ; 
	psf_format outformat = PSF_FMT_UNKNOWN ; 
	PSF_CHPEAK* peaks = NULL ; 
	float* inframe = NULL ;
    FILE *fp = NULL;
    unsigned long winsize ;
    double brktime ;
    double windur = DEFAULT_WINDOW_MSECS;

   
    printf ( " ENVX : extract amplitude envelope from mono soundfile\n " ) ;

    
	if (argc < ARG_NARGS){
		printf("invalid parameters \n");
		return -1;
	}
    windur = atoi(argv[ARG_WINDUR]);
    printf("windur %f \n", windur);
    psf_init();
	ifd = psf_sndOpen(argv[1],&props,0);
	if (ifd < 0){
		printf("failed to open the file \n");
		return 1;
	}
	printf("sample rate %d \n", props.srate);
	printf(" number of channels = %d \n " , props.chans);
	if ( props.samptype == PSF_SAMP_8 ) 
		printf ( " sample type : Bbit \n " ); 
	else if ( props.samptype == PSF_SAMP_16 )
		printf ( " sample type : 16bit \n " ) ;

	if (props.samptype == PSF_SAMP_IEEE_FLOAT ) {
		printf ( " Info : infile is already in floats format . \n " ) ; 
	}

    if ( props.chans > 1 ) {
        printf( " Soundfile contains %d channels must be mono . \n " , props.chans );
        error++;
        goto EXIT;
    }

    fp = fopen ( argv[ARG_OUTFILE] , "w+" ) ; 
    if ( fp == NULL ) {
        printf ( " envx : unable to create breakpoint file  %s \n" ,argv[ARG_OUTFILE]) ; 
        error++ ; 
        goto EXIT ;
    }

	int frames = 1024;
	//inframe = (float*) malloc(props.chans*sizeof(float)* frames) ;

    /*set buffersi z e to the required envelope window size */

    windur = windur/1000;
    winsize = ( unsigned long ) ( windur * props.srate ) ;
    inframe = (float*)malloc (winsize*sizeof(float) );
	long totalread = 0;
	int i =0;
    brktime = 0.0;
    printf("winsize %ld \n", winsize);
    framesread = psf_sndReadFloatFrames(ifd, inframe, winsize);
	while ( framesread > 0){

		totalread++;
		//int output_i = 0
       
        double amp ; 
        amp = maxsamp ( inframe , framesread ) ;
        if ( fprintf ( fp , "%f\t%f\n" , brktime , amp ) < 2 ) {
            printf("failed to write to brk file \n");
            error++;
            break;
        }
        brktime += windur ;
        framesread = psf_sndReadFloatFrames(ifd, inframe, winsize);
        
		//stearo, one frame has two samples
		//freadread from mono should be used her
	}
	printf("after loop frameread %d \n",framesread);
	if (framesread < 0){
		printf("Error reading file. \n");
		error++;
	}else{
		printf("Done. totoal read %u \n",totalread);
	}

	if ( psf_sndReadPeaks ( ofd, peaks , NULL ) > 0 ) {
		long i ; 
		double peaktime ;
		printf ( " PEAK information : \n " ) ;
		for ( i= 0 ; i < props . chans ; i++ ) { 
			peaktime = ( double ) peaks[ i ].pos;
			printf ( " CH %d : \t%.4f at %.4f sec s \n " , i+1 , peaks[i].val , peaktime ) ;
		}
	}
EXIT:
	if (inframe){
		free(inframe);
	}
	if (ifd >=0)
		psf_sndClose(ifd) ;
	if (ofd >=0)
		psf_sndClose(ofd);

    if ( fp ){
        if( fclose ( fp ) ) 
            printf ( " envx : failed to close outputfile %s\n", argv [ ARG_OUTFILE] );
    }

	psf_finish();
	
	return error;


    return 1;

}

