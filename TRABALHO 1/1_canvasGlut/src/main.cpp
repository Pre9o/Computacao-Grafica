#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "gl_canvas2d.h"
#include "Image.h"
#include "ImagesFunctions.h"
#include "Botao.h"
#include "Sidebar.h"
#include "ImageManager.h"

#include <chrono>

// ...

std::chrono::steady_clock::time_point inicio;

#define MAX_IMAGES 3

#define RECT_SIZE 10
#define TEXT_COORD 2

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1520, screenHeight = 1080;

int opcao  = 50; //variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().
int clicando = 0;

Sidebar sidebar;
ImageManager imageManager;


void render(){
   CV::translate(500, 500);
   for (Bmp* image : imageManager.images) {
      DrawImage(image);
   }

   if (imageManager.fourthImage != NULL) {
      // Renderiza a quarta imagem em uma posição fixa
      Draw4thImage(imageManager.fourthImage);
   }

   for (Botao* botao : sidebar.botoes) {
      botao->Render();
   }

   if(imageManager.draggingImage != NULL){
      ManipularVetorImagem(imageManager.images, imageManager.draggingImage);
   }

   if(imageManager.selectedImage != NULL){
      DesenharMoldura(imageManager.selectedImage);
   }

   if(imageManager.histograma){
      switch(imageManager.OpcaoHistograma){
         case 1:
            DesenharHistogramaRed(imageManager.selectedImage);
            break;
         case 2:
            DesenharHistogramaGreen(imageManager.selectedImage);
            break;
         case 3:
            DesenharHistogramaBlue(imageManager.selectedImage);
            break;
         case 4:
            DesenharHistogramaGray(imageManager.selectedImage);
            break;
      }
   }

   auto agora = std::chrono::steady_clock::now();
   auto duracao = std::chrono::duration_cast<std::chrono::seconds>(agora - inicio);

   int horas = duracao.count() / 3600;
   int minutos = (duracao.count() % 3600) / 60;
   int segundos = duracao.count() % 60;

   desenharTempo(horas, minutos, segundos);


   //Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}


//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   if( key < 200 )
   {
      opcao = key;
   }

   switch(key)
   {
      case 27:
         exit(0);
      break;
      case 200:
         //espaço
         rotateImage(imageManager.selectedImage, 90);
      break;
      case 202:
         //espaço
         rotateImage(imageManager.selectedImage, -90);
      break;
      case 'B':
         imageManager.selectedImage->adjustBrightness(5);
      break;
      case 'b':
         imageManager.selectedImage->adjustBrightness(-5);
      break;
      case 'C':
         imageManager.selectedImage->adjustContrast(5);
      break;
      case 'c':
         imageManager.selectedImage->adjustContrast(-5);
      break;
   }
}
//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   x -= 500;
   y -= 500;

   mouseX = x;
   mouseY = y;

   if(clicando){
      ArrastarImagem(imageManager.draggingImage, x, y);
   }

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);

   if (button == 0) {
      for (Bmp* image : imageManager.images) {
            if (image->contains(x, y && imageManager.draggingImage == nullptr)) {
               imageManager.selectedImage = image;
            }
            else if(!image->contains(x, y) && imageManager.draggingImage == nullptr){
               //selectedImage = nullptr;
            }
      }
      if(state == 1) {
      // Parar o arrasto
      imageManager.draggingImage = nullptr;
      clicando = 0;

      }
      else if (state == 0) {
         // Iniciar o arrasto
         for (Bmp* image : imageManager.images) {
            if (image->contains(x, y)) {
               imageManager.selectedImage = image;
               imageManager.draggingImage = image;
               clicando = 1;
            }
         }

         for (Botao* botao : sidebar.botoes) {
            if(botao->Colidiu(x, y)){
               botao->onClick();
            }
         }
      }
   }
}


int main()
{
   CV::init(screenHeight, screenWidth, "Rafael Carneiro Pregardier");

   inicio = std::chrono::steady_clock::now();

   sidebar.ConstruirBotoes(imageManager);

   CV::run();
   
   return 0;
}