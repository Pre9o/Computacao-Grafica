// Bloco.h

#ifndef BLOCO_H
#define BLOCO_H

#include <vector>
#include "Vector2.h"
#include "Explosoes.h"
#include <algorithm>

/**
 * @class Bloco
 * @brief Representa um bloco no jogo.
 * 
 * A classe Bloco guarda informações sobre um bloco, como seu tamanho, posição, cor, e pontos.
 * Também dispõe de m[etodos para ingeragir e manipular os blocos.
 */
class Bloco {
    public:
        Vector2 tamanho;
        Explosao* explosao = NULL;

        std::vector<Vector2> extremos_bloco;
        int cor;
        bool ativo;
        int pontos;

        Bloco();
        bool operator==(const Bloco& outro) const;
        void setExtremosBloco(std::vector<Vector2> extremos);
        void setBloco(int i, int pontos, std::vector<Bloco>& blocos_ativos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo);
        bool definirBlocoAtivo(int i, std::vector<Bloco>& blocos_ativos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo);
        Vector2 getPosicao();
        Vector2 getTamanho();
        void diminuirPontos(std::vector<Bloco>& blocos_ativos, int& pontuacao);
        void desenhaBloco();
};

#endif // BLOCO_H