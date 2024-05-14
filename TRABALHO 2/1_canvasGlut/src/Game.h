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
    bool ativo;
    int pontos;

    Bloco(){
        tamanho = Vector2(80, 80); //80x80
        cor = 0;
        tipo = 0;
        chanceSerGerado = 0;
        ativo = false;
        pontos = 0;
    }

    void setExtremosBloco(std::vector<Vector2> extremos) {
        extremos_bloco = extremos;
    }

    void setBloco(int i){
        cor = rand() % 20;
        tipo = rand() % 100 < 80 ? 0 : rand() % 100 < 95 ? 1 : rand() % 100 < 99 ? 2 : 3;
        chanceSerGerado = rand() % 100 < 50 ? 1 : 0;
        ativo = (i == 7) ? true : false;
        pontos = 10;
    }

    void desenhaBloco(){
        for(int i = 0; i < extremos_bloco.size(); i++){
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
                matriz_tabuleiro[i][j].setBloco(i);
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

    Tabuleiro tabuleiro;

    float angulo_circulo_mouse;

    Canhao(){
        mouse_pos = Vector2(0, 0);
        origem = Vector2(0, 0);
        vetor_direcao = Vector2(0, 0);
        angulo_circulo_mouse = 0;
    }

    void setCanhao(Tabuleiro& tabuleiro){
        origem = Vector2((tabuleiro.extremos_tabuleiro[0].x + tabuleiro.extremos_tabuleiro[1].x) / 2, tabuleiro.extremos_tabuleiro[3].y + 10);

        angulo_circulo_mouse = atan2(mouse_pos.y - origem.y, mouse_pos.x - origem.x);

        vetor_direcao = mouse_pos - origem;
        vetor_direcao.normalize();

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

    void desenhaCanhao(){
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
        velocidade = 0;
        direcao = Vector2(0, 0);
        raio = 0;
        cor = 2;
    }

    void setBola(Canhao& canhao){
        this->posicao = canhao.origem;
        this->velocidade = 500;
        this->direcao = canhao.vetor_direcao;
        this->direcao.normalize();
        this->raio = 5;
    }

    void desenhaBola(){
        CV::color(cor);
        CV::circle(posicao, raio, 50);
    }

    void moverBola(double deltaTime){
        this->posicao = this->posicao + this->direcao * this->velocidade * deltaTime;
        //printf("POSICAO BOLA: %f %f\n", posicao.x, posicao.y);
        //printf("DIRECAO BOLA: %f %f\n", direcao.x, direcao.y);
        //printf("VELOCIDADE BOLA: %d\n", velocidade);
        //printf("DELTA TIME: %f\n", deltaTime);
    }
    
};


class Controle{
    public:
    int nivel;
    int pontos;
    std::vector<Bola> bolas;
    Tabuleiro tabuleiro;
    Canhao canhao;

    bool jogando;

    Controle(){
        nivel = 1;
        pontos = 0;
        jogando = false;
    }

    void adicionarBolas(int num_bolas){
        for(int i = 0; i < num_bolas; i++){
            Bola bola;
            bolas.push_back(bola);
            printf("POSICAO BOLA: %f %f\n", bola.posicao.x, bola.posicao.y);
        }
    }

    void setTabuleiro(Tabuleiro tabuleiro){
        this->tabuleiro = tabuleiro;
    }

    void setCanhao(Canhao canhao){
        this->canhao = canhao;
    }

    void executaJogada(double deltaTime){
        Bola bolaAntesColisao;
        for(auto& bola: bolas){
            bolaAntesColisao = bola;
            bola.moverBola(deltaTime);
            if(testaColisaoTabuleiro(tabuleiro, bola) || testaColisaoBlocos(tabuleiro, bola)){
                bola.posicao = bolaAntesColisao.posicao;
            }
        }
    }

    bool testaColisaoTabuleiro(Tabuleiro& tabuleiro, Bola& bola){
        if(bola.posicao.x - bola.raio < tabuleiro.extremos_tabuleiro[0].x || bola.posicao.x + bola.raio > tabuleiro.extremos_tabuleiro[2].x){
            bola.direcao.x *= -1;
            return true;
        }
        if(bola.posicao.y + bola.raio > tabuleiro.extremos_tabuleiro[0].y || bola.posicao.y - bola.raio < tabuleiro.extremos_tabuleiro[2].y){
            bola.direcao.y *= -1;
            return true;
        }
        return false;
    }
    
    bool testaColisaoBlocos(Tabuleiro& tabuleiro, Bola& bola){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 7; j++){
                if(tabuleiro.matriz_tabuleiro[i][j].ativo == true){
                    // Calcula a distância da bola e o quadrado do bloco
                    for(int k = 0; k < 4; k++){
                        float testX = bola.posicao.x;
                        float testY = bola.posicao.y;

                        if(bola.posicao.x < tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].x){
                            testX = tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].x;
                        }
                        else if(bola.posicao.x > tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].x + tabuleiro.matriz_tabuleiro[i][j].tamanho.x){
                            testX = tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].x + tabuleiro.matriz_tabuleiro[i][j].tamanho.x;
                        }
                        if(bola.posicao.y < tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].y){
                            testY = tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].y;
                        }
                        else if(bola.posicao.y > tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].y + tabuleiro.matriz_tabuleiro[i][j].tamanho.y){
                            testY = tabuleiro.matriz_tabuleiro[i][j].extremos_bloco[k].y + tabuleiro.matriz_tabuleiro[i][j].tamanho.y;
                        }

                        float distX = bola.posicao.x - testX;
                        float distY = bola.posicao.y - testY;

                        float distance = sqrt((distX * distX) + (distY * distY));

                        if(distance <= bola.raio){
                            // calcular em qual lado a bola colidiu
                            float dx = bola.posicao.x - testX;
                            float dy = bola.posicao.y - testY;

                            printf("dx: %f dy: %f\n", dx, dy);

                            if(dx > 0 || dy > 0){
                                printf("dx > 0 && dy > 0\n");
                                if(dx > dy){
                                    bola.direcao.x *= -1;
                                }
                                else{
                                    bola.direcao.y *= -1;
                                }
                            }
                            else if(dx > 0 || dy < 0){
                                printf("dx > 0 && dy < 0\n");
                                if(dx > -dy){
                                    bola.direcao.x *= -1;
                                }
                                else{
                                    bola.direcao.y *= -1;
                                }
                            }
                            else if(dx < 0 || dy > 0){
                                printf("dx < 0 && dy > 0\n");
                                if(-dx > dy){
                                    bola.direcao.x *= -1;
                                }
                                else{
                                    bola.direcao.y *= -1;
                                }
                            }
                            else if(dx < 0 || dy < 0){
                                printf("dx < 0 && dy < 0\n");
                                if(-dx > -dy){
                                    bola.direcao.x *= -1;
                                }
                                else{
                                    bola.direcao.y *= -1;
                                }
                            }
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }
};
