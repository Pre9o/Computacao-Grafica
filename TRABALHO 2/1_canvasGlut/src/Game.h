#include "gl_canvas2d.h"
#include "Vector2.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include <chrono>

#define M_PI 3.14159265358979323846

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
                CV::rectFill(extremos_bloco[0], extremos_bloco[1], extremos_bloco[2], extremos_bloco[3]);
                CV::color(1, 1, 1);
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
                if(i == 7){
                    Vector2 p1 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80));
                    Vector2 p2 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80));
                    Vector2 p3 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80) + 80);
                    Vector2 p4 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80) + 80);
                    matriz_tabuleiro[i][j].setExtremosBloco({p1, p2, p3, p4});
                }
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
            CV::color(1, 1, 1);
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

class Canhao{
    public:
    Vector2 mouse_pos;
    Vector2 origem;
    Vector2 vetor_direcao;

    Vector2 inicio_linha;
    Vector2 inicio_linha_oposta;

    float angulo_circulo_mouse;

    Canhao(){
        mouse_pos = Vector2(0, 0);
        origem = Vector2(0, 0);
        vetor_direcao = Vector2(0, 0);
        angulo_circulo_mouse = 0;
    }

    void setMousePos(Vector2 pos){
        Vector2 vetor_direcao_temp = pos - origem;
        float angulo = atan2(vetor_direcao_temp.y, vetor_direcao_temp.x);

        // Convertendo o ângulo para graus e ajustando para que 0º seja para cima
        angulo = (angulo * 180 / M_PI);
        if (angulo < 0) angulo += 360;

        // Verificando se o ângulo está entre 0º e 180º
        if (angulo >= 0 && angulo <= 180) {
            mouse_pos = pos;
            vetor_direcao = vetor_direcao_temp;
        }
    }

    void desenhaCanhao(std::vector<Vector2> extremos_tabuleiro){
        origem = Vector2((extremos_tabuleiro[0].x + extremos_tabuleiro[1].x) / 2, extremos_tabuleiro[3].y + 10);
        angulo_circulo_mouse = atan2(mouse_pos.y - origem.y, mouse_pos.x - origem.x);

        Vector2 vetor_direcao = mouse_pos - origem;
        vetor_direcao.normalize();

        // Calcular a posição inicial da linha na borda do círculo
        Vector2 inicio_linha = origem + Vector2(cos(angulo_circulo_mouse - M_PI/2), sin(angulo_circulo_mouse - M_PI/2)) * 10;

        CV::color(1, 0, 0);
        CV::line(inicio_linha, inicio_linha + vetor_direcao * 50);

        // Calcular a posição inicial da segunda linha na borda oposta do círculo
        Vector2 inicio_linha_oposta = origem + Vector2(cos(angulo_circulo_mouse + M_PI/2), sin(angulo_circulo_mouse + M_PI/2)) * 10;

        CV::color(0, 1, 0);
        CV::line(inicio_linha_oposta, inicio_linha_oposta + vetor_direcao * 50);

        CV::color(1, 1, 1);
        CV::circle(origem, 10, 20);

        CV::circle((inicio_linha.operator+(inicio_linha_oposta)).operator/(2) + vetor_direcao * 50, 10, 20);
    }
};


class Bola{
    public:
        Vector2 posicao;
        int velocidade;
        Vector2 direcao;
        float raio;
        int cor;

    Bola(){
        posicao = Vector2(0, 0);
        velocidade = 5;
        direcao = Vector2(0, 0);
        raio = 5;
        cor = 2;
    }

    void lancarBola(Canhao& canhao){
        posicao = canhao.origem;
        direcao = canhao.vetor_direcao;
        direcao.normalize();
        velocidade = 5;
    }

    void desenhaBola(){
        CV::color(cor);
        CV::circle(posicao, raio, 50);
    }

    void moverBola(float deltaTime){
        posicao = posicao + direcao * velocidade * deltaTime;
    }

};


class Controle{
    public:
    int nivel;
    int pontos;
    std::vector<Bola> bolas;

    bool jogando;

    Controle(){
        nivel = 1;
        pontos = 0;
        jogando = true;
    }

    void setBolas(){
        Bola bola;
        bolas.push_back(bola);
    }

    void executaJogada(Canhao& canhao, clock_t deltaTime, bool *primeira_vez){
    printf("Quantidade de bolas: %d\n", bolas.size());
    if(*primeira_vez){
        bolas[0].lancarBola(canhao);
        *primeira_vez = false;
    }

    for(auto& bola: bolas){
        bola.moverBola(deltaTime);
    }
    }
};
