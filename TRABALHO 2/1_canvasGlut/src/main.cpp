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
#include "Game.h"

#include <chrono>
#include <thread>

// Início do tempo para calcular a duração da execução do programa
std::chrono::steady_clock::time_point inicio;

clock_t lastTime = clock();

clock_t intervalo_tempo_inicio = clock();

#define MAX_IMAGES 3

// Dimensões da tela
int screenWidth = 1620, screenHeight = 900;

int opcaoMenu = 0;
int mouseX, mouseY;
int clicando = 0;

bool primeira_vez = true;

int menuOpcao = 0;

// Sidebar que contém os botões para manipular as imagens
Sidebar sidebar;
// Gerenciador de imagens que contém as imagens carregadas e a imagem selecionada
ImageManager imageManager;

Tabuleiro tabuleiro;

Canhao canhao;

Controle controle;


// Função para renderizar a tela
void render(){
clock_t start = clock();

   CV::clear(0, 0, 0);

   switch (opcaoMenu)
   {
   case 0:
      for(Botao* botao : sidebar.botoes){
         botao->AtualizarPosicaoMeioTela(screenWidth / 2, screenHeight / 2);
         botao->Render();
      }
      break;
   case 1:
      tabuleiro.setExtremosTabuleiro(Vector2(screenWidth/2 - 300, screenHeight/2 + 400), Vector2(screenWidth/2 + 260, screenHeight/2 + 401), Vector2(screenWidth/2 + 261, screenHeight/2 - 400), Vector2(screenWidth/2 - 300, screenHeight/2 - 400));
      tabuleiro.setTabuleiro();

      tabuleiro.desenhaTabuleiro();

      canhao.setCanhao(tabuleiro);
      controle.setTabuleiro(tabuleiro);
      controle.setCanhao(canhao);

      for(auto& linha : tabuleiro.matriz_tabuleiro){
         for(Bloco& bloco : linha){
            if(bloco.ativo == true){
               bloco.desenhaBloco();
            }
         }
      }
      canhao.desenhaCanhao();

      for(auto& bola: controle.bolas){
         bola.desenhaBola();
      }

      if(controle.jogando){
         clock_t now = clock();
         double deltaTime = (double)(now - lastTime) / 1000.0f;

         double tempo_inicio = (double)(now - intervalo_tempo_inicio) / CLOCKS_PER_SEC;

         if(deltaTime > 1.0/60.0f && tempo_inicio > 2.0){
            //printf("DELTA PORRA TIME: %f\n", deltaTime);
            controle.executaJogada(deltaTime);
            lastTime = now;
         }
      }
      break;

   default:
      break;
   }


    clock_t end = clock();
    float duration = (float)(end - start) / CLOCKS_PER_SEC;
    float framedeltaTime = 1.0f / 60.0f; // Para 60 FPS

    if (duration < framedeltaTime) {
        Sleep((framedeltaTime - duration) * 1000);
    }

}

// Função para lidar com a entrada do teclado
void keyboard(int key){
   switch(key){
      case 27:
         // Se a tecla ESC for pressionada, sai do programa
         exit(0);
      break;
   }
}

// Função para lidar com a liberação da tecla
void keyboardUp(int /*key*/){
}


// Função para lidar com a entrada do mouse
void mouse(int button, int state, int /*wheel*/, int /*direction*/, int x, int y)
{
   mouseX = x;
   mouseY = y;

   canhao.setMousePos(Vector2(x, y));

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
               if(opcaoMenu == 0){
                  botao->onClick();
               }
            }
         }
         if(opcaoMenu == 1){
            controle.jogando = true;
            for(auto& bola: controle.bolas){
               bola.setBola(canhao);
            }
         }
      }
   }
}

// Função principal
int main(){
   // Inicializa a tela com as dimensões especificadas e o título da janela
   CV::init(screenWidth, screenHeight, "Rafael Carneiro Pregardier");

   // Marca o início do tempo
   inicio = std::chrono::steady_clock::now();

   // Constrói os botões na barra lateral
   sidebar.ConstruirBotoesMenuInicial(&opcaoMenu, &intervalo_tempo_inicio);

   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 + 400));
   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 260, screenHeight/2 + 401));
   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 261, screenHeight/2 - 400));
   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 - 400));

   tabuleiro.setExtremosTabuleiro(Vector2(screenWidth/2 - 300, screenHeight/2 + 400), Vector2(screenWidth/2 + 260, screenHeight/2 + 401), Vector2(screenWidth/2 + 261, screenHeight/2 - 400), Vector2(screenWidth/2 - 300, screenHeight/2 - 400));
   tabuleiro.setTabuleiro();

   //printf("Extremos: X:%f Y:%f X:%f Y:%f\n", tabuleiro.extremos_tabuleiro[0].x, tabuleiro.extremos_tabuleiro[0].y, tabuleiro.extremos_tabuleiro[2].x, tabuleiro.extremos_tabuleiro[2].y);

   tabuleiro.definirBlocos();

   canhao.setCanhao(tabuleiro); 
   printf("Canhao: X:%f Y:%f\n", canhao.origem.x, canhao.origem.y);

   controle.setTabuleiro(tabuleiro);

   controle.setCanhao(canhao);

   controle.adicionarBolas(1);

   srand(time(NULL));

   // Inicia o loop principal do programa
   CV::run();

   return 0;
}