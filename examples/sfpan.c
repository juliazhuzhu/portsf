#include <stdio.h>
#include "portsf.h"

enum {ARG_PROGNAME , ARG_INFILE , ARG_OUTFILE , ARG_PANPOS , ARG_NARGS } ;

typedef struct panpos {
        double left;
        double right;
} PANPOS;

PANPOS simplepan(double position){

    PANPOS pos;

    position *= position;
    pos.left = position - 0.5;
    pos.right = position + 0.5;

    return pos;
}

int main(int argc, char* argv[]){

    PSF_PROPS props;
	long framesread = 0;
	int ifd = - 1 , ofd = - 1 ; 
	int error = 0 ; 
	psf_format outformat = PSF_FMT_UNKNOWN ; 
	PSF_CHPEAK* peaks = NULL ; 
	float* frame = NULL ;

    
	if (argc < ARG_NARGS){
		printf("invalid parameters \n");
		return -1;
	}

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
	
	//float ampfac = ( float ) pow ( 10.0 , dbval /20.0 ) ;
	props.samptype = PSF_SAMP_IEEE_FLOAT ;
	

	outformat = psf_getFormatExt (argv[2 ]) ;
	if (PSF_FMT_UNKNOWN == outformat){
		error++;
		printf("invalid output format \n");
		goto EXIT;
	}
	props.format = outformat;
	int frames = 1024;
	frame = (float*) malloc(props.chans*sizeof(float)* frames) ;
	props.chans = 2;
	ofd = psf_sndCreate ( argv[2], &props , 0, 0 , PSF_CREATE_RDWR ) ;
	float* output_frame = (float*) malloc(props.chans*sizeof(float)* frames) ;
	peaks = ( PSF_CHPEAK* ) malloc(props.chans*sizeof (PSF_CHPEAK)) ;
	framesread = psf_sndReadFloatFrames(ifd, frame,frames);
	printf("frameread %d \n",framesread);
	long totalread = 0;
	int i =0, output_i = 0;
    PANPOS thispos = simplepan(1);
	while (framesread > 0){
		totalread++;
		//int output_i = 0
        for (int j = 0; j < framesread; j++){
            //printf("before %d \n", fabs(frame[i*framesread+j]));
            output_frame[output_i++] = (float)frame[j]*thispos.left;
            output_frame[output_i++] = (float)frame[j]*thispos.right;
            //printf("after %f \n", fabs(frame[i*framesread+j]));
			
        }
		//stearo, one frame has two samples
		//freadread from mono should be used here
		if (psf_sndWriteFloatFrames(ofd, output_frame, framesread) == -1){
			printf("error in writing. \n");
			error++;
			break;
		}
		output_i = 0;
		framesread = psf_sndReadFloatFrames(ifd, frame,frames);
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
	if (frame){
		free(frame);
	}
	if (ifd >=0)
		psf_sndClose (ifd) ;
	if (ofd >=0)
		psf_sndClose(ofd);
	psf_finish();
	
	return error;


    return 1;

}

