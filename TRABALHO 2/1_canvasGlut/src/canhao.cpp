#include "Canhao.h"

Canhao::Canhao(){
    mouse_pos = Vector2(0, 0);
    origem = Vector2(0, 0);
    vetor_direcao = Vector2(0, 0);
    angulo_circulo_mouse = 0;
}

void Canhao::setCanhao(Tabuleiro& tabuleiro){
    this->origem = Vector2((tabuleiro.extremos_tabuleiro[0].x + tabuleiro.extremos_tabuleiro[1].x) / 2, tabuleiro.extremos_tabuleiro[3].y + 10);

    this->angulo_circulo_mouse = atan2(mouse_pos.y - origem.y, mouse_pos.x - origem.x);

    this->vetor_direcao = mouse_pos - origem;
    this->vetor_direcao.normalize();
}

void Canhao::setMousePos(Vector2 pos){
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

void Canhao::desenhaCanhao(){
    // Calcular a posição inicial da linha na borda do círculo
    Vector2 inicio_linha = origem + Vector2(cos(angulo_circulo_mouse - M_PI/2), sin(angulo_circulo_mouse - M_PI/2)) * 10;

    CV::color(0.5, 0.5, 0.5);
    CV::line(inicio_linha, inicio_linha + vetor_direcao * 50);

    // Calcular a posição inicial da segunda linha na borda oposta do círculo
    Vector2 inicio_linha_oposta = origem + Vector2(cos(angulo_circulo_mouse + M_PI/2), sin(angulo_circulo_mouse + M_PI/2)) * 10;

    CV::color(0.5, 0.5, 0.5);
    CV::line(inicio_linha_oposta, inicio_linha_oposta + vetor_direcao * 50);

    CV::color(0.5, 0.5, 0.5);
    CV::circleFill(origem, 10, 20);

    CV::color(0.5, 0.5, 0.5);
    CV::rectFill(inicio_linha, inicio_linha_oposta, inicio_linha_oposta + vetor_direcao * 50, inicio_linha + vetor_direcao * 50);

    
    CV::color(0.5, 0.5, 0.5);
    CV::circle((inicio_linha.operator+(inicio_linha_oposta)).operator/(2) + vetor_direcao * 50, 10, 20);
}

void Canhao::desenhaBocaCanhao(){
    Vector2 inicio_linha = origem + Vector2(cos(angulo_circulo_mouse - M_PI/2), sin(angulo_circulo_mouse - M_PI/2)) * 10;
    Vector2 inicio_linha_oposta = origem + Vector2(cos(angulo_circulo_mouse + M_PI/2), sin(angulo_circulo_mouse + M_PI/2)) * 10;

    CV::color(1, 1, 1);
    CV::circleFill((inicio_linha.operator+(inicio_linha_oposta)).operator/(2) + vetor_direcao * 50, 10, 20);
}