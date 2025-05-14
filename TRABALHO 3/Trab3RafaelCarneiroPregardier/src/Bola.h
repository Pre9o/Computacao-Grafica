// Bola.h

#ifndef BOLA_H
#define BOLA_H

#include "Vector2.h"
#include "Canhao.h"
#include "gl_canvas2d.h"

/**
 * @class Bola
 * @brief Representa uma bola no jogo.
 * 
 * A classe bola é responsável por representar uma bola no jogo. Ela contém atributos como a posição, velocidade, direção e raio da bola.
 * Também contém métodos para desenhar e mover a bola na tela.
 */
class Bola {
    public:
        Vector2 posicao;
        Vector2 direcao;
        Vector2 posicaoInicialAnimacao;
        Vector2 posicaoFinalAnimacao;
        int velocidade;
        float progressoAnimacao;
        float raio;
        double atrasoInicial;
        bool emAnimacao;

        bool operator==(const Bola& outra) const;
        Bola();
        void setBola(Canhao& canhao, double atrasoInicial);
        void desenhaBola();
        void moverBola(double deltaTime);
        void iniciarAnimacaoRetorno(const Vector2& destino);
        void atualizarAnimacao(float deltaTime);
        bool estaEmAnimacao() const { return emAnimacao; }
};

#endif // BOLA_H