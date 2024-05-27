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

#include "windows.h"
#include "mmsystem.h"

// Início do tempo para calcular a duração da execução do programa
std::chrono::steady_clock::time_point inicio;
clock_t lastTime;
clock_t intervalo_tempo_inicio = clock();

#define MAX_IMAGES 3

// Dimensões da tela
int screenWidth = 1620, screenHeight = 900;

int opcaoMenu = 0;
int mouseX, mouseY;
int clicando = 0;

bool primeira_vez = true;

int menuOpcao = 0;

double tempo_inicio = 0;

bool setarUsername = false;

// Sidebar que contém os botões para manipular as imagens
Sidebar sidebar;
// Gerenciador de imagens que contém as imagens carregadas e a imagem selecionada
ImageManager imageManagerMenuInicial;
ImageManager imageManagerJogo;
ImageManager imageManagerMenuPausa;

Tabuleiro tabuleiro;

Canhao canhao;

Controle controle;

bool carregado = false;
bool firstMove = true;

double atraso = 0;

clock_t start = clock();

std::string username = "";

// Função para renderizar a tela
void render(){
   clock_t end = clock();
   float duration = (float)(end - start) / CLOCKS_PER_SEC;
   float framedeltaTime = 1.0f / 60.0f; // Para 60 FPS


   CV::clear(0.08235, 0.17647, 0.28627);

   switch (opcaoMenu)
   {
   case 0:
      for(Botao* botao : sidebar.botoesMenuInicial){
         botao->AtualizarPosicaoMeioTela(screenWidth / 2, screenHeight / 2);
         botao->Render();
      }

      for (Bmp* image : imageManagerMenuInicial.images) {
         AtualizarParametros(image, imageManagerMenuInicial.images.size() * 100, Vector2(screenWidth / 2, screenHeight / 2));
         DrawImage(image);
      }


      break;
   case 1:
      controle.tabuleiro.setExtremosTabuleiro(Vector2(screenWidth/2 - 300, screenHeight/2 + 400), Vector2(screenWidth/2 + 260, screenHeight/2 + 401), Vector2(screenWidth/2 + 261, screenHeight/2 - 400), Vector2(screenWidth/2 - 300, screenHeight/2 - 400));
      controle.tabuleiro.setTabuleiro();

      controle.tabuleiro.desenhaTabuleiro();

      controle.canhao.setCanhao(controle.tabuleiro);

      controle.setExtremosTabelaDePontos(Vector2(screenWidth/2 - 600, screenHeight/2 + 400), Vector2(screenWidth/2 - 400, screenHeight/2 + 400), Vector2(screenWidth/2 - 400, screenHeight/2 - 400), Vector2(screenWidth/2 - 600, screenHeight/2 - 400));

      for(auto& linha : controle.tabuleiro.matriz_tabuleiro){
         for(Bloco& bloco : linha){
            if(bloco.explosao != NULL) {
               bloco.explosao->desenha();
               if(bloco.explosao->tempoRestante == 0){
                  bloco.explosao = NULL;
               }
            }
            if(bloco.ativo == true){
               bloco.desenhaBloco();
            }
         }
      }
      for(auto& bola: controle.bolas){
         bola.desenhaBola();
      }

      controle.canhao.desenhaCanhao();

      controle.canhao.desenhaBocaCanhao();

      controle.desenharTabelaDePontos();

      controle.exibirNivelEPontuacao();

      if(controle.jogando){
         if (firstMove) {
            lastTime = clock();
            firstMove = false;
         }
         clock_t now = clock();
         double deltaTime = (double)(now - lastTime) / 1000.0f;

         if(deltaTime > 1.0/60.0f){
            controle.controlaJogo(0.01, &firstMove);
            lastTime = now;
         }
      }

      carregado = true;
      break;
   case 2:
      for(Botao* botao : sidebar.botoesMenuPausa){
         botao->AtualizarPosicaoMeioTela(screenWidth / 2, screenHeight / 2);
         botao->Render();
      }

      for (Bmp* image : imageManagerMenuInicial.images) {
         AtualizarParametros(image, imageManagerMenuInicial.images.size() * 100, Vector2(screenWidth / 2, screenHeight / 2));
         DrawImage(image);
      }
      break;
   case 3:
      if(setarUsername){
         CV::color(1, 1, 1);
         CV::text(screenWidth/2 - 100, screenHeight/2, username.c_str());
      }
      break;

   default:
      break;

   if (duration < framedeltaTime) {
        Sleep((framedeltaTime - duration) * 1000);
        start = clock();
    }
   }
}

// Função para lidar com a entrada do teclado
void keyboard(int key){
   if(setarUsername){
      if(isalnum(key)){
         username += key;
      }
      else if(key == 8 && username.size() > 0){
         username = username.substr(0, username.size() - 1);
      }
      else if(key == 13){
         setarUsername = false;
         opcaoMenu = 0;
         controle.setUsername(username);
      }
   }
   

   switch(key){
      case 27:
         // Se a tecla ESC for pressionada, sai do programa
         if(opcaoMenu == 0){
            exit(0);
         }
         else if(opcaoMenu == 1){
            opcaoMenu = 2;
         }
         else if(opcaoMenu == 2){
            opcaoMenu = 1;
         }
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

   controle.canhao.setMousePos(Vector2(x, y));

   if (button == 0) {
      if(state == 1) {
      }
      else if (state == 0) {
         for (Botao* botao : sidebar.botoesMenuInicial) {
            if(botao->Colidiu(x, y)){
               if(opcaoMenu == 0){
                  botao->onClick();
               }
            }
         }

         for (Botao* botao : sidebar.botoesMenuPausa) {
            if(botao->Colidiu(x, y)){
               if(opcaoMenu == 2){
                  botao->onClick();
               }
            }
         }

         if(opcaoMenu == 1 && carregado == true && controle.jogando == false){
            controle.jogando = true;
            canhao = controle.canhao;
            for(auto& bola: controle.bolas){
               bola.setBola(canhao, atraso);
               atraso += 0.1;
            }
            atraso = 0;
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
   sidebar.ConstruirBotoesMenuInicial(&opcaoMenu, &intervalo_tempo_inicio, &setarUsername);

   sidebar.ConstruirBotoesMenuPausa(&opcaoMenu, &intervalo_tempo_inicio);

   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 + 400));
   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 260, screenHeight/2 + 401));
   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 261, screenHeight/2 - 400));
   tabuleiro.extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 - 400));

   tabuleiro.setExtremosTabuleiro(Vector2(screenWidth/2 - 300, screenHeight/2 + 400), Vector2(screenWidth/2 + 260, screenHeight/2 + 401), Vector2(screenWidth/2 + 261, screenHeight/2 - 400), Vector2(screenWidth/2 - 300, screenHeight/2 - 400));
   tabuleiro.setTabuleiro();

   canhao.setCanhao(tabuleiro);

   controle.setTabuleiro(tabuleiro);

   controle.gerarNivel();

   controle.setCanhao(canhao);

   controle.extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 600, screenHeight/2 + 400));
   controle.extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 400, screenHeight/2 + 400));
   controle.extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 400, screenHeight/2 - 400));
   controle.extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 600, screenHeight/2 - 400));

   Vector2 posicao = Vector2(screenWidth/2, screenHeight/2);

   LoadImages(imageManagerMenuInicial.images, ".\\images\\Teste.bmp", posicao);

   srand(time(0));

   PlaySound(TEXT(".\\audios\\Embalo.wav"), NULL, SND_ASYNC);
   // Inicia o loop principal do programa
   CV::run();

   return 0;
}
