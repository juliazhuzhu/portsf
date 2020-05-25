#include <stdio.h>
#include "portsf.h"
#include "breakpoint.h"

enum {ARG_PROGNAME , ARG_INFILE , ARG_BRK , ARG_OUTFILE , ARG_NARGS } ;

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
	ifd = psf_sndOpen(argv[ARG_INFILE],&props,0);
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
	

	int npoints = 0;
	FILE* fp = fopen(argv[ARG_BRK],"r");
	if (fp == NULL){
		printf("failure to open breakpoints file \n");
		return -1;
	}

	BREAKPOINT* points = get_breakpoints(fp,&npoints);
	printf("totoal points %lu \n", npoints);

	double incr= 1.0/props.srate ; 
	double curpos = 0.0 ; 
	unsigned long ileft = 0 ; 
	unsigned long iright = 1 ;
	BREAKPOINT* leftpoint = &points[0] ; 
	BREAKPOINT* rightpoint = &points[1] ; 
	double width = rightpoint->time - leftpoint->time ; 
	//printf("rightpoint %f, leftpoint %f \n", rightpoint->time ,points[0].time);
	double height = rightpoint->value - leftpoint->value ;
	//printf("width %f, height %f \n", width,height);
	double thisamp = 0.0;
	double frac = 0.0;

	//int frames = 1024;
	unsigned long windur = 15;//1000ms
	windur = windur/1000;
    unsigned long frames = 1024;//( unsigned long ) ( windur * props.srate ) ;
	frame = (float*) malloc(props.chans*sizeof(float)* frames) ;
	//props.chans = 1;
	ofd = psf_sndCreate ( argv[ARG_OUTFILE], &props , 0, 0 , PSF_CREATE_RDWR ) ;
	float* output_frame = (float*) malloc(props.chans*sizeof(float)* frames) ;
	peaks = ( PSF_CHPEAK* ) malloc(props.chans*sizeof (PSF_CHPEAK)) ;
	framesread = psf_sndReadFloatFrames(ifd, frame,frames);
	printf("frameread %d \n",framesread);
	long totalread = 0;
	int i =0;
	int more_points = 1;
    
	while (framesread > 0){
		totalread++;
		
		//stearo, one frame has two samples
		//freadread from mono should be used here
		for (int i =0 ; i < framesread; i++){
			if (more_points > 0){
				if (width == 0)
					thisamp = rightpoint->value;
				else {
					frac = (curpos - leftpoint->time)/width;
					thisamp = leftpoint->value + frac * height;
				}
		
				curpos += incr;
				if (curpos > rightpoint->time){
					ileft++; iright++;
					if (iright < npoints){
						leftpoint = &points[ileft];
						rightpoint = &points[iright];
						width = rightpoint->time - leftpoint->time;
						height = rightpoint->value - leftpoint->value;
						//printf("rightpoint %f, leftpoint %f \n", rightpoint->time ,leftpoint->time);
					}else{
						more_points = 0;
					}
				}
			}
			frame[i] = (float)frame[i]*thisamp;

		}
		if (psf_sndWriteFloatFrames(ofd, frame, framesread) == -1){
			printf("error in writing. \n");
			error++;
			break;
		}
		
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

