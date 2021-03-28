#include <stdio.h>
#include <iostream>

const int BYTES_PER_PIXEL = 3; /// red, green, & blue
const int FILE_HEADER_SIZE = 14;
const int INFO_HEADER_SIZE = 40;

void generateBitmapImage(unsigned char* image, int height, int width, char* imageFileName);
unsigned char* createBitmapFileHeader(int height, int stride);
unsigned char* createBitmapInfoHeader(int height, int width);


class Pixel
{
    public:
        //enforce that colors are in a valid range
        char validateColorVal(char value)
        {
            if(value < 0){value = 0;};
            if(value > 255){value = 255;};
            return value;
        };
        //Default Constructor
        Pixel()   :red(0),     green(0),    blue(0){};
        //Parameter Constructor
        Pixel(  char rVal,    char gVal,  char bVal)
                :red(rVal),  green(gVal), blue(bVal){};
        //Overloaded Parameter Constructor
        Pixel(  char rVal,    char gVal,  char bVal,  char aVal)
                :red(rVal),  green(gVal), blue(bVal),alpha(aVal){};
        char getRed()                   {return this->red;};
        char getGreen()                 {return this->green;};
        char getBlue()                  {return this->blue;};
        char getAlpha()                 {return this->alpha;};
        void setRed(char newRed)        {this->red=validateColorVal(newRed);};
        void setGreen(char newGreen)    {this->green=validateColorVal(newGreen);};
        void setBlue(char newBlue)      {this->blue=validateColorVal(newBlue);};
        void setAlpha(char newAlpha)    {this->alpha=validateColorVal(newAlpha);};
        void setPixelSize(int newSize)  {this->pixelSize=newSize;};

    private:
        //values range from 0-255. 0 is absence of color 255 is saturated with color
        //char is used because they are all limited to 8bits, which is the max value of a color
        char alpha = 255;
        char red;
        char green;
        char blue;
        int pixelSize = 3;
} pixel;

int main ()
{
    Pixel thePix;
    char red, green, blue;
    int height=250, width=250;
    unsigned char image[height][width][BYTES_PER_PIXEL];
    char* imageFileName = (char*) "bitmapImage.bmp";

    std::cout << "Enter Red Value: ";
    std::cin >> red;
    std::cout << "Enter Green Value: ";
    std::cin >> green;
    std::cout << "Enter Blue Value: ";
    std::cin >> blue;

    thePix.setRed(red);
    thePix.setGreen(green);
    thePix.setBlue(blue);

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            image[i][j][2] = (unsigned char)(thePix.getRed());// ( i * 255 / height );             ///red
            image[i][j][1] = (unsigned char)(thePix.getGreen());// ( j * 255 / width );              ///green
            image[i][j][0] = (unsigned char)(thePix.getBlue());// ( (i+j) * 255 / (height+width) ); ///blue
        }
    }

    generateBitmapImage((unsigned char*) image, height, width, imageFileName);
    printf("Image generated!!");
}


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

    int i;
    for (i = 0; i < height; i++) {
        fwrite(image + (i*widthInBytes), BYTES_PER_PIXEL, width, imageFile);
        fwrite(padding, 1, paddingSize, imageFile);
    }

    fclose(imageFile);
}

unsigned char* createBitmapFileHeader (int height, int stride)
{
    int fileSize = FILE_HEADER_SIZE + INFO_HEADER_SIZE + (stride * height);

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

unsigned char* createBitmapInfoHeader (int height, int width)
{
    static unsigned char infoHeader[] = {
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