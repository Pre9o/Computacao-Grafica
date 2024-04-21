/*
Este arquivo contém várias funções para manipular e exibir imagens usando a biblioteca Bmp. 

Funções:
- ManipularVetorImagem: Encontra uma imagem específica no vetor de imagens e a move para o final do vetor.
- ArrastarImagem: Define a posição inicial e final de uma imagem.
- DesenharMoldura: Desenha uma moldura ao redor de uma imagem.
- DrawImage: Desenha uma imagem pixel por pixel.
- InicializarParametros: Define a posição inicial e final de uma imagem.
- LoadImages: Carrega uma imagem de um caminho especificado, inicializa seus parâmetros, converte a imagem de BGR para RGB e a adiciona ao vetor de imagens.
- Draw4thImage: Desenha a quarta imagem do vetor de imagens.
- DesenharHistogramaRed: Desenha o histograma do canal vermelho de uma imagem.
- DesenharHistogramaGreen: Desenha o histograma do canal verde de uma imagem.
- DesenharHistogramaBlue: Desenha o histograma do canal azul de uma imagem.
- DesenharHistogramaGray: Desenha o histograma do canal cinza de uma imagem.
- rotateImage: Rotaciona uma imagem por um ângulo especificado.
- desenharDigito: Desenha um dígito em uma posição especificada.
- desenharTempo: Desenha o tempo atual na tela.
*/

#include <vector>
#include <algorithm>
#include "gl_canvas2d.h"
#include "Image.h"
#include <numeric>

#define M_PI 3.14159265358979323846

// Função para manipular o vetor de imagens
void ManipularVetorImagem(std::vector<Bmp*>& images, Bmp* &image){
   // Encontra a imagem no vetor
   std::vector<Bmp*>::iterator it = std::find(images.begin(), images.end(), image);
   // Se a imagem for encontrada, troca a posição com a última imagem do vetor
   if(it != images.end()){
      std::iter_swap(it, images.end() - 1);
   }
}

// Função para arrastar a imagem
void ArrastarImagem(Bmp* image, int x, int y) {
   // Define a posição inicial e final da imagem
   image->x_start = x - image->getWidth()/2;
   image->y_start = y - image->getHeight()/2;
   image->x_end = x + image->getWidth()/2;
   image->y_end = y + image->getHeight()/2;
}

// Função para desenhar a moldura da imagem
void DesenharMoldura(Bmp* image) {
   // Define a cor da moldura
   CV::color(0, 0, 0);
   // Desenha a moldura
   CV::rect(image->x_start, image->y_start, image->x_end, image->y_end);
}

// Função para desenhar a imagem
void DrawImage(Bmp* image){
   // Percorre a altura e a largura da imagem
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         // Define a posição do pixel
         int pos = i * image->getWidth() * 3 + j * 3;
         // Define a cor do pixel
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos+1]/255.0, image->getImage()[pos+2]/255.0);
         // Desenha o pixel
         CV::rectFill(j + image->x_start, i + image->y_start, j + image->x_start + 1, i + image->y_start + 1);
      }
   }
}

// Função para inicializar os parâmetros da imagem
void InicializarParametros(Bmp* image, int offset){
   // Define a posição inicial e final da imagem
   image->x_start = offset;
   image->y_start = 0;
   image->x_end = offset + image->getWidth();
   image->y_end = image->getHeight();
}

// Função para carregar as imagens
void LoadImages(std::vector<Bmp*> &images, const char* path){
   // Carrega a imagem
   Bmp* image_loaded = new Bmp(path);
   // Inicializa os parâmetros da imagem
   InicializarParametros(image_loaded, images.size() * 100);
   // Converte a imagem de BGR para RGB
   image_loaded->convertBGRtoRGB();
   // Adiciona a imagem ao vetor de imagens
   images.push_back(image_loaded);
}

// Função para desenhar a quarta imagem
void Draw4thImage(Bmp* image){
   // Percorre a altura e a largura da imagem
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         // Define a posição do pixel
         int pos = i * image->getWidth() * 3 + j * 3;
         // Define a cor do pixel
         CV::color(image->getImage()[pos]/255.0, image->getImage()[pos+1]/255.0, image->getImage()[pos+2]/255.0);
         // Desenha o pixel
         CV::rectFill(j + 1100, i + 200, j + 1101, i + 201);
      }
   }
}

// Função para desenhar o histograma do canal vermelho
void DesenharHistogramaRed(Bmp* image){
   // Inicializa o histograma
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   // Calcula o histograma
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos]]++;
      }
   }

   // Encontra o valor máximo do histograma
   int max = *std::max_element(histogram, histogram + 256);

   // Desenha o histograma
   for(int i = 1; i < 255; i++){
      CV::color(1, 0, 0);
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
   }
}

// Função para desenhar o histograma do canal verde
void DesenharHistogramaGreen(Bmp* image){
   // Inicializa o histograma
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   // Calcula o histograma
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos + 1]]++;
      }
   }

   // Encontra o valor máximo do histograma
   int max = *std::max_element(histogram, histogram + 256);

   // Desenha o histograma
   for(int i = 1; i < 255; i++){
      CV::color(0, 1, 0);
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
   }
}

// Função para desenhar o histograma do canal azul
void DesenharHistogramaBlue(Bmp* image){
   // Inicializa o histograma
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   // Calcula o histograma
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         histogram[image->getImage()[pos + 2]]++;
      }
   }

   // Encontra o valor máximo do histograma
   int max = *std::max_element(histogram, histogram + 256);

   // Desenha o histograma
   for(int i = 1; i < 255; i++){
      CV::color(0, 0, 1);
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
   }
}

// Função para desenhar o histograma do canal cinza
void DesenharHistogramaGray(Bmp* image){
   // Inicializa o histograma
   int histogram[256];
   for(int i = 0; i < 256; i++){
      histogram[i] = 0;
   }

   // Calcula o histograma
   for(int i = 0; i < image->getHeight(); i++)
   {
      for(int j = 0; j < image->getWidth(); j++)
      {
         int pos = i * image->getWidth() * 3 + j * 3;
         int gray = (image->getImage()[pos] + image->getImage()[pos + 1] + image->getImage()[pos + 2]) / 3;
         histogram[gray]++;
      }
   }

   // Encontra o valor máximo do histograma
   int max = *std::max_element(histogram, histogram + 256);

   // Desenha o histograma
   for(int i = 1; i < 255; i++){
      CV::color(0.5, 0.5, 0.5);
      float barHeight = (std::log(1 + histogram[i]) / std::log(1 + max) * 300);
      CV::rectFill(i + 1100, -100, i + 1101, -100 + barHeight);
   }
}

// Função para rotacionar a imagem
void rotateImage(Bmp* image, double angle) {
    // Calcula a nova largura e altura da imagem após a rotação
    int width = image->getWidth();
    int height = image->getHeight();

    double rad = angle * M_PI / 180.0;
    double cosAngle = cos(rad);
    double sinAngle = sin(rad);

    int newWidth = abs(width * cosAngle) + abs(height * sinAngle);
    int newHeight = abs(width * sinAngle) + abs(height * cosAngle);

    // Cria uma nova imagem com a nova largura e altura
    unsigned char* newImage = new unsigned char[newWidth * newHeight * 3];

    int centerX = width / 2;
    int centerY = height / 2;

    // Rotaciona a imagem
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

    // Recorta a imagem rotacionada para remover os espaços em branco
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

    // Cria uma nova imagem com a largura e altura recortadas
    unsigned char* croppedImage = new unsigned char[croppedWidth * croppedHeight * 3];

    for (int y = 0; y < croppedHeight; y++) {
        for (int x = 0; x < croppedWidth; x++) {
            for (int c = 0; c < 3; c++) {
                croppedImage[(y * croppedWidth + x) * 3 + c] = newImage[((y + minY) * newWidth + (x + minX)) * 3 + c];
            }
        }
    }

    // Libera a memória da imagem antiga e define a nova imagem
    delete[] newImage;
    image->setImage(croppedImage);
    image->setWidth(croppedWidth);
    image->setHeight(croppedHeight);
}

// Estrutura para representar um segmento
typedef struct {
    int posX, posY, posX2, posY2;
} Segmento;

// Array de segmentos
Segmento segmentos[7] = {
   {0, -350, 50, -355},  
   {50, -355, 55, -410},
   {50, -410, 55, -465}, 
   {0, -465, 50, -470},
   {-5, -410, 0, -465},  
   {-5, -355, 0, -410}, 
   {0, -410, 50, -415} 
};

// Array de dígitos
bool digitos[10][7] = {
   {true, true, true, true, true, true, false},  
   {false, true, true, false, false, false, false},  
   {true, true, false, true, true, false, true},  
   {true, true, true, true, false, false, true},  
   {false, true, true, false, false, true, true},  
   {true, false, true, true, false, true, true},  
   {true, false, true, true, true, true, true},  
   {true, true, true, false, false, false, false}, 
   {true, true, true, true, true, true, true},  
   {true, true, true, true, false, true, true}  
};

// Função para desenhar um dígito
void desenharDigito(int digito, int posX, int posY) {
   for (int i = 0; i < 7; i++) {
      if (digitos[digito][i]) {
         CV::color(0, 0, 0);
         CV::rectFill(segmentos[i].posX + posX, segmentos[i].posY + posY, segmentos[i].posX2 + posX, segmentos[i].posY2 + posY);
      }
   }
}

// Função para desenhar o tempo
void desenharTempo(int horas, int minutos, int segundos) {
   desenharDigito(horas / 10, 0, 0);
   desenharDigito(horas % 10, 80, 0); 
   desenharDigito(minutos / 10, 160, 0); 
   desenharDigito(minutos % 10, 240, 0); 
   desenharDigito(segundos / 10, 320, 0); 
   desenharDigito(segundos % 10, 400, 0); 
}