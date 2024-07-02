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

#include <chrono>
#include <thread>

#include "windows.h"
#include "mmsystem.h"

#define M_PI 3.14159265358979323846

int screenWidth = 1620, screenHeight = 900;

int mouseX, mouseY;

clock_t start = clock();

// Parâmetros iniciais

Vector2 origemVirabrequim(0, 0);
Vector2 origemVirabrequim2(0, 0);

float alturaCilindro = -110.0f;
float comprimentoCilindro = 70.0f;

float comprimentoPistao = 70.0f;

float anguloRotacao = 0.0f;
float constante = 0.001f;

void drawVirabrequim(Vector2 origem){
   CV::color(0, 1, 0);
   CV::circle(origem.x, origem.y, 15, 50);
}

void drawSegundoCirculo(Vector2 origem, float angulo) {
   float raio = 40.0f; // Define o raio da linha entre os círculos
    // Limites de ângulo em radianos
    

    float x = origem.x + cos(angulo) * raio; // Calcula a nova posição x do segundo círculo
    float y = origem.y + sin(angulo) * raio; // Calcula a nova posição y do segundo círculo


    // Desenha a linha entre os círculos
    CV::color(1, 0, 0);
    CV::line(origem.x, origem.y, x, y);

    // Desenha o segundo círculo
    CV::circle(x, y, 15, 50); 

    origemVirabrequim2 = Vector2(x, y);
}

// Função para desenhar o cilindro
void drawCilindro(Vector2 origemVirabrequim2, float anguloRotacao) {
    // Posição fixa da extremidade superior do cilindro
    Vector2 extremidadeSuperior(origemVirabrequim.x, origemVirabrequim.y + comprimentoCilindro * 2);

    // Calcula a direção do cilindro apontando para o virabrequim
    Vector2 direcao = origemVirabrequim2 - extremidadeSuperior;
    direcao.normalize();
    
    // Calcula a posição da extremidade inferior do cilindro mantendo o comprimento constante
    Vector2 extremidadeInferior = extremidadeSuperior + direcao * comprimentoCilindro;
    
    // Desenha o cilindro (linha) entre as duas extremidades
    CV::color(1, 1, 0); // Cor amarela para o cilindro
    CV::line(extremidadeSuperior.x, extremidadeSuperior.y, extremidadeInferior.x, extremidadeInferior.y);

}

void drawPistao(Vector2 origem, float angulo) {

    // Calcula a posição da extremidade inferior do cilindro que acompanha o virabrequim
    float raio = 80.0f;
    
      float x = origem.x + cos(angulo) * raio; // Calcula a nova posição x do segundo círculo
      float y = origem.y + sin(angulo) * raio; // Calcula a nova posição y do segundo círculo

      // Desenha o pistão
      CV::color(0, 1, 1); // Cor ciano para o pistão
      CV::line(origem.x, origem.y, x, y);

}

// Função para renderizar a tela
void render(){
   clock_t end = clock();
   float duration = (float)(end - start) / CLOCKS_PER_SEC;
   float frameDeltaTime = 1.0f / 60.0f; // Para 60 FPS

   CV::clear(0.08235, 0.17647, 0.28627);

   CV::translate(500, 500);
   
   // Desenha o cilindro em 2D
   drawVirabrequim(origemVirabrequim);
   // Atualiza e desenha o segundo círculo com rotação
   anguloRotacao -= constante; // Ajusta este valor para controlar a velocidade de rotação
   drawSegundoCirculo(origemVirabrequim, anguloRotacao);
   drawCilindro(origemVirabrequim2, anguloRotacao);
   drawPistao(origemVirabrequim2, anguloRotacao);


   if (duration < frameDeltaTime) {
      Sleep((frameDeltaTime - duration) * 1000);
      start = clock();
   }
}

// Função para lidar com a entrada do teclado
void keyboard(int key){
   switch(key){
      case 27:
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

   if (button == 0) {
      if(state == 1) {
      }

      else if (state == 0) {
      }
   }
}

// Função principal
int main(){
   CV::init(screenWidth, screenHeight, "Rafael Carneiro Pregardier");

   CV::run();

   return 0;
}
