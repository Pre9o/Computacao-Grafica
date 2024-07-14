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


std::vector<Vector2> pontos_eixo;
std::vector<Vector2> pontos_retangulo;
std::vector<Vector2> pontos_virabrequim;
std::vector<Vector2> pontos_cilindro;

std::vector<Vector3> pontos_atuais_eixo;
std::vector<Vector3> pontos_atuais_retangulo;
std::vector<Vector3> pontos_atuais_virabrequim;
std::vector<Vector3> pontos_atuais_cilindro;

std::vector<Vector3> eixo;
std::vector<Vector3> retangulo;
std::vector<Vector3> virabrequim;
std::vector<Vector3> cilindro;

Vector3 camera_pos = Vector3(0, 0, 0);

float camera_rot_x = 0.0f;
float camera_rot_y = 0.0f;
float camera_rot_z = 0.0f;

bool isMousePressed = false;
int lastMouseX, lastMouseY;


Vector3 Rotacao(Vector3 pontos, float angulo_x, float angulo_y, float angulo_z){
    float radianosX = angulo_x * PI / 180;
    float radianosY = angulo_y * PI / 180;
    float radianosZ = angulo_z * PI / 180;

    float x = pontos.x;
    float y = pontos.y;
    float z = pontos.z;

    // Rotação em torno do eixo X
    pontos.y = y * cos(radianosX) - z * sin(radianosX);
    pontos.z = y * sin(radianosX) + z * cos(radianosX);

    y = pontos.y;
    z = pontos.z;

    // Rotação em torno do eixo Y
    pontos.x = x * cos(radianosY) + z * sin(radianosY);
    pontos.z = -x * sin(radianosY) + z * cos(radianosY);

    x = pontos.x;
    z = pontos.z;

    // Rotação em torno do eixo Z
    pontos.x = x * cos(radianosZ) - y * sin(radianosZ);
    pontos.y = x * sin(radianosZ) + y * cos(radianosZ);


    return pontos;
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
        CV::color(0, 0, 1);
        CV::line(pontos[i], pontos[(i + 1) % numLados]);
    }

    // Desenhar arestas da base superior
    for (int i = numLados; i < 2 * numLados; i++) {
        CV::color(0, 1, 1);
        CV::line(pontos[i], pontos[(i + 1) % numLados + numLados]);
    }

    // Desenhar arestas verticais
    for (int i = 0; i < numLados; i++) {
        CV::color(1, 0, 1);
        CV::line(pontos[i], pontos[i + numLados]);
    }

    printf("Cilindro: %d\n", pontos.size());
}

void DrawConexaoVirabrequim(std::vector<Vector2> pontos) {
    // Desenhar arestas do retângulo
    for (int i = 0; i < 4; i++) {
        CV::color(1, 0.2, 1);
        //CV::line(pontos[i], pontos[(i + 1) % 4]); // arestas inferiores
        CV::color(0.2, 0.5, 0);
        //CV::line(pontos[i + 4], pontos[((i + 1) % 4) + 4]); // arestas superiores
        CV::color(0.2, 1, 0.8);
        CV::line(pontos[i], pontos[i + 4]); // arestas verticais
    }
    
}
void DrawRetangulo(std::vector<Vector2> pontos) {
    
}


 void GerarPartesVirabrequim(int numLados, float altura, float raio, int deslocamento) {
    for (int i = 0; i < numLados; i++) {
        float angulo = 2 * PI * i / numLados;
        float x = deslocamento + raio * cos(angulo);
        float y = deslocamento + raio * sin(angulo);
        if(deslocamento == 0){
            printf("Cilindro um: %f %f %f %d\n", x, y, altura / 2, i);
            eixo.push_back(Vector3(x, y, -altura / 2));
        }
        else{
            printf("Cilindro dois: %f %f %f\n", x, y, -altura / 2);
            virabrequim.push_back(Vector3(x, y, -altura / 2));
        }
    }

    for (int i = 0; i < numLados; i++) {
        float angulo = 2 * PI * i / numLados;
        float x = deslocamento + raio * cos(angulo);
        float y = deslocamento + raio * sin(angulo);
        if(deslocamento == 0){
            eixo.push_back(Vector3(x, y, altura / 2));
        }
        else{
            virabrequim.push_back(Vector3(x, y, altura / 2));
        }
    }

    printf("Virabrequim: %d\n", virabrequim.size());
}

//gerar retangulo com base em um ponto no meio do cilindro
void GerarConexaoVirabrequim(std::vector<Vector3> eixo, std::vector<Vector3> cilindro_dois, float largura, float altura) {
    retangulo.push_back(Vector3(eixo[0].x, eixo[0].y, eixo[0].z));
    retangulo.push_back(Vector3(eixo[10].x, eixo[10].y, eixo[10].z + 0.5));
    retangulo.push_back(Vector3(eixo[0].x, eixo[0].y, eixo[0].z + 0.5));
    retangulo.push_back(Vector3(eixo[10].x, eixo[10].y, eixo[10].z));

    retangulo.push_back(Vector3(cilindro_dois[30].x, cilindro_dois[30].y, cilindro_dois[30].z));
    retangulo.push_back(Vector3(cilindro_dois[20].x, cilindro_dois[20].y, cilindro_dois[20].z + 0.5));
    retangulo.push_back(Vector3(cilindro_dois[30].x, cilindro_dois[30].y, cilindro_dois[30].z + 0.5));
    retangulo.push_back(Vector3(cilindro_dois[20].x, cilindro_dois[20].y, cilindro_dois[20].z));
}

void GerarCilindro(int numLados, float altura, float raio, int deslocamento) {
    for (int i = 0; i < numLados; i++) {
        float angulo = 2 * PI * i / numLados;
        float x =  raio * cos(angulo);
        float y =  raio * sin(angulo);
        cilindro.push_back(Vector3(Rotacao(Vector3(x, y, -altura / 2 - deslocamento), 90, 0, 0)));
    }

    for (int i = 0; i < numLados; i++) {
        float angulo = 2 * PI * i / numLados;
        float x =  raio * cos(angulo);
        float y =  raio * sin(angulo);
        cilindro.push_back(Vector3(Rotacao(Vector3(x, y, altura / 2 - deslocamento), 90, 0, 0)));
    }

    printf("Cilindro: %d\n", cilindro.size());
}

std::vector<Vector3 AtualizarCilindro(std::vector<Vector3> cilindro, std::vector<Vector3> virabrequim, float altura, float raio) {
    Vector3 direcao = virabrequim[0] - cilindro[0];
    direcao.normalize();

    for (int i = 0; i < cilindro.size(); i++) {
        cilindro[i] = cilindro[i] + direcao * altura;
    }

    for (int i = 0; i < cilindro.size(); i++) {
        cilindro[i] = Rotacao(cilindro[i], 0, 0, 0);
    }

    printf("Cilindro: %d\n", cilindro.size());

}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
    CV::clear(0, 0, 0);

    CV::translate(screenHeight/2, screenWidth/2);


    for (int i = 0; i < eixo.size(); i++) {
        pontos_atuais_eixo[i] = WorldToCamera(eixo[i]);
        pontos_atuais_eixo[i] = Rotacao(pontos_atuais_eixo[i], angulo_x, angulo_y, angulo_z);
        pontos_atuais_eixo[i] = Translacao(pontos_atuais_eixo[i], 50);

        pontos_eixo[i] = Projecao(pontos_atuais_eixo[i], 10000);
    }
    DrawCilindro(pontos_eixo);

    for (int i = 0; i < virabrequim.size(); i++) {
        pontos_atuais_virabrequim[i] = WorldToCamera(virabrequim[i]);
        pontos_atuais_virabrequim[i] = Rotacao(pontos_atuais_virabrequim[i], angulo_x, angulo_y, angulo_z);
        pontos_atuais_virabrequim[i] = Translacao(pontos_atuais_virabrequim[i], 50);

        pontos_virabrequim[i] = Projecao(pontos_atuais_virabrequim[i], 10000);
    }
    DrawCilindro(pontos_virabrequim);

    for (int i = 0; i < retangulo.size(); i++) {
        pontos_atuais_retangulo[i] = WorldToCamera(retangulo[i]);
        pontos_atuais_retangulo[i] = Rotacao(pontos_atuais_retangulo[i], angulo_x, angulo_y, angulo_z);
        pontos_atuais_retangulo[i] = Translacao(pontos_atuais_retangulo[i], 50);

        pontos_retangulo[i] = Projecao(pontos_atuais_retangulo[i], 10000);
    }
    DrawConexaoVirabrequim(pontos_retangulo);

    for (int i = 0; i < cilindro.size(); i++) {
        pontos_atuais_cilindro[i] = AtualizarCilindro(cilindro, virabrequim, 3.0, 0.5);
        pontos_atuais_cilindro[i] = WorldToCamera(cilindro[i]);
        pontos_atuais_cilindro[i] = Rotacao(pontos_atuais_cilindro[i], angulo_x, angulo_y, angulo_z);
        pontos_atuais_cilindro[i] = Translacao(pontos_atuais_cilindro[i], 50);

        pontos_cilindro[i] = Projecao(pontos_atuais_cilindro[i], 10000);
    }
    DrawCilindro(pontos_cilindro);

    angulo_z += 0;

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
        case 200: // seta para esquerda
            camera_pos.x -= 0.5;
            break;
        case 201: // seta para cima
            camera_pos.y += 0.5;
            break;
        case 202: // seta para a direita
            camera_pos.x += 0.5;
            break;
        case 203: // seta para baixo
            camera_pos.y -= 0.5;
            
            break;
        case 'w':
            camera_pos.z += 0.5;
            break;
        case 's':
            camera_pos.z -= 0.5;
            break;
        case 'x': // rotaciona no eixo X
            angulo_x += 5;
            break;
        case 'y': // rotaciona no eixo Y
            angulo_y += 5;
            break;
        case 'z': // rotaciona no eixo Z
            angulo_z += 1;
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
    int numLados = 40; // Número de lados do cilindro
    float altura = 3.0; // Altura do cilindro
    float raio = 0.5; // Raio do cilindro
    int deslocamento = 0;

    GerarPartesVirabrequim(numLados, altura, raio, deslocamento);

    for (int i = 0; i < eixo.size(); i++) {
        pontos_eixo.push_back(Projecao(eixo[i], 100));
    }

    pontos_atuais_eixo = eixo;

    int numLados_cilindro_dois = 40; // Número de lados do cilindro
    float altura_cilindro_dois = 3.0; // Altura do cilindro
    float raio_cilindro_dois = 0.5; // Raio do cilindro
    int deslocamento_cilindro_dois = 2;

    GerarPartesVirabrequim(numLados_cilindro_dois, altura_cilindro_dois, raio_cilindro_dois, deslocamento_cilindro_dois);

    for (int i = 0; i < virabrequim.size(); i++) {
        pontos_virabrequim.push_back(Projecao(virabrequim[i], 100));
    }

    pontos_atuais_virabrequim = virabrequim;

    float largura = 1.0; // Largura do retângulo
    float altura_retangulo = 1.0; // Altura do retângulo

    GerarConexaoVirabrequim(eixo, virabrequim, largura, altura_retangulo);

    for (int i = 0; i < retangulo.size(); i++) {
        pontos_retangulo.push_back(Projecao(retangulo[i], 100));
    }

    pontos_atuais_retangulo = retangulo;

    int numLados_cilindro = 40; // Número de lados do cilindro
    float altura_cilindro = 3.0; // Altura do cilindro
    float raio_cilindro = 0.5; // Raio do cilindro
    int deslocamento_cilindro = 7;

    GerarCilindro(numLados_cilindro, altura_cilindro, raio_cilindro, deslocamento_cilindro);

    for (int i = 0; i < cilindro.size(); i++) {
        pontos_cilindro.push_back(Projecao(cilindro[i], 100));
    }

    pontos_atuais_cilindro = cilindro;

   CV::init(screenWidth, screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
