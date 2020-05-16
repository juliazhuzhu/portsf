
#include <stdio.h>
#include "portsf.h"

int main(){
	psf_init();
	
	PSF_PROPS props;
	int fd = psf_sndOpen("video_confence.wav",&props,0);
	if (fd < 0){
		printf("failed to open the file \n");
		return 1;
	}
	printf("sample rate %d \n", props.srate);
	printf(" number of channels = %d \n " , props.chans);
	if ( props.samptype == PSF_SAMP_8 ) 
		printf ( " sample type : Bbit \n " ); 
	else if ( props.samptype == PSF_SAMP_16 )
		printf ( " sample type : 16bit \n " ) ;
	psf_sndClose (fd) ;
	psf_finish();
	
	return 1;
}
