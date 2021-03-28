//#include <stdio.h>
//#include <iostream>
//#include <fstream>
//#include <bitset>
//#include "bmp.h"
#include "../prototypes.h"


void makeImage (char* imageName, imageArr pixArr) 
{
	unsigned char rawImage[HEIGHT][WIDTH][BYTES_PER_PIXEL];

	for (int i = 0; i < HEIGHT; i++) 
    {
        for (int j = 0; j < WIDTH; j++) 
        {
            int xLoc = WIDTH - (j+1);
            rawImage[i][j][2] = (unsigned char)((pixArr[i][xLoc]).getRed());
            rawImage[i][j][1] = (unsigned char)((pixArr[i][xLoc]).getGreen());
            rawImage[i][j][0] = (unsigned char)((pixArr[i][xLoc]).getBlue());
        }
    }

	generateBitmapImage((unsigned char*) rawImage, imageName);
	printf("Image generated!!");
};


void generateBitmapImage (unsigned char* image, char* imageFileName)
{
    int widthInBytes = WIDTH * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader();
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    int i;
    for (i = 0; i < HEIGHT; i++) 
    {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, WIDTH, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * HEIGHT);

    static unsigned char fileHeader[] = {
        0,0,     /// signature
        0,0,0,0, /// image file size in bytes
        0,0,0,0, /// reserved
        0,0,0,0, /// start of pixel array
    };

    fileHeader[ 0] = (unsigned char)('B');
    fileHeader[ 1] = (unsigned char)('M');
    fileHeader[ 2] = (unsigned char)(fileSize      );
    fileHeader[ 3] = (unsigned char)(fileSize >>  8);
    fileHeader[ 4] = (unsigned char)(fileSize >> 16);
    fileHeader[ 5] = (unsigned char)(fileSize >> 24);
    fileHeader[10] = (unsigned char)(FILE_HEADER_SIZE + INFO_HEADER_SIZE);

    return fileHeader;
}

unsigned char* createBitmapInfoHeader ()
{
    static unsigned char infoHeader[] = {
        0,0,0,0, /// header size
        0,0,0,0, /// image WIDTH
        0,0,0,0, /// image HEIGHT
        0,0,     /// number of color planes
        0,0,     /// bits per pixel
        0,0,0,0, /// compression
        0,0,0,0, /// image size
        0,0,0,0, /// horizontal resolution
        0,0,0,0, /// vertical resolution
        0,0,0,0, /// colors in color table
        0,0,0,0, /// important color count
    };

    infoHeader[ 0] = (unsigned char)(INFO_HEADER_SIZE);
    infoHeader[ 4] = (unsigned char)(WIDTH      );
    infoHeader[ 5] = (unsigned char)(WIDTH >>  8);
    infoHeader[ 6] = (unsigned char)(WIDTH >> 16);
    infoHeader[ 7] = (unsigned char)(WIDTH >> 24);
    infoHeader[ 8] = (unsigned char)(HEIGHT      );
    infoHeader[ 9] = (unsigned char)(HEIGHT >>  8);
    infoHeader[10] = (unsigned char)(HEIGHT >> 16);
    infoHeader[11] = (unsigned char)(HEIGHT >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}


/* test pixel array
int main ()
{
    Pixel thePix;
	Pixel pixArr [250*250] = {};
    int red, green, blue;
    unsigned char image[HEIGHT][WIDTH][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "NewImage.bmp";

    std::cout << "Enter Red Value: ";
    std::cin >> red;
    std::cout << "Enter Green Value: ";
    std::cin >> green;
    std::cout << "Enter Blue Value: ";
    std::cin >> blue;

    thePix = Pixel(red, green, blue);

	int totCt = 0;
    for (int i = 0; i < HEIGHT; i++) 
    {
        for (int j = 0; j < WIDTH; j++) 
        {
			pixArr[totCt] = Pixel(thePix.getRed(),thePix.getGreen(),thePix.getBlue());

			totCt++;
        }
    }

	makeImage(imageFileName, pixArr, 250, 250);

	return 0;
}
*/