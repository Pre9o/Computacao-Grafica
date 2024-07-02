// render.cpp
#include "render.h"


/**
 * @brief Renderiza o menu inicial.
 *
 * @param sidebar A barra lateral do jogo.
 * @param imageManagerMenuInicial O gerenciador de imagens do menu inicial.
 * @param screenWidth A largura da tela.
 * @param screenHeight A altura da tela.
 *
 */
void renderizarMenuInicial(Sidebar sidebar, ImageManager imageManagerMenuInicial, int screenWidth, int screenHeight){
    for(Botao* botao : sidebar.botoesMenuInicial){
            botao->AtualizarPosicaoMeioTela(screenWidth / 2, screenHeight / 2);
            botao->Render();
    }

    for (Bmp* image : imageManagerMenuInicial.images) {
            AtualizarParametros(image, imageManagerMenuInicial.images.size() * 100, Vector2(screenWidth / 2, screenHeight / 2));
            DrawImage(image);
    }
}

/**
 * Função responsável por renderizar o jogo.
 *
 * @param controle Referência para o objeto Controle que contém as informações do jogo.
 * @param firstMove Referência para uma variável booleana que indica se é o primeiro movimento do jogo.
 * @param lastTime Referência para uma variável do tipo clock_t que armazena o tempo do último movimento.
 * @param carregado Referência para uma variável booleana que indica se o jogo foi carregado.
 */
void renderizarJogo(Controle& controle, bool& firstMove, clock_t& lastTime, bool& carregado){
    controle.tabuleiro.setExtremosTabuleiro(Vector2(screenWidth/2 - 300, screenHeight/2 + 400), Vector2(screenWidth/2 + 260, screenHeight/2 + 401), Vector2(screenWidth/2 + 261, screenHeight/2 - 400), Vector2(screenWidth/2 - 300, screenHeight/2 - 400));
        controle.tabuleiro.setTabuleiro();

        controle.tabuleiro.desenhaTabuleiro();

        controle.canhao.setCanhao(controle.tabuleiro);

        controle.setExtremosTabelaDePontos(Vector2(screenWidth/2 - 600, screenHeight/2 + 400), Vector2(screenWidth/2 - 400, screenHeight/2 + 400), Vector2(screenWidth/2 - 400, screenHeight/2 - 400), Vector2(screenWidth/2 - 600, screenHeight/2 - 400));

        for(auto& linha : controle.tabuleiro.matriz_tabuleiro){
            for(Bloco& bloco : linha){
                if(bloco.explosao != NULL) {
                    bloco.explosao->desenha();
                    if(bloco.explosao->tempoRestante == 0){
                        bloco.explosao = NULL;
                    }
                }
                if(bloco.ativo == true){
                    bloco.desenhaBloco();
                }
            }
        }
        for(auto& bola: controle.bolas){
            bola.desenhaBola();
        }

        controle.canhao.desenhaCanhao();
        controle.canhao.desenhaBocaCanhao();
        controle.desenharTabelaDePontos();
        controle.exibirNivelEPontuacao();

        if(controle.jogando){
            if (firstMove) {
                lastTime = clock();
                firstMove = false;
            }

            clock_t now = clock();
            double deltaTime = (double)(now - lastTime) / 1000.0f;

            if(deltaTime > 1.0/60.0f){
                controle.controlaJogo(0.01, &firstMove);
                lastTime = now;
            }
        }

        carregado = true;
}

/**
 * Função responsável por renderizar o menu de pausa.
 *
 * @param sidebar A barra lateral do jogo.
 * @param imageManagerMenuPausa O gerenciador de imagens do menu de pausa.
 * @param imageManagerMenuInicial O gerenciador de imagens do menu inicial.
 * @param screenWidth A largura da tela.
 * @param screenHeight A altura da tela.
*/
void renderizarMenuPausa(Sidebar sidebar, ImageManager imageManagerMenuPausa, ImageManager imageManagerMenuInicial, int screenWidth, int screenHeight){
    for(Botao* botao : sidebar.botoesMenuPausa){
        botao->AtualizarPosicaoMeioTela(screenWidth / 2, screenHeight / 2);
        botao->Render();
    }

    for (Bmp* image : imageManagerMenuInicial.images) {
        AtualizarParametros(image, imageManagerMenuInicial.images.size() * 100, Vector2(screenWidth / 2, screenHeight / 2));
        DrawImage(image);
    }
}
