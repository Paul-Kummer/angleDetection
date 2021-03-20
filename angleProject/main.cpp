#include "escapi3/escapi.cpp"
#include "prototypes.h"



int main()
{
	//Variables
	bool angleAchieved=false;	
	float angle;	
	float desiredAngle=0;	
	imageArr bmpPixels;
	
	//make the vector the correct size
	bmpPixels.resize(HEIGHT);
	for(int i=0; i < HEIGHT; i++) {bmpPixels[i].resize(WIDTH);};

	//pointers
	bool* aaPtr = &angleAchieved;
	float* aPtr = &angle;
	float* daPtr = &desiredAngle;
	//imageArr* pixArrPtr = &bmpPixels;



	//get the angle from the user
	desiredAngle = adjustToAngle();
/*
	//take pictures from camera to get angle
	captureImages(bmpPixels, aaPtr, aPtr);

	//activate a switch if the angle isn't achieved
	controlSwitch(angle, desiredAngle, aaPtr);
*/

	//thread cannot use references, switch to pointers
	///*
	std::thread thOne(captureImages, bmpPixels, aaPtr, aPtr);
	usleep(5000000);//give it time to set the current angle
	std::thread thTwo(controlSwitch, aPtr, *daPtr, aaPtr);
	thOne.join();
	thTwo.join();
	//*/

	printf("\n\n\t--=Goodbye!=---\n");
	return 0;
}