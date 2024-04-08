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

   for(int i = 1; i < 255; i++){
      CV::color(1, 0, 0); 
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
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

   for(int i = 1; i < 255; i++){
      CV::color(0, 1, 0); 
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
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

   for(int i = 1; i < 255; i++){
      CV::color(0, 0, 1); 
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
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

   for(int i = 1; i < 255; i++){
      CV::color(0.5, 0.5, 0.5); 
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
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

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int oldX = round(cosAngle * (x - newWidth / 2) + sinAngle * (y - newHeight / 2) + centerX);
            int oldY = round(-sinAngle * (x - newWidth / 2) + cosAngle * (y - newHeight / 2) + centerY);

            if (oldX >= 0 && oldX < width && oldY >= 0 && oldY < height) {
                newImage[(y * newWidth + x) * 3] = image->getImage()[(oldY * width + oldX) * 3];
                newImage[(y * newWidth + x) * 3 + 1] = image->getImage()[(oldY * width + oldX) * 3 + 1];
                newImage[(y * newWidth + x) * 3 + 2] = image->getImage()[(oldY * width + oldX) * 3 + 2];
            } else {
                newImage[(y * newWidth + x) * 3] = 0;
                newImage[(y * newWidth + x) * 3 + 1] = 0;
                newImage[(y * newWidth + x) * 3 + 2] = 0;
            }
        }
    }

    // Após a rotação, mas antes de definir a nova imagem...

   int minX = newWidth, minY = newHeight, maxX = 0, maxY = 0;

   for (int y = 0; y < newHeight; y++) {
      for (int x = 0; x < newWidth; x++) {
         if (newImage[(y * newWidth + x) * 3] != 0 ||
            newImage[(y * newWidth + x) * 3 + 1] != 0 ||
            newImage[(y * newWidth + x) * 3 + 2] != 0) {
            minX = std::min(minX, x);
            minY = std::min(minY, y);
            maxX = std::max(maxX, x);
            maxY = std::max(maxY, y);
         }
      }
   }

   int croppedWidth = maxX - minX + 1;
   int croppedHeight = maxY - minY + 1;

   unsigned char* croppedImage = new unsigned char[croppedWidth * croppedHeight * 3];

   for (int y = 0; y < croppedHeight; y++) {
      for (int x = 0; x < croppedWidth; x++) {
         for (int c = 0; c < 3; c++) {
            croppedImage[(y * croppedWidth + x) * 3 + c] = newImage[((y + minY) * newWidth + (x + minX)) * 3 + c];
         }
      }
   }

   delete[] newImage;
   image->setImage(croppedImage);
   image->setWidth(croppedWidth);
   image->setHeight(croppedHeight);
}

typedef struct {
    int posX, posY, posX2, posY2;
} Segmento;
    
// Defina as posições e tamanhos dos segmentos
Segmento segmentos[7] = {
   {0, -350, 50, -355},  // Segmento A
   {50, -355, 55, -410},  // Segmento B
   {50, -410, 55, -465},  // Segmento C
   {0, -465, 50, -470},  // Segmento D
   {-5, -410, 0, -465},  // Segmento E
   {-5, -355, 0, -410},  // Segmento F
   {0, -410, 50, -415} // Segmento G
};

// Defina quais segmentos devem ser acesos para cada dígito
bool digitos[10][7] = {
   {true, true, true, true, true, true, false},  // 0
   {false, true, true, false, false, false, false},  // 1
   {true, true, false, true, true, false, true},  // 2
   {true, true, true, true, false, false, true},  // 3
   {false, true, true, false, false, true, true},  // 4 corrigido
   {true, false, true, true, false, true, true},  // 5
   {true, false, true, true, true, true, true},  // 6
   {true, true, true, false, false, false, false},  // 7
   {true, true, true, true, true, true, true},  // 8
   {true, true, true, true, false, true, true}  // 9 corrigido
};

// Para desenhar um dígito, acenda os segmentos apropriados
void desenharDigito(int digito, int posX, int posY) {
   for (int i = 0; i < 7; i++) {
      if (digitos[digito][i]) {
         CV::color(0, 0, 0);
         CV::rectFill(segmentos[i].posX + posX, segmentos[i].posY + posY, segmentos[i].posX2 + posX, segmentos[i].posY2 + posY);
      }
   }
}


// Para desenhar o tempo, desenhe cada dígito em uma posição apropriada
void desenharTempo(int horas, int minutos, int segundos) {
   desenharDigito(horas / 10, 0, 0);
   desenharDigito(horas % 10, 80, 0); // alterado de 60 para 80
   desenharDigito(minutos / 10, 160, 0); // alterado de 120 para 160
   desenharDigito(minutos % 10, 240, 0); // alterado de 180 para 240
   desenharDigito(segundos / 10, 320, 0); // alterado de 240 para 320
   desenharDigito(segundos % 10, 400, 0); // alterado de 300 para 400
}