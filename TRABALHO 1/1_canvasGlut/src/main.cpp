/*
Este programa é um editor de imagens simples que permite ao usuário carregar imagens, selecionar uma imagem clicando nela, arrastar a imagem selecionada e aplicar várias operações à imagem selecionada, como rotacionar, ajustar o brilho e o contraste, e visualizar o histograma da imagem. O programa também exibe o tempo decorrido desde o início do programa.

Classes:
- Bmp: Representa uma imagem BMP.
- Botao: Representa um botão na interface do usuário.
- Sidebar: Representa a barra lateral que contém os botões.
- ImageManager: Gerencia as imagens carregadas e a imagem selecionada.

Métodos:
- render(): Renderiza a tela.
- keyboard(int key): Lida com a entrada do teclado.
- keyboardUp(int key): Lida com a liberação da tecla.
- mouse(int button, int state, int wheel, int direction, int x, int y): Lida com a entrada do mouse.
- main(): Função principal do programa.

Feito por Rafael Carneiro Pregardier.
*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

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

// Início do tempo para calcular a duração da execução do programa
std::chrono::steady_clock::time_point inicio;

#define MAX_IMAGES 3

#define RECT_SIZE 10
#define TEXT_COORD 2

// Dimensões da tela
int screenWidth = 1520, screenHeight = 1080;

int opcao  = 50;
int mouseX, mouseY;
int clicando = 0;

int grausRotacao = 0;

// Sidebar que contém os botões para manipular as imagens
Sidebar sidebar;
// Gerenciador de imagens que contém as imagens carregadas e a imagem selecionada
ImageManager imageManager;

// Função para renderizar a tela
void render(){
   // Translada o sistema de coordenadas para o centro da tela
   CV::translate(500, 500);
   // Desenha todas as imagens carregadas
   for (Bmp* image : imageManager.images) {
      DrawImage(image);
   }

   // Se a quarta imagem existir (imagem resultante de uma operação), desenha ela
   if (imageManager.fourthImage != NULL) {
      Draw4thImage(imageManager.fourthImage);
   }

   // Renderiza todos os botões na barra lateral
   for (Botao* botao : sidebar.botoes) {
      botao->Render();
   }

   // Se uma imagem está sendo arrastada, atualiza sua posição
   if(imageManager.draggingImage != NULL){
      ManipularVetorImagem(imageManager.images, imageManager.draggingImage);
   }

   // Se uma imagem está selecionada, desenha uma moldura ao redor dela
   if(imageManager.selectedImage != NULL){
      DesenharMoldura(imageManager.selectedImage);
   }

   // Se o histograma está ativado, desenha o histograma da imagem selecionada
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

   // Calcula o tempo decorrido desde o início do programa
   auto agora = std::chrono::steady_clock::now();
   auto duracao = std::chrono::duration_cast<std::chrono::seconds>(agora - inicio);

   int horas = duracao.count() / 3600;
   int minutos = (duracao.count() % 3600) / 60;
   int segundos = duracao.count() % 60;

   // Desenha o tempo decorrido na tela
   desenharTempo(horas, minutos, segundos);

}

// Função para lidar com a entrada do teclado
void keyboard(int key){
   if( key < 200 ){
      opcao = key;
   }

   switch(key){
      case 27:
         // Se a tecla ESC for pressionada, sai do programa
         exit(0);
      break;
      case 200:
         // Se a tecla 200 for pressionada, rotaciona a imagem selecionada 90 graus
         grausRotacao += 1;
         rotateImage(imageManager.selectedImage, grausRotacao);
      break;
      case 202:
         // Se a tecla 202 for pressionada, rotaciona a imagem selecionada -90 graus
         grausRotacao -= 1;
         rotateImage(imageManager.selectedImage, -grausRotacao);
      break;
      case 'B':
         // Se a tecla 'B' for pressionada, aumenta o brilho da imagem selecionada
         imageManager.selectedImage->adjustBrightness(5);
      break;
      case 'b':
         // Se a tecla 'b' for pressionada, diminui o brilho da imagem selecionada
         imageManager.selectedImage->adjustBrightness(-5);
      break;
      case 'C':
         // Se a tecla 'C' for pressionada, aumenta o contraste da imagem selecionada
         imageManager.selectedImage->adjustContrast(5);
      break;
      case 'c':
         // Se a tecla 'c' for pressionada, diminui o contraste da imagem selecionada
         imageManager.selectedImage->adjustContrast(-5);
      break;
   }
}

// Função para lidar com a liberação da tecla
void keyboardUp(int /*key*/){
}

// Função para lidar com a entrada do mouse
void mouse(int button, int state, int /*wheel*/, int /*direction*/, int x, int y)
{
   x -= 500;
   y -= 500;

   mouseX = x;
   mouseY = y;

   // Se o botão esquerdo do mouse está sendo pressionado, arrasta a imagem selecionada
   if(clicando){
      ArrastarImagem(imageManager.draggingImage, x, y);
   }

   if (button == 0) {
      // Se o botão esquerdo do mouse foi pressionado, verifica se uma imagem foi clicada
      for (Bmp* image : imageManager.images) {
            if (image->contains(x, y && imageManager.draggingImage == nullptr)) {
               imageManager.selectedImage = image;
            }
            else if(!image->contains(x, y) && imageManager.draggingImage == nullptr){
            }
      }
      if(state == 1) {
      // Se o botão esquerdo do mouse foi solto, para de arrastar a imagem
      imageManager.draggingImage = nullptr;
      clicando = 0;

      }
      else if (state == 0) {
         // Se o botão esquerdo do mouse foi pressionado, verifica se uma imagem ou botão foi clicado
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

// Função principal
int main(){
   // Inicializa a tela com as dimensões especificadas e o título da janela
   CV::init(screenHeight, screenWidth, "Rafael Carneiro Pregardier");

   // Marca o início do tempo
   inicio = std::chrono::steady_clock::now();

   // Constrói os botões na barra lateral
   sidebar.ConstruirBotoes(imageManager);

   // Inicia o loop principal do programa
   CV::run();

   return 0;
}