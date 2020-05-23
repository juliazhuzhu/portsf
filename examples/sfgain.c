
#include <stdio.h>
#include <math.h>
#include "portsf.h"

int main(int argc, char* argv[]){
	
	
	PSF_PROPS props;
	long framesread = 0;
	int ifd = - 1 , ofd = - 1 ; 
	int error = 0 ; 
	psf_format outformat = PSF_FMT_UNKNOWN ; 
	PSF_CHPEAK* peaks = NULL ; 
	float* in_frame = NULL ;
	float* out_frame = NULL;

	if (argc < 4){
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

	float ampfac = atoi(argv[3]);
	ampfac = ampfac/100;

	//props.chans = 1;
	//float ampfac = ( float ) pow ( 10.0 , dbval /20.0 ) ;
	printf("ampfac %s \t%f \n", argv[3],ampfac);
	props.samptype = PSF_SAMP_IEEE_FLOAT ;
	

	outformat = psf_getFormatExt (argv[2 ]) ;
	if (PSF_FMT_UNKNOWN == outformat){
		error++;
		printf("invalid output format \n");
		goto EXIT;
	}
	props.format = outformat;
	int frames = 1024;
	in_frame = (float*) malloc(props.chans*sizeof(float)* frames) ;
	props.chans = 1;
	
	out_frame = (float*) malloc(props.chans*sizeof(float)* frames) ;

	ofd = psf_sndCreate ( argv[2], &props , 0, 0 , PSF_CREATE_RDWR ) ;
	
	peaks = ( PSF_CHPEAK* ) malloc(props.chans*sizeof (PSF_CHPEAK)) ;
	framesread = psf_sndReadFloatFrames(ifd, in_frame,frames);
	printf("frameread %d \n",framesread);
	long totalread = 0;
	int i =0;
	while (framesread > 0){
		totalread++;

		//for (i = 0; i < props.chans; i++){
			int output = 0;
			for (int j = 0; j < framesread; j++){
				
				//input is a stereo type, output is mono
				//hence out frame should read a even/odd frame.
				out_frame[output++] = in_frame[j*2];//*ampfac;
				
			}
			//printf("output %d \n", output);
		//}
		if (psf_sndWriteFloatFrames(ofd, out_frame, framesread) == -1){
			printf("error in writing. \n");
			error++;
			break;
		}
		framesread = psf_sndReadFloatFrames(ifd, in_frame,frames);
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
	if (in_frame){
		free(in_frame);
	}
	if (out_frame){
		free(out_frame);
	}
	if (ifd >=0)
		psf_sndClose (ifd) ;
	if (ofd >=0)
		psf_sndClose(ofd);
	psf_finish();
	
	return error;
}
