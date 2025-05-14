#include "Bola.h"

/**
 * Sobrecarga do operador de comparação.
 *
 * @param outra A bola a ser comparada.
 * @return Retorna true se as bolas forem iguais, caso contrário retorna false.
 */
bool Bola::operator==(const Bola& outra) const {
    return posicao.x == outra.posicao.x && posicao.y == outra.posicao.y && direcao.x == outra.direcao.x && direcao.y == outra.direcao.y;
}

/**
 * Construtor da classe Bola.
 *
 */
Bola::Bola() {
    posicao = Vector2(0, 0);
    velocidade = 0;
    direcao = Vector2(0, 0);
    raio = 0;
    atrasoInicial = 0.0;
    emAnimacao = false;
    progressoAnimacao = 0.0f;
}

/**
 * Define os atributos da bola com base nas informações do canhão.
 *
 * @param canhao O objeto Canhao que contém as informações necessárias.
 * @param atrasoInicial O atraso inicial da bola.
 */
void Bola::setBola(Canhao& canhao, double atrasoInicial){
    this->posicao = canhao.origem;
    this->velocidade = 1000;
    this->direcao = canhao.vetor_direcao;
    this->direcao.normalize();
    this->raio = 5;
    this->atrasoInicial = atrasoInicial;
}
/**
 * @brief Desenha a bola na tela.
 *
 */
void Bola::desenhaBola(){
    CV::color(0.98823, 0.5098, 0.20392);
    CV::circleFill(posicao, raio, 50);
}

/**
 * @brief Move a bola na tela.
 *
 * @param deltaTime O tempo que passou desde o último frame.
 */
void Bola::moverBola(double deltaTime){
    this->posicao = this->posicao + this->direcao * this->velocidade * deltaTime;
}

void Bola::iniciarAnimacaoRetorno(const Vector2& destino) {
    posicaoInicialAnimacao = posicao;
    posicaoFinalAnimacao = destino;
    progressoAnimacao = 0.0f;
    emAnimacao = true;
}

void Bola::atualizarAnimacao(float deltaTime) {
    if (!emAnimacao) return;

    progressoAnimacao += deltaTime * 2.0f; // Ajuste a velocidade da animação aqui
    if (progressoAnimacao >= 1.0f) {
        progressoAnimacao = 1.0f;
        emAnimacao = false;
    }

    // Interpolação linear entre a posição inicial e final
    posicao.x = posicaoInicialAnimacao.x + (posicaoFinalAnimacao.x - posicaoInicialAnimacao.x) * progressoAnimacao;
    posicao.y = posicaoInicialAnimacao.y + (posicaoFinalAnimacao.y - posicaoInicialAnimacao.y) * progressoAnimacao;
}
