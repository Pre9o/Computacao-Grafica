// Tabuleiro.h

#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <vector>
#include "Vector2.h"
#include "Bloco.h"

/**
 * @class Tabuleiro
 * @brief Classe que representa o tabuleiro do jogo.
 * 
 * A classe Tabuleiro é responsável por representar o tabuleiro do jogo.
 * Ela guarda informações como os cantos do tabuleiro, os blocos ativos no tabuleiro, a matriz que representa o tabuleiro do jogo e uma matriz auxiliar usada para reorganizar o tabuleiro.
 * A classe Tabuleiro também contém métodos para configurar o tabuleiro, verificar o número de blocos ativos no tabuleiro, definir os blocos no tabuleiro com base nos pontos e no nível, desenhar o tabuleiro do jogo, reorganizar o tabuleiro do jogo com base nos pontos e no nível e definir os cantos do tabuleiro do jogo.
 */
class Tabuleiro {
    public:
        std::vector<Vector2> extremos_tabuleiro; /**< Os cantos do tabuleiro do jogo. */
        std::vector<Bloco> blocos_ativos; /**< Os blocos ativos no tabuleiro. */

        Bloco matriz_tabuleiro[10][7]; /**< A matriz que representa o tabuleiro do jogo. */
        Bloco matriz_tabuleiro_aux[10][7]; /**< Uma matriz auxiliar usada para reorganizar o tabuleiro. */

        /**
         * @brief Construtor padrão para Tabuleiro.
         */
        Tabuleiro();

        /**
         * @brief Configura o tabuleiro do jogo.
         */
        void setTabuleiro();

        /**
         * @brief Verifica o número de blocos ativos no tabuleiro.
         * @return O número de blocos ativos.
         */
        int verificaBlocosAtivos();

        /**
         * @brief Define os blocos no tabuleiro com base nos pontos e no nível.
         * @param pontos Os pontos atuais.
         * @param blocosIniciaisMaximosDoNivel O número máximo de blocos iniciais para o nível atual.
         * @param chanceDeSerAtivo A chance de um bloco ser ativo.
         */
        void definirBlocos(int pontos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo);

        /**
         * @brief Desenha o tabuleiro do jogo.
         */
        void desenhaTabuleiro();

        /**
         * @brief Reorganiza o tabuleiro do jogo com base nos pontos e no nível.
         * @param pontos Os pontos atuais.
         * @param blocosIniciaisMaximosDoNivel O número máximo de blocos iniciais para o nível atual.
         */
        void reorganizaTabuleiro(int pontos, int blocosIniciaisMaximosDoNivel);

        /**
         * @brief Define os cantos do tabuleiro do jogo.
         * @param p1 O primeiro canto.
         * @param p2 O segundo canto.
         * @param p3 O terceiro canto.
         * @param p4 O quarto canto.
         */
        void setExtremosTabuleiro(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4);
};

#endif // TABULEIRO_H