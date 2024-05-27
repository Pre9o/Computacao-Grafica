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

class Controle{
    public:
    int nivel;
    int pontosIniciaisDoBloco;
    int blocosIniciaisMaximosDoNivel;
    int pontuacao;

    std::vector<Vector2> extremosTabelaDePontos;
    std::vector<Bola> bolas;
    Tabuleiro tabuleiro;
    Canhao canhao;

    std::string username;

    bool jogando;

    Controle();
    
    void setUsername(std::string username_);
    void telaDeModificacaoDeUsername(std::string username_);
    void setExtremosTabelaDePontos(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4);
    void desenharTabelaDePontos();
    void adicionarBolas(int num_bolas);
    void setTabuleiro(Tabuleiro tabuleiro);
    void setCanhao(Canhao canhao);
    bool testaColisaoTabuleiro(Tabuleiro& tabuleiro, Bola& bola);
    bool testaColisaoBlocos(Tabuleiro& tabuleiro, Bola& bola);
    void executaJogada(double deltaTime);
    void controlaJogo(double deltaTime, bool* firstMove);
    void gerarNivel();
    void exibirNivelEPontuacao();
};