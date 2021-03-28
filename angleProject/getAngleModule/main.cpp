#include "escapi3/escapi.cpp"
#include "prototypes.h"



int main()
{
	//Variables
	float angle;	
	imageArr bmpPixels;
	
	//make the vector the correct size
	bmpPixels.resize(HEIGHT);
	for(int i=0; i < HEIGHT; i++) {bmpPixels[i].resize(WIDTH);};

	//pointers
	float* aPtr = &angle;

	//take pictures from camera to get angle
	captureImages(bmpPixels, aPtr);
	std::cout << angle;

	delete aPtr;
	aPtr = nullptr;
	return 0;
}

//CODE FOR USB RELAY
/*
https://github.com/darrylb123/usbrelay/blob/master/README.md

http://cplusplus.com/forum/windows/29333/

https://github.com/numato/samplecode/tree/master/RelayAndGPIOModules/USBRelayAndGPIOModules/c/VC6
*/