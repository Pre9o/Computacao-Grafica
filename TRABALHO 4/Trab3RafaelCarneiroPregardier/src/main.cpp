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

#include "gl_canvas2d.h"

#include "Relogio.h"
#include "Botao.h"
#include <math.h>
#include <vector>
#include <limits>

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1000, screenHeight = 1000;


Relogio *r = NULL;
Botao   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Vector2 mouse_pos = Vector2(0, 0);

float angulo_mouse = 0;

float angulo_x = 0;
float angulo_y = 0;
float angulo_z = 0;

std::vector<Vector2> pontos;
std::vector<Vector3> pontos_atuais;

std::vector<Vector3> cubo;
std::vector<Vector3> cilindro;
Vector3 camera_pos = Vector3(0, 0, 0);

float camera_rot_x = 0.0f;
float camera_rot_y = 0.0f;
float camera_rot_z = 0.0f;

bool isMousePressed = false;
int lastMouseX, lastMouseY;


Vector3 Rotacao(Vector3 ponto_cubo, float angulo_x, float angulo_y, float angulo_z){
    float radianosX = angulo_x * PI / 180;
    float radianosY = angulo_y * PI / 180;
    float radianosZ = angulo_z * PI / 180;

    float x = ponto_cubo.x;
    float y = ponto_cubo.y;
    float z = ponto_cubo.z;

    // Rotação em torno do eixo X
    ponto_cubo.y = y * cos(radianosX) - z * sin(radianosX);
    ponto_cubo.z = y * sin(radianosX) + z * cos(radianosX);

    y = ponto_cubo.y;
    z = ponto_cubo.z;

    // Rotação em torno do eixo Y
    ponto_cubo.x = x * cos(radianosY) + z * sin(radianosY);
    ponto_cubo.z = -x * sin(radianosY) + z * cos(radianosY);

    x = ponto_cubo.x;
    z = ponto_cubo.z;

    // Rotação em torno do eixo Z
    ponto_cubo.x = x * cos(radianosZ) - y * sin(radianosZ);
    ponto_cubo.y = x * sin(radianosZ) + y * cos(radianosZ);


    return ponto_cubo;
}

Vector3 WorldToCamera(Vector3 ponto){
    // Translação da câmera
    ponto.x -= camera_pos.x;
    ponto.y -= camera_pos.y;
    ponto.z -= camera_pos.z;

    // Rotação da câmera
    ponto = Rotacao(ponto, -camera_rot_x, -camera_rot_y, -camera_rot_z);

    return ponto;
}


Vector2 Projecao(Vector3 ponto_cubo, float distance){
    float x;
    float y;

    x = ponto_cubo.x * distance / ponto_cubo.z;
    y = ponto_cubo.y * distance / ponto_cubo.z;

    return Vector2(x, y);
}

Vector3 Translacao(Vector3 ponto_cubo, float z){
    ponto_cubo.z += z;

    return ponto_cubo;
}


void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}


void DrawQuadrado(std::vector<Vector2> pontos)
{
    for(int i = 0; i < 4; i++){
        CV::line(pontos[i], pontos[(i + 1) % 4]); // arestas inferiores
        CV::line(pontos[i + 4], pontos[((i + 1) % 4) + 4]); // arestas superiores
        CV::line(pontos[i], pontos[i + 4]); // arestas verticais
    }
}

void DrawCilindro(std::vector<Vector2> pontos) {
    int numLados = pontos.size() / 2;

    // Desenhar arestas da base inferior
    for (int i = 0; i < numLados; i++) {
        CV::line(pontos[i], pontos[(i + 1) % numLados]);
    }

    // Desenhar arestas da base superior
    for (int i = numLados; i < 2 * numLados; i++) {
        CV::line(pontos[i], pontos[(i + 1) % numLados + numLados]);
    }

    // Desenhar arestas verticais
    for (int i = 0; i < numLados; i++) {
        CV::line(pontos[i], pontos[i + numLados]);
    }
}


 void GerarCilindro(int numLados, float altura, float raio) {
    for (int i = 0; i < numLados; i++) {
        float angulo = 2 * PI * i / numLados;
        float x = raio * cos(angulo);
        float y = raio * sin(angulo);
        cilindro.push_back(Vector3(x, y, -altura / 2));
    }

    for (int i = 0; i < numLados; i++) {
        float angulo = 2 * PI * i / numLados;
        float x = raio * cos(angulo);
        float y = raio * sin(angulo);
        cilindro.push_back(Vector3(x, y, altura / 2));
    }
}


//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
    CV::clear(0, 0, 0);

    CV::translate(screenHeight/2, screenWidth/2);

    //for(int i = 0; i < cubo.size(); i++){
    //    pontos_atuais[i] = Rotacao(cubo[i], angulo_cubo);
    //    pontos_atuais[i] = Translacao(pontos_atuais[i], 100);
     //   pontos[i] = Projecao(pontos_atuais[i], 10000);
   // }    

    //DrawQuadrado(pontos);


    for (int i = 0; i < cilindro.size(); i++) {
        pontos_atuais[i] = WorldToCamera(cilindro[i]);
        pontos_atuais[i] = Rotacao(pontos_atuais[i], angulo_x, angulo_y, angulo_z);
        pontos_atuais[i] = Translacao(pontos_atuais[i], 50);

        pontos[i] = Projecao(pontos_atuais[i], 10000);
    }
    DrawCilindro(pontos);


    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    printf("\nTecla: %d" , key);
    if( key < 200 )
    {
        opcao = key;
    }

    switch(key)
    {
        case 27:
            exit(0);
            break;

        // setas para mover a câmera
        case 200: // seta para cima
            camera_pos.y += 0.5;
            break;
        case 201: // seta para baixo
            camera_pos.y -= 0.5;
            break;
        case 202: // seta para a direita
            camera_pos.x += 0.5;
            break;
        case 203: // seta para a esquerda
            camera_pos.x -= 0.5;
            break;
        case 'w':
            camera_pos.z += 0.5;
            break;
        case 's':
            camera_pos.z -= 0.5;
            break;

        // teclas para rotacionar a câmera
        case 'i':
            camera_rot_x += 5.0;
            break;
        case 'k':
            camera_rot_x -= 5.0;
            break;
        case 'j':
            camera_rot_y += 5.0;
            break;
        case 'l':
            camera_rot_y -= 5.0;
            break;
        case 'u':
            camera_rot_z += 5.0;
            break;
        case 'o':
            camera_rot_z -= 5.0;
            break;
    }
}


//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{

   mouseX = x - screenWidth/2; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y - screenHeight/2;


   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if(isMousePressed){
        int deltaX = x - lastMouseX;
        int deltaY = y - lastMouseY;

        // Ajusta os ângulos de rotação da câmera com base nos movimentos do mouse
        camera_rot_y += deltaX * 0.1f; // sensibilidade do mouse
        camera_rot_x += deltaY * 0.1f;

        // Atualiza a posição do mouse para a próxima chamada
        lastMouseX = x;
        lastMouseY = y;
   }

    
   if(button == 0 && state == 0){
        isMousePressed = true;
        lastMouseX = x;
        lastMouseY = y;
   }else if (state == 1){
        isMousePressed = false;
   }
}

int main(void)
{
   //inicializa a Canvas
    int numLados = 20; // Número de lados do cilindro
    float altura = 1.0; // Altura do cilindro
    float raio = 0.5; // Raio do cilindro

    GerarCilindro(numLados, altura, raio);

    for (int i = 0; i < cilindro.size(); i++) {
        pontos.push_back(Projecao(cilindro[i], 100));
    }

    pontos_atuais = cilindro;


    //for(int i = 0; i < cubo.size(); i++){
    //    pontos.push_back(Projecao(cubo[i], 100));
    //}

    //pontos_atuais = cubo;

   CV::init(screenWidth, screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
