// Tabuleiro.h

#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <vector>
#include "Vector2.h"
#include "Bloco.h"

class Tabuleiro {
    public:
        std::vector<Vector2> extremos_tabuleiro;
        std::vector<Bloco> blocos_ativos;

        Bloco matriz_tabuleiro[10][7];
        Bloco matriz_tabuleiro_aux[10][7];

        Tabuleiro();

        void setTabuleiro();
        int verificaBlocosAtivos();
        void definirBlocos(int pontos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo);
        void desenhaTabuleiro();
        void reorganizaTabuleiro(int pontos, int blocosIniciaisMaximosDoNivel);
        void setExtremosTabuleiro(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4);
};

#endif // TABULEIRO_H