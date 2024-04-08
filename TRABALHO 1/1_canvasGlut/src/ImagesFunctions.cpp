#include <vector>
#include <algorithm>
#include "gl_canvas2d.h"
#include "Image.h"
#include <numeric>

void ManipularVetorImagem(std::vector<Bmp*>& images, Bmp* &image){
   // mover a image para a última posição do vetor
   std::vector<Bmp*>::iterator it = std::find(images.begin(), images.end(), image);
   if(it != images.end()){
      std::iter_swap(it, images.end() - 1);
   }
}

void ArrastarImagem(Bmp* image, int x, int y) {
   image->x_start = x - image->getWidth()/2;
   image->y_start = y - image->getHeight()/2;
   image->x_end = x + image->getWidth()/2;
   image->y_end = y + image->getHeight()/2;
}

void DesenharMoldura(Bmp* image) {
   CV::color(0, 0, 0);
   CV::rect(image->x_start, image->y_start, image->x_end, image->y_end);
}

void DrawImage(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos+1]/255.0, image->getImage()[pos+2]/255.0);
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

void InicializarParametros(Bmp* image, int offset){
   image->x_start = offset;
   image->y_start = 0;
   image->x_end = offset + image->getWidth();
   image->y_end = image->getHeight();
}


void LoadImages(std::vector<Bmp*> &images, const char* path){
   Bmp* image_loaded = new Bmp(path);
   InicializarParametros(image_loaded, images.size() * 100);
   image_loaded->convertBGRtoRGB();
   images.push_back(image_loaded);
}

void Draw4thImage(Bmp* image){
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos+1]/255.0, image->getImage()[pos+2]/255.0);
         CV::rectFill(j + 1100, i + 200, j + 1101, i + 201);
      }
   }
}

void DesenharHistogramaRed(Bmp* image){
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos]]++;
      }
   }

   int max = *std::max_element(histogram, histogram + 256);
   int min = *std::min_element(histogram, histogram + 256);

   // Desenhar o fundo
   CV::color(1, 1, 1);
   CV::rectFill(1100, 100, 1356, 400);

   for(int i = 0; i < 256; i++){
   CV::color(1, 0, 0); 
   float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
   CV::rectFill(i + 1100, 400 - barHeight, i + 1101, 400);
   }
}

void DesenharHistogramaGreen(Bmp* image){
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos + 1]]++;
      }
   }

   int max = *std::max_element(histogram, histogram + 256);
   int min = *std::min_element(histogram, histogram + 256);

   // Desenhar o fundo
   CV::color(1, 1, 1);
   CV::rectFill(1100, 100, 1356, 400);

   for(int i = 0; i < 256; i++){
      CV::color(0, 1, 0); 
      float height = 300 - (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, height, i + 1101, 400);
   }
}

void DesenharHistogramaBlue(Bmp* image){
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos + 2]]++;
      }
   }

   int max = *std::max_element(histogram, histogram + 256);
   int min = *std::min_element(histogram, histogram + 256);
   

   for(int i = 1; i < 255; i++){
      CV::color(0, 0, 1); 
      float height = 300 - (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, height, i + 1101, 400);
   }
}

void DesenharHistogramaGray(Bmp* image){
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         int gray = (image->getImage()[pos] + image->getImage()[pos + 1] + image->getImage()[pos + 2]) / 3;
         histogram[gray]++;
      }
   }

   int max = *std::max_element(histogram, histogram + 256);
   int min = *std::min_element(histogram, histogram + 256);


   for(int i = 0; i < 256; i++){
      CV::color(1, 0, 1); 
      float height = 300 - (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, height, i + 1101, 400);
   }
}

void rotateImage(Bmp* image, double angle) {
    int width = image->getWidth();
    int height = image->getHeight();

    double rad = angle * M_PI / 180.0;
    double cosAngle = cos(rad);
    double sinAngle = sin(rad);

    int newWidth = abs(width * cosAngle) + abs(height * sinAngle);
    int newHeight = abs(width * sinAngle) + abs(height * cosAngle);

    unsigned char* newImage = new unsigned char[newWidth * newHeight * 3];

    int centerX = width / 2;
    int centerY = height / 2;
    int newCenterX = newWidth / 2;
    int newCenterY = newHeight / 2;

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int oldX = cosAngle * (x - newCenterX) - sinAngle * (y - newCenterY) + centerX;
            int oldY = sinAngle * (x - newCenterX) + cosAngle * (y - newCenterY) + centerY;

            if (oldX >= 0 && oldX < width && oldY >= 0 && oldY < height) {
                for (int c = 0; c < 3; c++) {
                    newImage[(y * newWidth + x) * 3 + c] = image->getImage()[(oldY * width + oldX) * 3 + c];
                }
            } else {
                for (int c = 0; c < 3; c++) {
                    newImage[(y * newWidth + x) * 3 + c] = 0;
                }
            }
        }
    }

    delete[] image->getImage();
    image->setImage(newImage, newWidth, newHeight);
}

void drawSegment(int x, int y, int segment, int size, bool on) {
    int thickness = size / 10;
    int length = size * 4;
    int height = size * 5;

    switch (segment) {
        case 0: // Top
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x + length, y + height, x + length, y + height / 2);
            break;
        case 1: // Top right
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x + length, y + height, x + length, y + height / 2);
            break;
        case 2: // Bottom right
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x + length, y + height / 2, x + length, y);
            break;
        case 3: // Bottom
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x, y, x + length, y);
            break;
        case 4: // Bottom left
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x, y + height / 2, x, y);
            break;
        case 5: // Top left
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x, y + height, x, y + height / 2);
            break;
        case 6: // Middle
            if (on) CV::color(1, 0, 0);
            else CV::color(0.5, 0.5, 0.5);
            CV::line(x, y + height / 2, x + length, y + height / 2);
            break;
    }
}

void drawDigit(int x, int y, int digit, int size) {
    bool segments[7] = {false, false, false, false, false, false, false};

    switch (digit) {
        case 0: segments[0] = segments[1] = segments[2] = segments[3] = segments[4] = segments[5] = true; break;
        case 1: segments[1] = segments[2] = true; break;
        case 2: segments[0] = segments[1] = segments[6] = segments[4] = segments[3] = true; break;
        case 3: segments[0] = segments[1] = segments[6] = segments[2] = segments[3] = true; break;
        case 4: segments[5] = segments[6] = segments[1] = segments[2] = true; break;
        case 5: segments[0] = segments[5] = segments[6] = segments[2] = segments[3] = true; break;
        case 6: segments[0] = segments[5] = segments[4] = segments[3] = segments[2] = segments[6] = true; break;
        case 7: segments[0] = segments[1] = segments[2] = true; break;
        case 8: segments[0] = segments[1] = segments[2] = segments[3] = segments[4] = segments[5] = segments[6] = true; break;
        case 9: segments[0] = segments[1] = segments[2] = segments[5] = segments[6] = true; break;
    }

    for (int i = 0; i < 7; i++) {
        drawSegment(x, y, i, size, segments[i]);
    }
}