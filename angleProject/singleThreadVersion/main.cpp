#include "escapi3/escapi.cpp"
#include "prototypes.h"



int main()
{
	//Variables
	bool angleAchieved=false;	
	float angle;	
	float desiredAngle;	
	imageArr bmpPixels;
	
	//make the vector the correct size
	bmpPixels.resize(HEIGHT);
	for(int i=0; i < HEIGHT; i++) {bmpPixels[i].resize(WIDTH);};

	//pointers
	bool* aaPtr = &angleAchieved;
	float* aPtr = &angle;
	float* daPtr = &desiredAngle;

	//get the angle from the user
	desiredAngle = adjustToAngle();

	do
	{
		//take pictures from camera to get angle
		captureImages(bmpPixels, aPtr);

		//activate a switch if the angle isn't achieved
		controlSwitch(aPtr, *daPtr, aaPtr);
	} while (!angleAchieved);

	printf("\nThe Angle %3.1f has been achieved: current angle %3.1f\n",desiredAngle, angle);

	delete aaPtr;
	delete aPtr;
	delete daPtr;
	aaPtr = nullptr;
	aPtr = nullptr;
	daPtr = nullptr;
	return 0;
}

//CODE FOR USB RELAY
/*
https://github.com/darrylb123/usbrelay/blob/master/README.md

http://cplusplus.com/forum/windows/29333/

https://github.com/numato/samplecode/tree/master/RelayAndGPIOModules/USBRelayAndGPIOModules/c/VC6
*/