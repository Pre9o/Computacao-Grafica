/*
Este programa é um editor de imagens simples que permite ao usuário carregar imagens, selecionar uma imagem clicando nela, arrastar a imagem selecionada e aplicar várias operações à imagem selecionada, como rotacionar, ajustar o brilho e o contraste, e visualizar o histograma da imagem. O programa também exibe o tempo decorrido desde o início do programa.

Classes:
- Bmp: Representa uma imagem BMP.
- Botao: Representa um botão na interface do usuário.
- Sidebar: Representa a barra lateral que contém os botões.
- ImageManager: Gerencia as imagens carregadas e a imagem selecionada.
- Tabuleiro: Representa o tabuleiro do jogo.
- Canhao: Representa o canhão do jogo.
- Controle: Controla o jogo.

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
#include "Controle.h"
#include "Render.h"

#include <chrono>
#include <thread>

#include "windows.h"
#include "mmsystem.h"

#define MAX_IMAGES 3

clock_t lastTime;

int screenWidth = 1620, screenHeight = 900;

int opcaoMenu = 0;

int mouseX, mouseY;

bool setarUsername = false;

Sidebar sidebar;

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

std::string username = "Guest";

// Função para renderizar a tela
void render(){
   CV::clear(0.08235, 0.17647, 0.28627);

   switch (opcaoMenu){
      case 0:
         renderizarMenuInicial(sidebar, imageManagerMenuInicial, screenWidth, screenHeight);

         break;
      case 1:
         renderizarJogo(controle, firstMove, lastTime, carregado);

         break;
      case 2:
         renderizarMenuPausa(sidebar, imageManagerMenuPausa, imageManagerMenuInicial, screenWidth, screenHeight);

         break;
      case 3:
         if(setarUsername){
            controle.telaDeModificacaoDeUsername(username);
         }
         break;

   default:
      break;
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
      case 'r': 
            if (opcaoMenu == 1) {
                for (auto& bola : controle.bolas) {
                    bola.iniciarAnimacaoRetorno(Vector2(canhao.origem.x - 2, canhao.origem.y - 2));
                }
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
   CV::init(screenWidth, screenHeight, "Rafael Carneiro Pregardier");

   sidebar.ConstruirBotoesMenuInicial(&opcaoMenu, &setarUsername);
   sidebar.ConstruirBotoesMenuPausa(&opcaoMenu);

   canhao.setCanhao(tabuleiro);

   controle.setTabuleiro(tabuleiro);
   controle.gerarNivel();
   controle.setCanhao(canhao);
   controle.setUsername(username);

   Vector2 posicao = Vector2(screenWidth/2, screenHeight/2);
   LoadImages(imageManagerMenuInicial.images, "./Trab3RafaelCarneiroPregardier/images/Teste.bmp", posicao);

   srand(time(0));

   //PlaySound(TEXT("./Trab3RafaelCarneiroPregardier/audios/Embalo.wav"), NULL, SND_ASYNC);

   CV::run();

   return 0;
}
