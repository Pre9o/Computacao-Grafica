#include "Explosoes.h"

Particula::Particula(float x, float y, float angulo, int cor) {
            this->x = x;
            this->y = y;
            this->velocidadeX = cos(angulo) * velocidadeInicial;
            this->velocidadeY = sin(angulo) * velocidadeInicial;
            this->cor = cor;
    }

/**
 * Atualiza a posição da partícula com base no tempo decorrido.
 * 
 * @param deltaTime O tempo decorrido desde a última atualização.
 */
void Particula::atualiza(float deltaTime) {
            x += velocidadeX * deltaTime;
            y += velocidadeY * deltaTime;
            velocidadeX *= 1 - atrito * deltaTime;
            velocidadeY *= 1 - atrito * deltaTime;
}

/**
 * Desenha a partícula na tela.
*/
void Particula::desenha() {
            CV::color(cor);
            CV::point(x, y);
}

/**
 * @brief Construtor da classe Explosao.
 * 
 * @param x A posição x da explosão.
 * @param y A posição y da explosão.
 * @param tempoInicial O tempo inicial da explosão.
 * @param cor A cor da explosão.
 */
Explosao::Explosao(float x, float y, clock_t tempoInicial, int cor) {
    this->x = x;
    this->y = y;
    this->tempoInicial = tempoInicial; // tempo inicial da explosão
    this->tempoRestante = clock(); // tempo restante para a explosão
    this->cor = cor;

    srand(time(0)); // inicializa a semente do gerador de números aleatórios
    for(int i = 0; i < 100; i++) {
        float angulo = (rand() % 360) * 3.14159 / 180.0; // gera um ângulo aleatório
        particulas.push_back(Particula(x, y, angulo, cor));
    }
}

/**
 * Atualiza a explosão.
 * 
 */
void Explosao::desenha() {
    if (double(tempoRestante - tempoInicial) / 1000.0f < 1.5) {
        for(int i = 0; i < 100; i++) {
            for(auto& particula : particulas) {
                particula.atualiza(double(tempoRestante - tempoInicial) / 1000.0f);
                particula.desenha();
            }
        }
        tempoRestante = clock();
    }
    else{
        tempoRestante = 0;
    }
}

