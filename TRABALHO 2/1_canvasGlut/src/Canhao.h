// Canhao.h

#ifndef CANHAO_H
#define CANHAO_H

#include "Vector2.h"
#include "Tabuleiro.h"
#include <stdio.h>
#include <stdlib.h>

#define M_PI 3.14159265358979323846

class Canhao {
    public:
        Vector2 mouse_pos;
        Vector2 origem;
        Vector2 vetor_direcao;

        Vector2 inicio_linha;
        Vector2 inicio_linha_oposta;

        Tabuleiro tabuleiro;

        float angulo_circulo_mouse;

        Canhao();
        void setCanhao(Tabuleiro& tabuleiro);
        void setMousePos(Vector2 pos);
        void desenhaCanhao();
        void desenhaBocaCanhao();
};

#endif // CANHAO_H