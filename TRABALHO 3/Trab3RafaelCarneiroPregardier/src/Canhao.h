// Canhao.h

#ifndef CANHAO_H
#define CANHAO_H

#include "Vector2.h"
#include "Tabuleiro.h"
#include <stdio.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

/**
 * @class Canhao
 * @brief Classe que representa um canhão.
 * 
 * A classe Canhao é responsável por representar o canhão do jogo.
 * Ela guarda informações como a posição do mouse, a origem do canhão, o vetor de direção do canhão, o início da linha que representa o canhão,
 * o tabuleiro do jogo e o ângulo do círculo do mouse.
 
 */
class Canhao {
    public:
        Vector2 mouse_pos; ///< A posição do mouse.
        Vector2 origem; ///< O ponto de origem do canhão.
        Vector2 vetor_direcao; ///< O vetor de direção do canhão.

        Vector2 inicio_linha; ///< O ponto inicial da linha.
        Vector2 inicio_linha_oposta; ///< O ponto inicial da linha oposta.

        Tabuleiro tabuleiro; ///< O tabuleiro do jogo.

        float angulo_circulo_mouse; ///< O ângulo do círculo do mouse.

        /**
         * @brief Construtor padrão para a classe Canhao.
         */
        Canhao();

        /**
         * @brief Define as propriedades do objeto Canhao.
         * @param tabuleiro O tabuleiro do jogo a ser definido.
         */
        void setCanhao(Tabuleiro& tabuleiro);

        /**
         * @brief Define a posição do mouse.
         * @param pos A posição do mouse a ser definida.
         */
        void setMousePos(Vector2 pos);

        /**
         * @brief Desenha o canhão.
         */
        void desenhaCanhao();

        /**
         * @brief Desenha a boca do canhão.
         */
        void desenhaBocaCanhao();
};

#endif // CANHAO_H