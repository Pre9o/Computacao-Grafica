#include "Bola.h"

bool Bola::operator==(const Bola& outra) const {
    // compara as bolas com base em suas posições e raios
    return posicao.x == outra.posicao.x && posicao.y == outra.posicao.y && direcao.x == outra.direcao.x && direcao.y == outra.direcao.y;
}

Bola::Bola(){
    posicao = Vector2(0, 0);
    velocidade = 0;
    direcao = Vector2(0, 0);
    raio = 0;
    atrasoInicial = 0.0;
}

void Bola::setBola(Canhao& canhao, double atrasoInicial){
    this->posicao = canhao.origem;
    this->velocidade = 500;
    this->direcao = canhao.vetor_direcao;
    this->direcao.normalize();
    this->raio = 5;
    this->atrasoInicial = atrasoInicial;
}

void Bola::desenhaBola(){
    CV::color(0.98823, 0.5098, 0.20392);
    CV::circleFill(posicao, raio, 50);
}

void Bola::moverBola(double deltaTime){
    this->posicao = this->posicao + this->direcao * this->velocidade * deltaTime;
}