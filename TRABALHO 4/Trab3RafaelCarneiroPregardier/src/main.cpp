/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2024
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
//  Instru��es:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <limits>

#include "gl_canvas2d.h"
#include "FuncoesGerais.h"

#define Modelos_COUNT 9

int screenWidth = 1000, screenHeight = 1000;

int mouseX, mouseY; 

Vector3 posicaoCamera = Vector3(0, 0, 0);
Vector3 rotacaoCamera = Vector3(0, 0, 0);

bool isMousePressed = false;
int lastMouseX, lastMouseY;

float velocidadeRotacao = 0.005;
float distancia = 400;

clock_t start = clock();

Pecas *pecas[9];
FuncoesGerais3D funcoesGerais;


void render(){
   clock_t end = clock();
   float duration = (float)(end - start) / CLOCKS_PER_SEC;
   float frameDeltaTime = 1.0f / 60.0f;

   CV::clear(0, 0, 0);
   CV::translate(screenHeight/2, screenWidth/2);

    funcoesGerais.executar3D(pecas, posicaoCamera, rotacaoCamera, distancia, velocidadeRotacao);

    if (duration < frameDeltaTime) {
      Sleep((frameDeltaTime - duration) * 1000);
      start = clock();
    }

}

void keyboard(int key){
   switch(key)
    {
        case 27:
            exit(0);
            break;

        // setas para mover a câmera
        case 200: // seta para esquerda
            posicaoCamera.x -= 0.5;
            break;
        case 201: // seta para cima
            posicaoCamera.y += 0.5;
            break;
        case 202: // seta para a direita
            posicaoCamera.x += 0.5;
            break;
        case 203: // seta para baixo
            posicaoCamera.y -= 0.5;
            
            break;
        case 'w':
            posicaoCamera.z += 0.5;
            break;
        case 's':
            posicaoCamera.z -= 0.5;
            break;
    }
}

void keyboardUp(int key){
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{

   mouseX = x - screenWidth/2; 
   mouseY = y - screenHeight/2;


   if(isMousePressed){
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        rotacaoCamera.y += deltaX * 0.01f; 
        rotacaoCamera.x += deltaY * 0.01f;

        lastMouseX = x;
        lastMouseY = y;
   }

   if(button == 0 && state == 0){
        isMousePressed = true;
        lastMouseX = x;
        lastMouseY = y;
   }
   else if (state == 1){
        isMousePressed = false;
   }
}

int main(void)
{
   pecas[0] = new CilindroECubos(Vector3(0, 0, 0), Vector3(M_PI / 2, 0, 0), 20, 300, 20);
   pecas[1] = new CilindroECubos(Vector3(0, 0, -180), Vector3(M_PI / 2, M_PI / 2, 0), 10, 200, 4);
   pecas[2] = new CilindroECubos(Vector3(0, 0, -200 + 10), Vector3(M_PI / 2, 0, 0), 10, 200, 4);
   pecas[3] = new CilindroECubos(Vector3(0, 0, -100), Vector3(M_PI / 2, M_PI / 2, 0), 10, 200, 4);
   pecas[4] = new CilindroECubos(Vector3(0, 0, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);

   pecas[5] = new Engrenagem(Vector3(0, 0, 200), Vector3(0, 0, 0), 80, 100, 120);
   pecas[6] = new Engrenagem(Vector3(2 * 120 - 20, 0, 200), Vector3(0, 0, 0), 80, 100, 120);

   CV::init(screenWidth, screenHeight, "Rafael Carneiro Pregardier");
   CV::run();
}
