#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include <math.h>
#include <utility>
#include <unistd.h>
#include <mutex>
#include <thread>




const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;
const int WIDTH = 500;
const int HEIGHT = 500;

class Pixel
{
    public:
        //enforce that colors are in a valid range
        int validateColorVal(int value)
        {
            if(value < 0){value = 0;};
            if(value > 255){value = value % 255;};
            return value;
        };
        //Default Constructor
        Pixel()   :red(0),     green(0),    blue(0){};
        //Parameter Constructor
        Pixel(  int rVal,    int gVal,  int bVal)
                :red(rVal),  green(gVal), blue(bVal){};
        //Overloaded Parameter Constructor
        Pixel(  int rVal,    int gVal,  int bVal,  int aVal)
                :red(rVal),  green(gVal), blue(bVal),alpha(aVal){};
		//Overloaded Paramter Constructor
		Pixel(int pixelAsInt)
		{
			size_t bytesPerInt = sizeof(int);
			for (int i = 0; i < bytesPerInt; i++)
			{
				size_t shift = 8 * (bytesPerInt - 1 - i);
				switch(i)
				{
					case 0: //0-7 bits are alpha
						this->alpha = (pixelAsInt >> shift) & 0xff;
					break;
					case 1: //8-15 bits are red
						this->red = (pixelAsInt >> shift) & 0xff;
					break;
					case 2: //16-23 bits are green
						this->green = (pixelAsInt >> shift) & 0xff;
					break;
					case 3: //24-31 bits are blue
						this->blue = (pixelAsInt >> shift) & 0xff;
					break;
				}
			}
		}
		Pixel operator=(Pixel otherPix){this->setRGB(otherPix.getRed(),otherPix.getGreen(), otherPix.getBlue()); return *this;};
        int getRed()                   				{return (int)this->red;};
        int getGreen()                 				{return (int)this->green;};
        int getBlue()                  				{return (int)this->blue;};
        int getAlpha()                 				{return (int)this->alpha;};
		void setRGB(int newR, int newG, int newB)	{red=validateColorVal(newR);green=validateColorVal(newG);blue=validateColorVal(newB);};
        void setRed(int newRed)        				{this->red=validateColorVal(newRed);};
        void setGreen(int newGreen)    				{this->green=validateColorVal(newGreen);};
        void setBlue(int newBlue)      				{this->blue=validateColorVal(newBlue);};
        void setAlpha(int newAlpha)    				{this->alpha=validateColorVal(newAlpha);};
        void setPixelSize(int newSize)  			{this->pixelSize=newSize;};

    private:
        //values range from 0-255. 0 is absence of color 255 is saturated with color
        //char is used because they are all limited to 8bits, which is the max value of a color
        int alpha = 255;
        int red;
        int green;
        int blue;
        int pixelSize = 3;
};


//This typedef must come after Pixel is defined
typedef std::vector<std::vector<Pixel>> imageArr;

void generateBitmapImage(unsigned char* image, char* imageFileName);
unsigned char* createBitmapFileHeader(int stride);
unsigned char* createBitmapInfoHeader();
void makeImage (char* imageName, imageArr pixArr);
float getAngle (imageArr& pixArr, bool drawLine=true, bool showStats=true);
void toBlackAndWhite(imageArr& pixArr, int maxColorLimit=75, int trimAmount=10);
void makeLine(imageArr& pixArr, float yIntercept, float slope, Pixel lineColor, int lineSize=5);
//void controlSwitch(float curAngle, float desiredAngle, bool& angleAchieved); //originally for single thread design
//void captureImages (imageArr& bmpPixels, bool& angleAchieved, float& angle); //originally for single thread design
void controlSwitch(float* curAngle, float desiredAngle, bool* angleAchieved);
void captureImages (imageArr bmpPixels, float* angle);
float adjustToAngle();
