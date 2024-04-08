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

std::vector<Bmp*> images;
std::vector<Botao*> botoes;

Bmp* draggingImage = NULL;
Bmp* selectedImage = NULL;

Bmp* fourthImage = NULL;

bool histograma = false;
int OpcaoHistograma = NULL;

void ConstruirBotoes(){
   if(botoes.size() == 0){
      botoes.push_back(new Botao(1075, -475, 150, 50, "Flip Vertical", 1, 1, 0, [](){
         if (selectedImage != NULL) {
            selectedImage->flipVertical();
         }
      }));
      botoes.push_back(new Botao(1250, -475, 150, 50, "Flip Horizontal", 1, 1, 0, [](){
         if (selectedImage != NULL) {
            selectedImage->flipHorizontal();
         }
      }));
      botoes.push_back(new Botao(1250, -175, 150, 50, "Grayscale", 0.5, 0.5, 0.5, [](){
         if (selectedImage != NULL) {
            // Deleta a imagem anterior para evitar vazamento de memória
            if (fourthImage != NULL) {
               delete fourthImage;
            }
         // Cria uma cópia da imagem selecionada
         fourthImage = new Bmp(*selectedImage);
         fourthImage->image_Gray();
         }
      }));
      botoes.push_back(new Botao(1250, -250, 150, 50, "Red", 1, 0, 0, [](){
         if (selectedImage != NULL) {
            if(fourthImage != NULL){
               delete fourthImage;
            }
            fourthImage = new Bmp(*selectedImage);
            fourthImage->image_R();
         }
      }));
      botoes.push_back(new Botao(1250, -325, 150, 50, "Green", 0, 1, 0, [](){
         if (selectedImage != NULL) {
            if(fourthImage != NULL){
               delete fourthImage;
            }
            fourthImage = new Bmp(*selectedImage);
            fourthImage->image_G();
         }
      }));
      botoes.push_back(new Botao(1250, -400, 150, 50, "Blue", 0, 0, 1, [](){
         if (selectedImage != NULL) {
            if(fourthImage != NULL){
               delete fourthImage;
            }
            fourthImage = new Bmp(*selectedImage);
            fourthImage->image_B();
         }
      }));
      botoes.push_back(new Botao(900, -400, 150, 50, "Load Image 3", 0.75, 1, 1, [](){
         LoadImages(images, ".\\images\\pinguim.bmp");
      }));
      botoes.push_back(new Botao(900, -325, 150, 50, "Load Image 2", 1, 0.75, 1, [](){
         LoadImages(images, ".\\images\\teste.bmp");
      }));
      botoes.push_back(new Botao(900, -250, 150, 50, "Load Image 1", 1, 1, 0.75, [](){
         LoadImages(images, ".\\images\\snail.bmp");
      }));
      botoes.push_back(new Botao(1075, -250, 150, 50, "Histograma Red", 1, 0, 0, [](){
         if (selectedImage != NULL) {
            !histograma ? histograma = true : histograma = false;
            OpcaoHistograma = 1;
         }
      }));
      botoes.push_back(new Botao(1075, -325, 150, 50, "Histograma Green", 0, 1, 0, [](){
         if (selectedImage != NULL) {
            !histograma ? histograma = true : histograma = false;
            OpcaoHistograma = 2;
         }
      }));
      botoes.push_back(new Botao(1075, -400, 150, 50, "Histograma Blue", 0, 0, 1, [](){
         if (selectedImage != NULL) {
            !histograma ? histograma = true : histograma = false;
            OpcaoHistograma = 3;
         }
      }));
      botoes.push_back(new Botao(1075, -175, 150, 50, "Histograma Gray", 0.5, 0.5, 0.5, [](){
         if (selectedImage != NULL) {
            !histograma ? histograma = true : histograma = false;
            OpcaoHistograma = 4;
         }
      }));
   }
}



void render(){
   CV::translate(500, 500);
   for (Bmp* image : images) {
      DrawImage(image);
   }

   if (fourthImage != NULL) {
      // Renderiza a quarta imagem em uma posição fixa
      Draw4thImage(fourthImage);
   }

   for (Botao* botao : botoes) {
      botao->Render();
   }

   if(draggingImage != NULL){
      ManipularVetorImagem(images, draggingImage);
   }

   if(selectedImage != NULL){
      DesenharMoldura(selectedImage);
   }

   if(histograma){
      switch(OpcaoHistograma){
         case 1:
            DesenharHistogramaRed(selectedImage);
            break;
         case 2:
            DesenharHistogramaGreen(selectedImage);
            break;
         case 3:
            DesenharHistogramaBlue(selectedImage);
            break;
         case 4:
            DesenharHistogramaGray(selectedImage);
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
         rotateImage(selectedImage, 90);
      break;
      case 202:
         //espaço
         rotateImage(selectedImage, -90);
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
      ArrastarImagem(draggingImage, x, y);
   }

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction, x, y);

   if (button == 0) {
      for (Bmp* image : images) {
            if (image->contains(x, y && draggingImage == nullptr)) {
               selectedImage = image;
            }
            else if(!image->contains(x, y) && draggingImage == nullptr){
               //selectedImage = nullptr;
            }
      }
      if(state == 1) {
      // Parar o arrasto
      draggingImage = nullptr;
      clicando = 0;

      }
      else if (state == 0) {
         // Iniciar o arrasto
         for (Bmp* image : images) {
            if (image->contains(x, y)) {
               selectedImage = image;
               draggingImage = image;
               clicando = 1;
            }
         }

         for (Botao* botao : botoes) {
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

   ConstruirBotoes();
   CV::run();
   
   return 0;
}