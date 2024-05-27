#include "Controle.h"

/**
 * @brief Construtor padrão para a classe Controle.
 */
Controle::Controle(){
    nivel = 1;
    pontosIniciaisDoBloco = 0;
    jogando = false;
    pontuacao = 0;

    this->extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 600, screenHeight/2 + 400));
    this->extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 400, screenHeight/2 + 400));
    this->extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 400, screenHeight/2 - 400));
    this->extremosTabelaDePontos.push_back(Vector2(screenWidth/2 - 600, screenHeight/2 - 400));

}

/**
 * Define o username do jogador.
 * 
 * @param username_ O username do jogador.
*/
void Controle::setUsername(std::string username_){
    this->username = username_;
}

/**
 * Desenha a tela de modificação de username.
*/
void Controle::telaDeModificacaoDeUsername(std::string username_){
    CV::color(1, 1, 1);
    CV::text(screenWidth/2 - CV::getTextWidth(username_.c_str(), GLUT_BITMAP_HELVETICA_18)/2, screenHeight/2, "Digite seu username: ", username_.c_str(), GLUT_BITMAP_HELVETICA_18);
}

/**
 * Define os extremos da tabela de pontos.
 * 
 * @param p1 O primeiro ponto.
 * @param p2 O segundo ponto.
 * @param p3 O terceiro ponto.
 * @param p4 O quarto ponto.
 */
void Controle::setExtremosTabelaDePontos(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4){
    extremosTabelaDePontos[0] = p1;
    extremosTabelaDePontos[1] = p2;
    extremosTabelaDePontos[2] = p3;
    extremosTabelaDePontos[3] = p4;
}

/**
 * Desenha a tabela de pontos na tela.
 */
void Controle::desenharTabelaDePontos(){
    for(size_t i = 0; i < extremosTabelaDePontos.size(); i++){
        CV::color(1, 1, 1);
        CV::line(extremosTabelaDePontos[i], extremosTabelaDePontos[(i + 1) % 4]);
    }
}

/**
 * Adiciona bolas ao vetor de bolas.
 * 
 * @param num_bolas O número de bolas a serem adicionadas.
 */
void Controle::adicionarBolas(int num_bolas){
    for(int i = 0; i < num_bolas; i++){
        Bola bola;
        bolas.push_back(bola);
    }
}

/**
 * Define o tabuleiro.
 * 
 * @param tabuleiro 
 */
void Controle::setTabuleiro(Tabuleiro tabuleiro){
    this->tabuleiro = tabuleiro;
}

/**
 * Define o canhão.
 * 
 * @param canhao 
 */
void Controle::setCanhao(Canhao canhao){
    this->canhao = canhao;
}

/**
 * Verifica se houve colisão entre a bola e o tabuleiro.
 * 
 * @param tabuleiro O tabuleiro do jogo.
 * @param bola A bola do jogo.
 * @return true se houve colisão, false caso contrário.
 */
bool Controle::testaColisaoTabuleiro(Tabuleiro& tabuleiro, Bola& bola){
    if(bola.posicao.x - bola.raio < tabuleiro.extremos_tabuleiro[0].x || bola.posicao.x + bola.raio > tabuleiro.extremos_tabuleiro[2].x){
        PlaySound(TEXT("./1_canvasGlut/audios/TUC.wav"), NULL, SND_ASYNC);
        bola.direcao.x *= -1;
        return true;
    }
    if(bola.posicao.y + bola.raio > tabuleiro.extremos_tabuleiro[0].y){
        PlaySound(TEXT("./1_canvasGlut/audios/TUC.wav"), NULL, SND_ASYNC);
        bola.direcao.y *= -1;
        return true;
    }
    if(bola.posicao.y - bola.raio < tabuleiro.extremos_tabuleiro[2].y){
        auto it = std::find(bolas.begin(), bolas.end(), bola); // encontra a bola no vetor
        if(it != bolas.end()){
            bolas.erase(it);
        }
    }

    return false;
}

/**
 * Verifica se houve colisão entre a bola e os blocos do tabuleiro.
 * 
 * @param tabuleiro O tabuleiro do jogo.
 * @param bola A bola do jogo.
 * @return true se houve colisão, false caso contrário.
 */
bool Controle::testaColisaoBlocos(Tabuleiro& tabuleiro, Bola& bola){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 7; j++){
            if(tabuleiro.matriz_tabuleiro[i][j].ativo == true){
                Vector2 posicao_bloco = tabuleiro.matriz_tabuleiro[i][j].getPosicao();
                Vector2 tamanho_bloco = tabuleiro.matriz_tabuleiro[i][j].getTamanho();

                float distancia_x = 0;
                float distancia_y = 0;

                if(bola.posicao.x < posicao_bloco.x){
                    distancia_x = posicao_bloco.x + tamanho_bloco.x/2 - bola.posicao.x - bola.raio;
                }
                else if(bola.posicao.x >= posicao_bloco.x){
                    distancia_x = bola.posicao.x + bola.raio - (posicao_bloco.x + tamanho_bloco.x/2);
                }

                if(bola.posicao.y < posicao_bloco.y){
                    distancia_y = posicao_bloco.y + tamanho_bloco.y/2 - bola.posicao.y - bola.raio;
                }
                else if(bola.posicao.y >= posicao_bloco.y){
                    distancia_y = bola.posicao.y + bola.raio - (posicao_bloco.y + tamanho_bloco.y/2);
                }

                float dx = distancia_x - tamanho_bloco.x/2;
                float dy = distancia_y - tamanho_bloco.y/2;


                if(dx <= bola.raio && dy <= bola.raio){
                    if(distancia_y > distancia_x){
                        if(dy > 0){
                            PlaySound(TEXT("./1_canvasGlut/audios/TUC.wav"), NULL, SND_ASYNC);
                            bola.direcao.y *= -1;
                        }
                        else{
                            PlaySound(TEXT("./1_canvasGlut/audios/TUC.wav"), NULL, SND_ASYNC);
                            bola.direcao.y *= -1;
                        }
                    }
                    else{
                        if(dx > 0){
                            PlaySound(TEXT("./1_canvasGlut/audios/TUC.wav"), NULL, SND_ASYNC);
                            bola.direcao.x *= -1;
                        }
                        else{
                            PlaySound(TEXT("./1_canvasGlut/audios/TUC.wav"), NULL, SND_ASYNC);
                            bola.direcao.x *= -1;
                        }
                    }
                    tabuleiro.matriz_tabuleiro[i][j].diminuirPontos(this->tabuleiro.blocos_ativos, this->pontuacao);
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * Executa a jogada.
 * 
 * @param deltaTime O tempo que passou desde o último frame.
 */
void Controle::executaJogada(double deltaTime){
    Bola bolaAntesColisao;
    for(auto& bola: bolas){
        if(bola.atrasoInicial > 0){
            bola.atrasoInicial -= deltaTime; // decrementa o atraso inicial
        } else {
            bolaAntesColisao = bola;
            bola.moverBola(deltaTime);
            if(testaColisaoTabuleiro(tabuleiro, bola) || testaColisaoBlocos(tabuleiro, bola)){
                bola.posicao = bolaAntesColisao.posicao;
            }
        }
    }
}

/**
 * Controla o jogo.
 * 
 * @param deltaTime O tempo que passou desde o último frame.
 * @param firstMove Indica se é o primeiro movimento do jogador.
 */
void Controle::controlaJogo(double deltaTime, bool* firstMove){
    this->executaJogada(deltaTime);

    if(this->bolas.size() == 0){
        this->jogando = false;
        *firstMove = true;
    }

    if(this->jogando == false){
        if(this->tabuleiro.verificaBlocosAtivos() == 0){
            this->nivel++;
            this->gerarNivel();
        }
        else{
            this->blocosIniciaisMaximosDoNivel++;
            this->tabuleiro.reorganizaTabuleiro(this->pontosIniciaisDoBloco, this->blocosIniciaisMaximosDoNivel);

            this->adicionarBolas(this->nivel);
        }
    }
}

/**
 * Desenha o jogo na tela.
 */
void Controle::gerarNivel(){
    this->blocosIniciaisMaximosDoNivel = 1 + 1 * (this->nivel / 10);
    if(nivel % 10 != 0){
        this->pontosIniciaisDoBloco = nivel;
        this->tabuleiro.definirBlocos(nivel, blocosIniciaisMaximosDoNivel, 7);
        this->adicionarBolas(nivel);
    }
    else{
        this->pontosIniciaisDoBloco = nivel * 2;
        this->tabuleiro.definirBlocos(nivel / 10 + 1, blocosIniciaisMaximosDoNivel, 7);
        this->adicionarBolas(nivel);
    }
}

/**
 * Exibe o nível e a pontuação na tela.
 */
void Controle::exibirNivelEPontuacao(){
    CV::color(1, 1, 1);
    CV::text(this->extremosTabelaDePontos[0].x - (CV::getTextWidth("Nivel: ", GLUT_BITMAP_HELVETICA_18)/2) + (this->extremosTabelaDePontos[1].y/10),
                this->extremosTabelaDePontos[0].y - (this->extremosTabelaDePontos[2].x/10) - (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
                "Nivel: ", nivel, GLUT_BITMAP_HELVETICA_18);
    CV::text(this->extremosTabelaDePontos[0].x - (CV::getTextWidth("Pontuacao: ", GLUT_BITMAP_HELVETICA_18)/2) + (this->extremosTabelaDePontos[1].y/10),
                this->extremosTabelaDePontos[0].y - (this->extremosTabelaDePontos[2].x/7) - (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
                "Pontuacao: ", pontuacao, GLUT_BITMAP_HELVETICA_18);
    CV::color(0.98823, 0.5098, 0.20392);
    CV::text(this->extremosTabelaDePontos[0].x - (CV::getTextWidth(username.c_str(), GLUT_BITMAP_HELVETICA_18)/2) + (this->extremosTabelaDePontos[1].y/10),
                this->extremosTabelaDePontos[0].y - (this->extremosTabelaDePontos[2].x/5) - (CV::getBitmapHeight(GLUT_BITMAP_HELVETICA_18)/2),
                username.c_str(), GLUT_BITMAP_HELVETICA_18);
}