#include "gl_canvas2d.h"
#include "Vector2.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

class Bloco{
    public:
    Vector2 tamanho;
    std::vector<Vector2> extremos_bloco;
    int cor;
    int tipo; //0 = Quadrado, 1 = Triangulo, 2 = Retangulo, 3 = PowerUp
    int chanceSerGerado;
    bool selecionado;
    int pontos;

    Bloco(){
        tamanho = Vector2(80, 80); //80x80
        cor = 0;
        tipo = 0;
        chanceSerGerado = 0;
        selecionado = false;
        pontos = 0;
    }

    void setExtremosBloco(std::vector<Vector2> extremos) {
        extremos_bloco = extremos;
    }

    void setBloco(){
        cor = rand() % 20;
        tipo = rand() % 100 < 80 ? 0 : rand() % 100 < 95 ? 1 : rand() % 100 < 99 ? 2 : 3;
        chanceSerGerado = rand() % 100 < 50 ? 1 : 0;
        selecionado = (chanceSerGerado == 1) ? true : false;
        pontos = 10;
    }

    void desenhaBloco(){
        for(int i = 0; i < extremos_bloco.size(); i++){
            if(selecionado){
                CV::color(cor);
                CV::line(extremos_bloco[i], extremos_bloco[(i + 1) % 4]);
                CV::color(1, 1, 1);
                CV::text(extremos_bloco[0].x - (CV::getTextWidth(pontos, GLUT_BITMAP_HELVETICA_18)/2) + (tamanho.y/2),
                        extremos_bloco[0].y + (tamanho.x/10) + (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
                        pontos,
                        GLUT_BITMAP_HELVETICA_18);
            }
        }
    }

    private:
    float chance_quadrado = 0.8;
    float chance_triangulo = 0.15;
    float chance_retangulo = 0.04;
    float chance_powerUp = 0.01;
};

class Tabuleiro {
    public:
    std::vector<Vector2> extremos_tabuleiro;
    Bloco matriz_tabuleiro[10][7];
    Bloco matriz_tabuleiro_aux[10][7];

    void setTabuleiro() {
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 7; j++){
                Vector2 p1 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80));
                Vector2 p2 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80));
                Vector2 p3 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80) + 80);
                Vector2 p4 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80) + 80);
                matriz_tabuleiro[i][j].setExtremosBloco({p1, p2, p3, p4});
            }
        }
    }

    void definirBlocos(){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 7; j++){
                matriz_tabuleiro[i][j].setBloco();
            }
        }
    }

    void desenhaTabuleiro() {
        for(int i = 0; i < extremos_tabuleiro.size(); i++){
            CV::color(1, 1, 0);
		    CV::line(extremos_tabuleiro[i], extremos_tabuleiro[(i + 1) % 4]);
	    }
    }

    void setExtremosTabuleiro(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
        extremos_tabuleiro[0] = p1; 
        extremos_tabuleiro[1] = p2;
        extremos_tabuleiro[2] = p3;
        extremos_tabuleiro[3] = p4;
    }
};