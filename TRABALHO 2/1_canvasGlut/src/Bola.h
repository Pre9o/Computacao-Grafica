// Bola.h

#ifndef BOLA_H
#define BOLA_H

#include "Vector2.h"
#include "Canhao.h"
#include "gl_canvas2d.h"

class Bola {
    public:
        Vector2 posicao;
        int velocidade;
        Vector2 direcao;
        float raio;
        double atrasoInicial;

        bool operator==(const Bola& outra) const;
        Bola();
        void setBola(Canhao& canhao, double atrasoInicial);
        void desenhaBola();
        void moverBola(double deltaTime);
};

#endif // BOLA_H