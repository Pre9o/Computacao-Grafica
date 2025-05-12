#ifndef ___BMP__H___
#define ___BMP__H___

#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <string.h>
#include <vector>

#define HEADER_SIZE      
#define INFOHEADER_SIZE  
#define uchar unsigned char

typedef struct {
   unsigned short int type;                 
   unsigned int size;                       
   unsigned short int reserved1, reserved2;
   unsigned int offset;                     
} HEADER;


typedef struct {
   unsigned int size;                  
   int width, height;                 
   unsigned short int planes;         
   unsigned short int bits;           
   unsigned int compression;         
   unsigned int imagesize;             
   int xresolution, yresolution;       
   unsigned int ncolours;              
   unsigned int impcolours;           
} INFOHEADER;


class Bmp
{
private:
   int width, height, imagesize, bytesPerLine, bits;
   unsigned char *data;

   HEADER     header;
   INFOHEADER info;

   void load(const char *fileName);
   unsigned char* originalData;

   unsigned char* originalImage = nullptr; // Armazena a imagem original
   int originalWidth = 0;                  // Largura original
   int originalHeight = 0;                 // Altura original

public:
   Bmp(const char *fileName);
   uchar* getImage();
   void   setImage(uchar* img);
   int    getWidth(void);
   void   setWidth(int w);
   int    getHeight(void);
   void   setHeight(int h);
   void   convertBGRtoRGB(void);
   void   adjustBrightness(int brightness);
   void   adjustContrast(float contrast);
   int    contains(int x, int y);
   void   flipVertical(void);
   void   flipHorizontal(void);
   void   image_R(void);
   void   image_G(void);
   void   image_B(void);
   void   image_Gray(void);
   int    x_start, y_start, x_end, y_end;

   void storeOriginalImage() {
       if (originalImage == nullptr) {
           originalWidth = getWidth();
           originalHeight = getHeight();
           originalImage = new unsigned char[originalWidth * originalHeight * 3];
           std::copy(getImage(), getImage() + (originalWidth * originalHeight * 3), originalImage);
       }
   }

   unsigned char* getOriginalImage() const {
       return originalImage;
   }

   int getOriginalWidth() const {
       return originalWidth;
   }

   int getOriginalHeight() const {
       return originalHeight;
   }

   Bmp(const Bmp& other) {
      width = other.width;
      height = other.height;
      x_start = other.x_start;
      y_start = other.y_start;
      x_end = other.x_end;
      y_end = other.y_end;
      bits = other.bits;
      bytesPerLine = other.bytesPerLine;

      
      data = new unsigned char[3 * other.width * other.height];
      memcpy(data, other.data, 3 * other.width * other.height);
   }

   ~Bmp() {
       delete[] originalImage;
   }
};

#endif