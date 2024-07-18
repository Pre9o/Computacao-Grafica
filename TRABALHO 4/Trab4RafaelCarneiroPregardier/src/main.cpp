/*
Este programa é um motor 3D que renderiza um motor de um carro. O motor é composto por cilindros, cubos e engrenagens. O motor é renderizado em 3D e pode ser movido com o mouse e teclado.

Classes:
- CilindroECubos: Classe que representa os cilindros e cubos.
- Engrenagem: Classe que representa uma engrenagem.
- FuncoesGerais3D: Classe que gera as peças do motor.
- Pecas: Classe base para as peças do motor.

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
#include <math.h>
#include <chrono>
#include <vector>
#include <limits>

#include "gl_canvas2d.h"
#include "FuncoesGerais.h"

#define VETOR_MAX 9

int screenWidth = 1000, screenHeight = 1000;

int mouseX, mouseY; 

bool isMousePressed = false;
int lastMouseX, lastMouseY;

float velocidadeRotacao = 0.005;
float distancia = 400;

clock_t start = clock();

Pecas *pecas[VETOR_MAX];
FuncoesGerais3D funcoesGerais;

Vector3 posicaoCamera = Vector3(0, 0, 0);
Vector3 rotacaoCamera = Vector3(0, 0, 0);

/**
 * Funcao que renderiza
 * 
 * @return void
 * 
 */
void render(){
    clock_t end = clock();
    float duration = (float)(end - start) / CLOCKS_PER_SEC;
    float frameDeltaTime = 1.0f / 60.0f;

    CV::clear(0, 0, 0);
    CV::translate(screenHeight/2, screenWidth/2);

    funcoesGerais.executar3D(pecas, posicaoCamera, rotacaoCamera, distancia, velocidadeRotacao);
            
    if (duration < frameDeltaTime){
        Sleep((frameDeltaTime - duration) * 1000);
        start = clock();
    }

    }

/**
 * Funcao que trata o teclado
 * 
 * @param key int
 * 
 * @return void
 */
void keyboard(int key){
    switch(key){
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

/**
 * Funcao que trata o mouse
 * 
 * @param button int
 * @param state int
 * @param wheel int
 * @param direction int
 * @param x int
 * @param y int
 * 
 * @return void
 */
void mouse(int button, int state, int wheel, int direction, int x, int y){

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

/**
 * Funcao principal
 * 
 * 
 * @return int
 */
int main(void){
    funcoesGerais.definirPecas(pecas);

    CV::init(screenWidth, screenHeight, "Rafael Carneiro Pregardier");
    CV::run();
}
