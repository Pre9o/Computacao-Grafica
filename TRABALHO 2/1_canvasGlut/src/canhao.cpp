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

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1000, screenHeight = 1000;


Relogio *r = NULL;
Botao   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
int opcao  = 50;//variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Vector2 mouse_pos = Vector2(0, 0);

float angulo_circulo_mouse = 0;

/*struct Bola
{
    Vector2 pos;
    float raio;
    float velocidade;

    Bola()
    {
        pos = Vector2(0, 0);
        raio = 10;
        velocidade = 0;
    }

    void move(float dx, float dy)
    {
        pos.x += dx;
        pos.y += dy;
    }
};

std::vector<Bola> bolas;*/

void DesenhaSenoide()
{
   float x=0, y;
   CV::color(1);
   CV::translate(20, 200); //desenha o objeto a partir da coordenada (200, 200)     
   for(float i=0; i < 68; i+=0.001)
   {
      y = sin(i)*50;
      CV::point(x, y);
      x+=0.01;
   }
   CV::translate(0, 0);
}

void DesenhaLinhaDegrade()
{
   Vector2 p;
   for(float i=0; i<350; i++)
   {
	  CV::color(i/200, i/200, i/200);
	  p.set(i+100, 240);
	  CV::point(p);
   }

   //desenha paleta de cores predefinidas na Canvas2D.
   for(int idx = 0; idx < 14; idx++)
   {
	  CV::color(idx);
      CV::translate(20 + idx*30, 100);
	  CV::rectFill(Vector2(0,0), Vector2(30, 30));
   }
   CV::translate(0, 0);
}

void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", screenWidth, screenHeight);
    CV::text(10,320, str);
}

void canhao_atirar(){
    printf("\nAtirou");
}



//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa fun��o com poucas linhas de codigo.
void render()
{
    //pintar o fundo de preto
    CV::clear(0, 0, 0);

    Vector2 origem = Vector2(screenHeight/2, screenWidth/2);

    angulo_circulo_mouse = atan2(mouse_pos.y - origem.y, mouse_pos.x - origem.x);

    Vector2 direcao = mouse_pos - origem;
    direcao.normalize();

    // Calcular a posição inicial da linha na borda do círculo
    Vector2 inicio_linha = origem + Vector2(cos(angulo_circulo_mouse - M_PI/2), sin(angulo_circulo_mouse - M_PI/2)) * 10;

    CV::color(1, 0, 0);
    CV::line(inicio_linha, inicio_linha + direcao * 200);

    // Calcular a posição inicial da segunda linha na borda oposta do círculo
    Vector2 inicio_linha_oposta = origem + Vector2(cos(angulo_circulo_mouse + M_PI/2), sin(angulo_circulo_mouse + M_PI/2)) * 10;

    CV::color(0, 1, 0);
    CV::line(inicio_linha_oposta, inicio_linha_oposta + direcao * 200);

    CV::color(1, 1, 1);
    CV::circle(origem, 10, 20);

    CV::circle((inicio_linha.operator+(inicio_linha_oposta)).operator/(2) + direcao * 200, 10, 20);

    

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

	  //seta para a esquerda
      case 200:
	  break;

	  //seta para a direita
	  case 202:
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

   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;

   mouse_pos.set(x, y);

   //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);

   if( state == 0 ) //clicou
   {
    canhao_atirar();

       if( bt->Colidiu(x, y) )
       {
           printf("\nClicou no botao\n");
       }
   }
}

int main(void)
{
   r = new Relogio();
   bt = new Botao(200, 400, 140, 50, "Sou um botao");

   CV::init(screenWidth, screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
   CV::run();
}
