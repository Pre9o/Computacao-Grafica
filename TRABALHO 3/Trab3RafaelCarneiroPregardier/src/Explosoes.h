// explosoes.h

#ifndef EXPLOSOES_H
#define EXPLOSOES_H

#include "gl_canvas2d.h"
#include <time.h>
#include <vector>

/**
 * @brief The Particula class represents a particle in the explosion.
 * 
 * This class stores the position, velocity, color, and other properties of a particle.
 * It provides methods to update the particle's position, draw it on the screen, and more.
 */
class Particula {
    public:
        /**
         * @brief Constrói um novo objeto Particula.
         * 
         * @param x A coordenada x inicial da partícula.
         * @param y A coordenada y inicial da partícula.
         * @param angulo O ângulo inicial da partícula.
         * @param cor A cor da partícula.
         */
        Particula(float x, float y, float angulo, int cor);

        /**
         * @brief Atualiza a posição da partícula com base no tempo decorrido.
         * 
         * @param deltaTime O tempo decorrido desde a última atualização.
         */
        void atualiza(float deltaTime);

        /**
         * @brief Desenha a partícula na tela.
         */
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

        /**
         * @brief Representa uma explosão.
         * 
         * Esta classe é responsável por representar uma explosão no jogo.
         * Ela contém informações sobre a posição, cor e tempo restante da explosão.
         */
        Explosao(float x, float y, clock_t tempoInicial, int cor);

        /**
         * @brief Desenha a explosão.
         * 
         * Esta função é responsável por desenhar a explosão na tela.
         */
        void desenha();

    private:
        float x, y; // posição da explosão
        int cor; // cor da explosão
        clock_t tempoInicial; // tempo inicial da explosão
        std::vector<Particula> particulas; // partículas da explosão
};

#endif // EXPLOSOES_H