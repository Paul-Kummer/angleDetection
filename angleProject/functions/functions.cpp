#include "..\prototypes.h"
#include "..\escapi3\escapi.h"





//for purpose of line drawing, the bottom left of the image is (0,0)
//draw x and y are both needed for cases of vertical and horizxontal lines
void makeLine(imageArr& pixArr, float yIntercept, float slope, Pixel lineColor, int lineSize)
{
	for(int xIndex = 0; xIndex < WIDTH; xIndex++)
	{
		int yLineSize = lineSize + (         (        (abs(slope) /(float)1000)    * HEIGHT     )          ); //Adjust the marker line for near vertical lines
		int y = (int)((slope * (float)xIndex) + yIntercept);
		int yIndex = HEIGHT-y;
		if(yIndex > 0 && yIndex < HEIGHT); //Make sure the point is on the image
		{
			//span the lineSize on the x axis of both sides of the point
			for(int aroundX = xIndex-lineSize; aroundX < xIndex+lineSize; aroundX++) 
			{
				//span the lineSize on the y axis of both sides of the point
				for(int aroundY = yIndex-yLineSize; aroundY < yIndex+yLineSize; aroundY++) 
				{		
					//ensure the pixels to change are on the image plane	
					if(aroundX > 0 && aroundX < WIDTH && aroundY > 0 && aroundY < HEIGHT) 
					{
						pixArr[aroundY][aroundX] = lineColor; //change the color of pixels on the path of the line
					}
				}
			}
		}
	}
}



//Method: Line of Best Fit (Least Square Method)
float getAngle (imageArr& pixArr, bool drawLine, bool showStats)
{
	float slope, angle, yIntercept;
	float ptCount;
	float xSum, xAvg, xSqrSum;
	float ySum, yAvg, ySqrSum;
	float xMinusXAvgTimesYMinusYAvgSum, xMinusXAvgSqrSum;
	float yMinusYAvgSqrSum;
	std::vector<std::pair<int,int>> pointArray;

	//statistical analysis
	float errorSumOfSquares;
	float totalSumOfSquares;
	float regressionSumOfSquares;
	float rSquared;
	float goodnessOfFit;
	float pearsonCoefficient;

	for(float y = 0; y < HEIGHT; y++) //HEIGHT
	{
		for(float x = 0; x < WIDTH; x++) //WIDTH
		{
			if(pixArr[y][x].getRed()==0) //all three colors will be 0 for black, so only one needs to be checked
			{
				int xPos = x, yPos = (HEIGHT - y); // y position has to be adjusted for top down approach
				pointArray.push_back(std::make_pair(xPos,yPos));
				xSum += x;
				ySum += yPos;
				xSqrSum += pow(x,2);
				ySqrSum += pow(yPos, 2);
				ptCount++;
			}
		}
	}

	//prevent divide by zero, and set to resonable value if ptCount is zero
	xAvg = ptCount>0?xSum/ptCount:0;
	yAvg = ptCount>0?ySum/ptCount:1;

	//determine the line of best fit for the points
	for(int i = 0; i < pointArray.size(); i++)
	{
		float x = (float)pointArray[i].first, y = (float)pointArray[i].second;
		xMinusXAvgTimesYMinusYAvgSum += ((x - xAvg)*(y - yAvg));
		xMinusXAvgSqrSum += pow((x - xAvg), 2);
	}

	//if the denominator is zero, the line is vertical and a slope of infinity
	slope = xMinusXAvgSqrSum!=0?(xMinusXAvgTimesYMinusYAvgSum / xMinusXAvgSqrSum):WIDTH*2; 
	yIntercept = yAvg - (slope * xAvg);

	//multiply by -1 because the image is read backwards
	angle = -1*((atan(slope) * 180) / 3.14159265);

	if(showStats)
	{
		float residual, residualSquared;
		float yMinusYBar, yMinusYBarSquared;
		for(int i = 0; i < pointArray.size(); i++)
		{
			float xActual = pointArray[i].first, yActual = pointArray[i].second;
			float yPredicted = (slope * xActual) + yIntercept;

			residual += (yActual-yPredicted);
			residualSquared += pow(residual,2);

			yMinusYBar += (yActual-yAvg);
			yMinusYBarSquared += pow(yMinusYBar, 2);
		}

		//R^2 = SSR/SST || 1-SSE/SST || SSR/(SSR+SSE)
		rSquared = 1 - (residualSquared/yMinusYBarSquared);
		goodnessOfFit = 100 * rSquared;
		
		printf("Image Width(pixels): %i\n", WIDTH);
		printf("Image Height(pixels): %i\n", HEIGHT);
		printf("y Average: %5.0f\n", yAvg);
		printf("x Average: %5.0f\n", xAvg);
		printf("Sum Squared Regression(SSR): %5.2f\n",residualSquared);
		printf("Total Sum of Squares(SST): %5.2f\n",yMinusYBarSquared);
		printf("The Line Fits: %3.0f Percent | R^2: %1.6f\n", goodnessOfFit, rSquared);
		printf("The Slope is: %3.4f\n", slope);
		printf("The Angle is: %3.1f\n", angle);
	}

	//this will add a line of best fit to the bmp created
	if(drawLine)
	{
		Pixel lineColor = Pixel(0,255,0); //make the line of best fit green
		makeLine(pixArr, yIntercept, slope, lineColor, (int)1);
	}
	return angle;
};




void toBlackAndWhite(imageArr& pixArr, int maxColorLimit, int trimAmount)
{
	for(int y = 0; y < HEIGHT; y++) //HEIGHT
	{
		for(int x = 0; x < WIDTH; x++) //WIDTH
		{
			pixArr[y][x];
			//when all the colors are the same value, a shade of grey will be produced
			int avgColor = ((pixArr[y][x].getRed() + pixArr[y][x].getGreen() + pixArr[y][x].getBlue())/3); // (red + green + blue)/3 = 0-255
			avgColor = avgColor<maxColorLimit?0:255; //remove all colors that don't meet the threshold, and make all other color black
			if(y > trimAmount && x > trimAmount && y < (HEIGHT-trimAmount) && x < (WIDTH-trimAmount)) //trim off 10 pixels around the border
			{
				pixArr[y][x].setRGB(avgColor, avgColor, avgColor);
			}
			else
			{
				pixArr[y][x].setRGB(255, 255, 255);
			}
		}
	}
};




//This should be it's own thread
void controlSwitch(float* curAngle, float desiredAngle, bool* angleAchieved)
{
	bool switchOne, switchTwo, setGuard=true;
	float deltaAngle = desiredAngle - *curAngle;
	
	//determine what switch if any should be run. That way they cannot alternate back and forth if they overshoot.
	if(deltaAngle > -4 && deltaAngle < 4)
	{
		if(setGuard)
		{
			*angleAchieved = true;
		};
		switchOne = false;
		switchTwo = false;
	}
	else if(deltaAngle < 0)
	{
		printf("\nTurning On Switch One\n");
		switchOne = true;
		switchTwo = false;
	}
	else
	{
		printf("\nTurning On Switch Two\n");
		switchOne = false;
		switchTwo = true;
	}

	do
	{
		float deltaAngle = desiredAngle - *curAngle;
		if(switchOne)
		{
			//guard angle
			*angleAchieved = (deltaAngle >= 0) ? true : false;
			//move down
		}
		else if(switchTwo)
		{
			//guard angle
			*angleAchieved = (deltaAngle <= 0) ? true : false;
			//move up
		}
	} while (!*angleAchieved);
	printf("\nThe Angle %3.1f has been achieved: current angle %3.1f\n",desiredAngle, *curAngle);
};




//this should be its own thread
void captureImages (imageArr bmpPixels, bool *angleAchieved, float* angle)
{
	char* imageName = (char*)"testImage.bmp";
	bool atAngle, setGuard=true;

	/* Initialize ESCAPI */
	int devices = setupESCAPI();
	setCaptureProperty(0,13,3,0); //look in escapi.h for propery values

	if (devices == 0)
	{
		printf("ESCAPI initialization failure or no devices found.\n");
	}

	struct SimpleCapParams capture;
	capture.mWidth = WIDTH;
	capture.mHeight = HEIGHT;
	capture.mTargetBuf = new int[HEIGHT * WIDTH];
	
	
	if (initCapture(0, &capture) == 0) //0 is device one
	{
		printf("Capture failed - device may already be in use.\n");
	};

	do
	{
		if(setGuard)
		{
			atAngle = *angleAchieved;
		};

		//request a capture      
		doCapture(0);
			
		while (isCaptureDone(0) == 0)
		{
			// Wait until capture is done.       
		};

		//each index an int consisting of 4 bytes, alpha(1byte), red(1byte), green(1byte), blue(1byte)
		int count = 0;
		for(int y = 0; y < HEIGHT; y++) //HEIGHT
		{
			for(int x = 0; x < WIDTH; x++) //WIDTH
			{
				//(HEIGHT-y)*WIDTH will give the posision of the row that is being iterated
				//(WIDTH-x) will give the current position within the row
				bmpPixels[y][x] = Pixel(capture.mTargetBuf[((HEIGHT-y)*WIDTH)+(WIDTH-x)]);
				count++;
			};
		};

		//clean up the image and make it black and white
		toBlackAndWhite(bmpPixels);

		//put lock guard on the angle while it's changed
		if(setGuard)
		{
			*angle = getAngle(bmpPixels, true, true); //returns the angle and takes input of pixel vector, draw line of best fit, and show statistics of line
		};
		
		//create an image of the current capture and put a line of best fit drawn over it
		//makeImage(imageName, bmpPixels); //This will save an image of what was just caputured in bmp format

		//pauses between capures, in micro seconds
		usleep(2000000); 
	}while(!atAngle);
	deinitCapture(0);
};



float adjustToAngle()
{
	float angleToAdjustTo;
	std::cout << "What Angle Do You Desire?: ";
	std::cin >> angleToAdjustTo; 
	return angleToAdjustTo;
};