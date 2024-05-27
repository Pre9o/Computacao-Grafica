#pragma once

#include <string>
#include "gl_canvas2d.h"
#include "Vector2.h"
#include <vector>
#include <time.h>
#include <chrono>
#include "windows.h"
#include "mmsystem.h"
#include "Bola.h"
#include "Bloco.h"
#include "Canhao.h"
#include "Tabuleiro.h"

/**
 * @brief Classe responsável por controlar o jogo e suas funcionalidades.
 * 
 * A classe Controle é responsável por controlar o jogo e suas funcionalidades. Ela guarda informações como o nível atual, a pontuação do jogador, os pontos iniciais de cada bloco, o número máximo de blocos no nível atual, os pontos extremos da tabela de pontos, as bolas presentes no jogo, o tabuleiro do jogo, o canhão do jogo, o nome de usuário do jogador e uma flag que indica se o jogo está em andamento.
 * A classe Controle também contém métodos para definir o nome de usuário do jogador, exibir a tela de modificação do nome de usuário, definir os pontos extremos da tabela de pontos, desenhar a tabela de pontos, adicionar bolas ao jogo, definir o tabuleiro do jogo, definir o canhão do jogo, testar a colisão entre o tabuleiro e uma bola, testar a colisão entre os blocos do tabuleiro e uma bola, executar uma jogada no jogo, controlar o fluxo do jogo, gerar um novo nível do jogo e exibir o nível atual e a pontuação do jogador.
 * 
 */
class Controle{
    public:
    int nivel; /**< Nível atual do jogo. */
    int pontosIniciaisDoBloco; /**< Pontuação inicial de cada bloco. */
    int blocosIniciaisMaximosDoNivel; /**< Número máximo de blocos no nível atual. */
    int pontuacao; /**< Pontuação atual do jogador. */

    std::vector<Vector2> extremosTabelaDePontos; /**< Vetor contendo os pontos extremos da tabela de pontos. */
    std::vector<Bola> bolas; /**< Vetor contendo as bolas presentes no jogo. */
    Tabuleiro tabuleiro; /**< Objeto que representa o tabuleiro do jogo. */
    Canhao canhao; /**< Objeto que representa o canhão do jogo. */

    std::string username; /**< Nome de usuário do jogador. */

    bool jogando; /**< Flag que indica se o jogo está em andamento. */

    /**
     * @brief Construtor da classe Controle.
     */
    Controle();
    
    /**
     * @brief Define o nome de usuário do jogador.
     * @param username_ O nome de usuário a ser definido.
     */
    void setUsername(std::string username_);

    /**
     * @brief Exibe a tela de modificação do nome de usuário.
     * @param username_ O nome de usuário atual.
     */
    void telaDeModificacaoDeUsername(std::string username_);

    /**
     * @brief Define os pontos extremos da tabela de pontos.
     * @param p1 O primeiro ponto extremo.
     * @param p2 O segundo ponto extremo.
     * @param p3 O terceiro ponto extremo.
     * @param p4 O quarto ponto extremo.
     */
    void setExtremosTabelaDePontos(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4);

    /**
     * @brief Desenha a tabela de pontos na tela.
     */
    void desenharTabelaDePontos();

    /**
     * @brief Adiciona bolas ao jogo.
     * @param num_bolas O número de bolas a serem adicionadas.
     */
    void adicionarBolas(int num_bolas);

    /**
     * @brief Define o tabuleiro do jogo.
     * @param tabuleiro O tabuleiro a ser definido.
     */
    void setTabuleiro(Tabuleiro tabuleiro);

    /**
     * @brief Define o canhão do jogo.
     * @param canhao O canhão a ser definido.
     */
    void setCanhao(Canhao canhao);

    /**
     * @brief Testa a colisão entre o tabuleiro e uma bola.
     * @param tabuleiro O tabuleiro do jogo.
     * @param bola A bola a ser testada.
     * @return true se houver colisão, false caso contrário.
     */
    bool testaColisaoTabuleiro(Tabuleiro& tabuleiro, Bola& bola);

    /**
     * @brief Testa a colisão entre os blocos do tabuleiro e uma bola.
     * @param tabuleiro O tabuleiro do jogo.
     * @param bola A bola a ser testada.
     * @return true se houver colisão, false caso contrário.
     */
    bool testaColisaoBlocos(Tabuleiro& tabuleiro, Bola& bola);

    /**
     * @brief Executa uma jogada no jogo.
     * @param deltaTime O tempo decorrido desde a última atualização.
     */
    void executaJogada(double deltaTime);

    /**
     * @brief Controla o fluxo do jogo.
     * @param deltaTime O tempo decorrido desde a última atualização.
     * @param firstMove Ponteiro para uma flag que indica se é o primeiro movimento do jogo.
     */
    void controlaJogo(double deltaTime, bool* firstMove);

    /**
     * @brief Gera um novo nível do jogo.
     */
    void gerarNivel();

    /**
     * @brief Exibe o nível atual e a pontuação do jogador.
     */
    void exibirNivelEPontuacao();
};