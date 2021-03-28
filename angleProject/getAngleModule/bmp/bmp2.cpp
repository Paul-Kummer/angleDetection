#pragma once

#include "../prototypes.h"


const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;


void generateBitmapImage (unsigned char* image, int height, int width, char* imageFileName)
{
    int widthInBytes = width * BYTES_PER_PIXEL;

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (widthInBytes) % 4) % 4;

    int stride = (widthInBytes) + paddingSize;

    FILE* imageFile = fopen(imageFileName, "wb");

    unsigned char* fileHeader = createBitmapFileHeader(height, stride);
    fwrite(fileHeader, 1, FILE_HEADER_SIZE, imageFile);

    unsigned char* infoHeader = createBitmapInfoHeader(height, width);
    fwrite(infoHeader, 1, INFO_HEADER_SIZE, imageFile);

    for (int i = 0; i < height; i++) 
	{
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

    static unsigned char fileHeader[] = 
	{
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

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = 
	{
        0,0,0,0, /// header size
        0,0,0,0, /// image width
        0,0,0,0, /// image height
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
    infoHeader[ 4] = (unsigned char)(width      );
    infoHeader[ 5] = (unsigned char)(width >>  8);
    infoHeader[ 6] = (unsigned char)(width >> 16);
    infoHeader[ 7] = (unsigned char)(width >> 24);
    infoHeader[ 8] = (unsigned char)(height      );
    infoHeader[ 9] = (unsigned char)(height >>  8);
    infoHeader[10] = (unsigned char)(height >> 16);
    infoHeader[11] = (unsigned char)(height >> 24);
    infoHeader[12] = (unsigned char)(1);
    infoHeader[14] = (unsigned char)(BYTES_PER_PIXEL*8);

    return infoHeader;
}


/*
//pixelHeight: 			the number of pixels spanning from top to bottom of the image
//pixelWidth: 			the number of pixels spanning from left to right of the image
//bytesPerPixel:		the number of bytes in one pixel. The pixel is typically 1byte:red, 1byte:green, 1byte:blue.
//						However, sometimes there is an alpha channel as well, so it could be 4 bytes.
//sizeOfMetadataBytes:	the size of the bmpHeader, bmpInfoHeader, 

struct BmpHeader //38bytes
{
	uint32_t pixelHeight=0, pixelWidth=0, bytesPerPixel=3, sizeOfThisHeader=38, infoHeaderBytes=40; 		//4*5 = 20bytes
	uint32_t sizeOfMetadataBytes = sizeOfThisHeader + infoHeaderBytes;									//4bytes
	char bitmapSignatureBytes[2] = {'B', 'M'};															//2bytes
	uint32_t sizeOfBitmapFile = (pixelHeight * pixelWidth * bytesPerPixel) + sizeOfMetadataBytes;		//4bytes
	uint32_t reservedBytes = 0;																			//4bytes
	uint32_t pixelDataOffset = sizeOfMetadataBytes;														//4bytes
} bmpHeader;																					//Total = 38bytes



struct BmpInfoHeader //40bytes
{
	uint32_t sizeOfThisHeader = 40; 								//4bytes
	int32_t width = 25; //pixels									//4bytes	
	int32_t height = 25; //pixels									//4bytes
	uint16_t numberOfColorPlanes = 1; //must be 1					//2bytes
	uint16_t colorDepth = 24;										//2bytes
	uint32_t compressionMethod = 0;									//4bytes
	uint32_t rawBitmapDataSize = 0; //generally ignored				//4bytes
	int32_t horizontalResolution = 3780; //pixel per meter			//4btyes
	int32_t verticalResolution = 3780; //pixel per meter			//4bytes
	uint32_t colorTableEntries = 0;									//4bytes
	uint32_t importantColors = 0;									//4bytes
} bmpInfoHeader;											//Total = 40bytes



struct Pixel
{
	//values range from 0-255. 0 is absence of color 255 is saturated with color
	//uint8_t alpha = 255;
	uint8_t red = 255;
	uint8_t green = 255;
	uint8_t blue = 255;
	uint8_t pixelSize = 3;
} pixel;





/* References *
C++: How to Write a Bitmap Image from Scratch. By: M Muiz Hassan
https://dev.to/muiz6/c-how-to-write-a-bitmap-image-from-scratch-1k6m
*/