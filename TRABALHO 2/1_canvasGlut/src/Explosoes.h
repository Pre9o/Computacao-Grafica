// explosoes.h

#ifndef EXPLOSOES_H
#define EXPLOSOES_H

#include "gl_canvas2d.h"
#include <time.h>
#include <vector>

class Particula {
    public:
        Particula(float x, float y, float angulo, int cor);
        void atualiza(float deltaTime);
        void desenha();

    private:
        float x, y;
        float velocidadeX, velocidadeY;
        int cor;
        static constexpr float velocidadeInicial = 10.0f;
        static constexpr float atrito = 0.1f;
};

class Explosao {
    public:
        clock_t tempoRestante; // tempo restante para a explosão

        Explosao(float x, float y, clock_t tempoInicial, int cor);
        void desenha();

    private:
        float x, y;
        int cor;
        clock_t tempoInicial; // tempo inicial da explosão
        std::vector<Particula> particulas;
};

#endif // EXPLOSOES_H