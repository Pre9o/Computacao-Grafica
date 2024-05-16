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
    bool ativo;
    int pontos;

    Bloco(){
        tamanho = Vector2(80, 80); //80x80
        cor = 0;
        tipo = 0;
        this->ativo = false;
        pontos = 0;
    }

    void setExtremosBloco(std::vector<Vector2> extremos) {
        extremos_bloco = extremos;
    }

    void setBloco(int i, int pontos, std::vector<Bloco>& blocos_ativos, int blocos_maximo_do_nivel){
        cor = rand() % 20;
        tipo = 0;
        this->ativo = this->definirBlocoAtivo(i, blocos_ativos, blocos_maximo_do_nivel);
        this->pontos = pontos;
    }

    bool definirBlocoAtivo(int i, std::vector<Bloco>& blocos_ativos, int blocos_maximo_do_nivel){
        if(i == 7 && blocos_ativos.size() < blocos_maximo_do_nivel){
            if(rand() % 100 < 50){
                printf("BLOCO ATIVO\n");
                blocos_ativos.push_back(*this);
                return true;
            }
            return false;
        }
        return false;
    }

    Vector2 getPosicao(){
        return extremos_bloco[0];
    }

    Vector2 getTamanho(){
        return tamanho;
    }

    void diminuirPontos(){
        this->pontos = this->pontos -= 1;
        if(this->pontos == 0){
            this->ativo = false;
        }
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
    std::vector<Bloco> blocos_ativos;

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

    int verificaBlocosAtivos(){
        int blocos_ativos = 0;
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 7; j++){
                if(matriz_tabuleiro[i][j].ativo == true){
                    blocos_ativos++;
                }
            }
        }
        return blocos_ativos;
    }

    void definirBlocos(int pontos, int blocos_maximo_do_nivel){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 7; j++){
                matriz_tabuleiro[i][j].setBloco(i, pontos, blocos_ativos, blocos_maximo_do_nivel);
            }
        }
    }

    void desenhaTabuleiro() {
        for(int i = 0; i < extremos_tabuleiro.size(); i++){
            CV::color(1, 1, 1);
		    CV::line(extremos_tabuleiro[i], extremos_tabuleiro[(i + 1) % 4]);
	    }
    }

    void reorganizaTabuleiro(int pontos, int blocos_maximo_do_nivel){
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 7; j++){
                matriz_tabuleiro[i][j] = matriz_tabuleiro[i+1][j];
            }
        }
        // Limpa a última linha após o deslocamento
        for(int j = 0; j < 7; j++){
            matriz_tabuleiro[9][j].setBloco(9, pontos, blocos_ativos, blocos_maximo_do_nivel);
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
        this->origem = Vector2((tabuleiro.extremos_tabuleiro[0].x + tabuleiro.extremos_tabuleiro[1].x) / 2, tabuleiro.extremos_tabuleiro[3].y + 10);

        this->angulo_circulo_mouse = atan2(mouse_pos.y - origem.y, mouse_pos.x - origem.x);

        this->vetor_direcao = mouse_pos - origem;
        this->vetor_direcao.normalize();
    }

    void setMousePos(Vector2 pos){
        Vector2 vetor_direcao_temp = pos - origem;
        float angulo = atan2(vetor_direcao_temp.y, vetor_direcao_temp.x);

        // Convertendo o ângulo para graus e ajustando para que 0º seja para cima
        angulo = (angulo * 180 / M_PI);
        if (angulo < 0) angulo += 360;

        // Verificando se o ângulo está entre 0º e 180º
        if (angulo >= 0 && angulo <= 180) {
            this->mouse_pos = pos;
            this->vetor_direcao = vetor_direcao_temp;
            this->vetor_direcao.normalize();
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
        double atrasoInicial;// adiciona um contador de tempo para cada bola
        
        bool operator==(const Bola& outra) const {
        // compara as bolas com base em suas posições e raios
        return posicao.x == outra.posicao.x && posicao.y == outra.posicao.y && direcao.x == outra.direcao.x && direcao.y == outra.direcao.y;
    }

    Bola(){
        posicao = Vector2(0, 0);
        velocidade = 0;
        direcao = Vector2(0, 0);
        raio = 0;
        cor = 2;
        atrasoInicial = 0.0; 
    }

    void setBola(Canhao& canhao){
        this->posicao = canhao.origem;
        this->velocidade = 500;
        this->direcao = canhao.vetor_direcao;
        this->direcao.normalize();
        this->raio = 5;
    }

    void desenhaBola(){
        CV::color(1, 1, 1);
        CV::circleFill(posicao, raio, 50);
    }

    void moverBola(double deltaTime){
        this->posicao = this->posicao + this->direcao * this->velocidade * deltaTime;
    }
    
};


class Controle{
    public:
    int nivel;
    int pontosIniciaisDoBloco;
    int blocosMaximoDoNivel;
    std::vector<Bola> bolas;
    Tabuleiro tabuleiro;
    Canhao canhao;

    bool jogando;

    Controle(){
        nivel = 1;
        pontosIniciaisDoBloco = 0;
        jogando = false;
    }

    void adicionarBolas(int num_bolas){
        for(int i = 0; i < num_bolas; i++){
            Bola bola;
            bolas.push_back(bola);
        }
    }

    void setTabuleiro(Tabuleiro tabuleiro){
        this->tabuleiro = tabuleiro;
    }

    void setCanhao(Canhao canhao){
        this->canhao = canhao;
    }


    bool testaColisaoTabuleiro(Tabuleiro& tabuleiro, Bola& bola){
        if(bola.posicao.x - bola.raio < tabuleiro.extremos_tabuleiro[0].x || bola.posicao.x + bola.raio > tabuleiro.extremos_tabuleiro[2].x){
            bola.direcao.x *= -1;
            return true;
        }
        if(bola.posicao.y + bola.raio > tabuleiro.extremos_tabuleiro[0].y){
            bola.direcao.y *= -1;
            return true;
        }
        if(bola.posicao.y - bola.raio < tabuleiro.extremos_tabuleiro[2].y){
            auto it = std::find(bolas.begin(), bolas.end(), bola); // encontra a bola no vetor
            if(it != bolas.end()){
                bolas.erase(it);
            }
        }

        return false;
    }
    
    bool testaColisaoBlocos(Tabuleiro& tabuleiro, Bola& bola){
        for(int i = 0; i < 10; i++){
            for(int j = 0; j < 7; j++){
                if(tabuleiro.matriz_tabuleiro[i][j].ativo == true){
                    Vector2 posicao_bloco = tabuleiro.matriz_tabuleiro[i][j].getPosicao();
                    Vector2 tamanho_bloco = tabuleiro.matriz_tabuleiro[i][j].getTamanho();

                    float distancia_x = 0;
                    float distancia_y = 0;

                    if(bola.posicao.x < posicao_bloco.x){
                        distancia_x = posicao_bloco.x + tamanho_bloco.x/2 - bola.posicao.x - bola.raio;
                    }
                    else if(bola.posicao.x >= posicao_bloco.x){
                        distancia_x = bola.posicao.x + bola.raio - (posicao_bloco.x + tamanho_bloco.x/2);
                    }
                    
                    if(bola.posicao.y < posicao_bloco.y){
                        distancia_y = posicao_bloco.y + tamanho_bloco.y/2 - bola.posicao.y - bola.raio;
                    }
                    else if(bola.posicao.y >= posicao_bloco.y){
                        distancia_y = bola.posicao.y + bola.raio - (posicao_bloco.y + tamanho_bloco.y/2);
                    }
                    
                    float dx = distancia_x - tamanho_bloco.x/2;
                    float dy = distancia_y - tamanho_bloco.y/2;


                    if(dx <= bola.raio && dy <= bola.raio){
                        printf("Posicao Bola: %f %f\n", bola.posicao.x, bola.posicao.y);

                        printf("POSICAO BLOCO: %f %f\n", posicao_bloco.x, posicao_bloco.y);
                        printf("TAMANHO BLOCO: %f %f\n", tamanho_bloco.x, tamanho_bloco.y);

                        printf("DISTANCIA X: %f\n", distancia_x);
                        printf("DISTANCIA Y: %f\n", distancia_y);

                        printf("DX: %f\n", dx);
                        printf("DY: %f\n", dy);

                        if(distancia_y > distancia_x){
                            if(dy > 0){
                                bola.direcao.y *= -1;
                            }
                            else{
                                bola.direcao.y *= -1;
                            }
                        }
                        else{
                            if(dx > 0){
                                bola.direcao.x *= -1;
                            }
                            else{
                                bola.direcao.x *= -1;
                            }
                        }
                        tabuleiro.matriz_tabuleiro[i][j].diminuirPontos();
                        return true;
                    }
                }
            }   
        }
        return false;
    }
    

    void executaJogada(double deltaTime){
        Bola bolaAntesColisao;
        for(auto& bola: bolas){
            if(bola.atrasoInicial > 0){
                bola.atrasoInicial -= deltaTime; // decrementa o atraso inicial
            } else {
                bolaAntesColisao = bola;
                bola.moverBola(deltaTime);
                if(testaColisaoTabuleiro(tabuleiro, bola) || testaColisaoBlocos(tabuleiro, bola)){
                    bola.posicao = bolaAntesColisao.posicao;
                }
            }
        }        
    }

    void controlaJogo(double deltaTime){
        this->executaJogada(deltaTime);


        if(this->bolas.size() == 0){
            this->jogando = false;
        }

        if(this->jogando == false){
            if(this->tabuleiro.verificaBlocosAtivos() == 0){
                this->nivel++;
                this->gerarNivel();
            }
            else{
                this->tabuleiro.reorganizaTabuleiro(this->pontosIniciaisDoBloco, this->blocosMaximoDoNivel);
                this->adicionarBolas(this->nivel);
            }
        }
    }

    void gerarNivel(){
        if(nivel % 10 != 0){
            printf("NIVEL: %d\n", nivel);
            this->pontosIniciaisDoBloco = nivel;
            this->blocosMaximoDoNivel = nivel % 10;
            this->tabuleiro.definirBlocos(nivel, blocosMaximoDoNivel);
            this->adicionarBolas(nivel);
        }
        else{
            printf("NIVEL: %d\n", nivel);
            this->pontosIniciaisDoBloco = nivel * 2;
            this->blocosMaximoDoNivel = nivel % 10 + 2;
            this->tabuleiro.definirBlocos(nivel % 10 + 1, blocosMaximoDoNivel);
            this->adicionarBolas(nivel);
        }
    }
    
};
