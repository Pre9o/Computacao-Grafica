#include "Tabuleiro.h"

/**
 * @brief Construtor da classe Tabuleiro.
 * 
 * Define os extremos do tabuleiro e chama a função setTabuleiro.
 * 
 * @param None
 * @return None
 */
Tabuleiro::Tabuleiro(){
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 + 400));
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 260, screenHeight/2 + 401));
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 + 261, screenHeight/2 - 400));
    this->extremos_tabuleiro.push_back(Vector2(screenWidth/2 - 300, screenHeight/2 - 400));

    this->setTabuleiro();
}

/**
 * @brief Define a posição dos blocos no tabuleiro.
 * 
 * Esta função percorre a matriz do tabuleiro e define a posição dos blocos com base nos extremos do tabuleiro.
 * Cada bloco é definido por quatro pontos: p1, p2, p3 e p4.
 * 
 * @param None
 * @return None
 */
void Tabuleiro::setTabuleiro() {
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 7; j++){
            Vector2 p1 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80));
            Vector2 p2 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80));
            Vector2 p3 = Vector2(extremos_tabuleiro[0].x + (j * 80) + 80, extremos_tabuleiro[3].y + (i * 80) + 80);
            Vector2 p4 = Vector2(extremos_tabuleiro[0].x + (j * 80), extremos_tabuleiro[3].y + (i * 80) + 80);
            matriz_tabuleiro[i][j].setExtremosBloco({p1, p2, p3, p4});
        }
    }
}

/**
 * @brief Verifica se há blocos ativos no tabuleiro.
 * 
 * Esta função percorre a matriz do tabuleiro e verifica se há blocos ativos.
 * 
 * @param None
 * @return O número de blocos ativos no tabuleiro.
 */
int Tabuleiro::verificaBlocosAtivos(){
    int blocos_ativos = 0;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 7; j++){
            if(matriz_tabuleiro[i][j].ativo == true){
                blocos_ativos++;
            }
        }
    }
    return blocos_ativos;
}

/**
 * @brief Define os blocos no tabuleiro.
 * 
 * Esta função percorre a matriz do tabuleiro e define os blocos com base nos pontos, no número de blocos ativos e na chance de ser ativo.
 * 
 * @param pontos Pontuação do jogador.
 * @param blocosIniciaisMaximosDoNivel Número máximo de blocos iniciais do nível.
 * @param chanceDeSerAtivo Chance de um bloco ser ativo.
 * @return None
 */
void Tabuleiro::definirBlocos(int pontos, int blocosIniciaisMaximosDoNivel, double chanceDeSerAtivo){
    for(int i = 0; i < 10; i++){
        chanceDeSerAtivo = 7;
        for(int j = 0; j < 7; j++){
            if(matriz_tabuleiro[i][j].ativo == false){
                matriz_tabuleiro[i][j].setBloco(i, pontos, blocos_ativos, blocosIniciaisMaximosDoNivel, chanceDeSerAtivo);
            }
            if(matriz_tabuleiro[i][j].ativo == false){
                chanceDeSerAtivo += 10;
            }
        }
    }
}

/**
 * @brief Desenha o tabuleiro na tela.
 * 
 * Esta função desenha o tabuleiro na tela, percorrendo a matriz do tabuleiro e desenhando cada bloco.
 * 
 * @param None
 * @return None
 */
void Tabuleiro::desenhaTabuleiro() {
    for(size_t i = 0; i < extremos_tabuleiro.size(); i++){
        CV::color(1, 1, 1);
        CV::line(extremos_tabuleiro[i], extremos_tabuleiro[(i + 1) % 4]);
    }
}

/**
 * @brief Verifica se houve colisão entre a bola e o tabuleiro.
 * 
 * Esta função verifica se houve colisão entre a bola e o tabuleiro, percorrendo a matriz do tabuleiro e verificando se houve colisão entre a bola e cada bloco.
 * 
 * @param bola A bola do jogo.
 * @return true se houve colisão, false caso contrário.
 */
void Tabuleiro::reorganizaTabuleiro(int pontos, int blocosIniciaisMaximosDoNivel){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 7; j++){
            matriz_tabuleiro[i][j] = matriz_tabuleiro[i+1][j];
        }
    }
    for(int j = 0; j < 7; j++){
        matriz_tabuleiro[9][j].setBloco(9, pontos, blocos_ativos, blocosIniciaisMaximosDoNivel, 0);
    }

    definirBlocos(pontos, blocosIniciaisMaximosDoNivel, 7);

}

/**
 * @brief Verifica se houve colisão entre a bola e o tabuleiro.
 * 
 * Esta função verifica se houve colisão entre a bola e o tabuleiro, percorrendo a matriz do tabuleiro e verificando se houve colisão entre a bola e cada bloco.
 * 
 * @param bola A bola do jogo.
 * @return true se houve colisão, false caso contrário.
 */
void Tabuleiro::setExtremosTabuleiro(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4) {
    extremos_tabuleiro[0] = p1;
    extremos_tabuleiro[1] = p2;
    extremos_tabuleiro[2] = p3;
    extremos_tabuleiro[3] = p4;
}