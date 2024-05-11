/*
A classe Sidebar representa a barra lateral da interface do usuário que contém botões para manipular as imagens. Cada botão é representado por uma instância da classe Botao e tem uma ação associada que é executada quando o botão é clicado.

Métodos:
- ConstruirBotoes(ImageManager &imageManager): Constrói os botões na barra lateral. Cada botão tem uma posição, tamanho, cor e ação associada. As ações dos botões incluem carregar imagens, aplicar várias operações às imagens (como flip vertical/horizontal, grayscale, red, green, blue) e visualizar o histograma da imagem selecionada.
*/

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Botao.h"
#include "Image.h"
#include "ImagesFunctions.h"
#include "ImageManager.h"
#include <time.h>

class Sidebar {
public:
    // Vetor para armazenar os botões na barra lateral
    std::vector<Botao*> botoes;

    // Método para construir os botões na barra lateral
    void ConstruirBotoesMenuInicial(int *opcaoMenu, clock_t *intervalo_tempo_inicio){
        // Se a barra lateral ainda não tem botões, cria os botões
        if(botoes.size() == 0){
            botoes.push_back(new Botao(-100, 100, 200, 50, "Play", 1, 1, 1, [opcaoMenu, intervalo_tempo_inicio](){
                printf("Botão Play clicado\n");
                *opcaoMenu = 1;
                *intervalo_tempo_inicio = clock();
            }));
            botoes.push_back(new Botao(-100, 0, 200, 50, "Settings", 1, 1, 1, [](){
                printf("Botão Settings clicado\n");
            }));
            botoes.push_back(new Botao(-100, -100, 200, 50, "Quit", 1, 1, 1, [](){
                printf("Botão Quit clicado\n");
                exit(0);
            }));
        }
    }
};
