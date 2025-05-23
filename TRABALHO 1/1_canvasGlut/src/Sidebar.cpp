#include "Sidebar.h"

void Sidebar::ConstruirBotoes(ImageManager &imageManager){
    // Se a barra lateral ainda não tem botões, cria os botões
    if(botoes.size() == 0){
        // Cria um botão para aplicar a operação de flip vertical à imagem selecionada
        botoes.push_back(new Botao(1075, -475, 150, 50, "Flip Vertical", 1, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                imageManager.selectedImage->flipVertical();
            }
        }));
        // Cria um botão para aplicar a operação de flip horizontal à imagem selecionada
        botoes.push_back(new Botao(1250, -475, 150, 50, "Flip Horizontal", 1, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                imageManager.selectedImage->flipHorizontal();
            }
        }));
        // Cria um botão para aplicar a operação de grayscale à imagem selecionada
        botoes.push_back(new Botao(1250, -175, 150, 50, "Grayscale", 0.5, 0.5, 0.5, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if (imageManager.fourthImage != NULL) {
                    delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->storeOriginalImage();
                imageManager.fourthImage->image_Gray();
            }
        }));
        // Cria um botão para aplicar a operação de red à imagem selecionada
        botoes.push_back(new Botao(1250, -250, 150, 50, "Red", 1, 0, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                    delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->storeOriginalImage();
                imageManager.fourthImage->image_R();
            }
        }));
        // Cria um botão para aplicar a operação de green à imagem selecionada
        botoes.push_back(new Botao(1250, -325, 150, 50, "Green", 0, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                    delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->storeOriginalImage();
                imageManager.fourthImage->image_G();
            }
        }));
        // Cria um botão para aplicar a operação de blue à imagem selecionada
        botoes.push_back(new Botao(1250, -400, 150, 50, "Blue", 0, 0, 1, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                    delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->storeOriginalImage();
                imageManager.fourthImage->image_B();
            }
        }));
        // Cria botões para carregar imagens
        botoes.push_back(new Botao(900, -400, 150, 50, "Load Image 3", 0.75, 1, 1, [&imageManager](){
            LoadImages(imageManager.images, "./1_canvasGlut/images/img2.bmp");
        }));
        botoes.push_back(new Botao(900, -325, 150, 50, "Load Image 2", 1, 0.75, 1, [&imageManager](){
            LoadImages(imageManager.images, "./1_canvasGlut/images/gato.bmp");
        }));
        botoes.push_back(new Botao(900, -250, 150, 50, "Load Image 1", 1, 1, 0.75, [&imageManager](){
            LoadImages(imageManager.images, "./1_canvasGlut/images/Teste.bmp");
        }));
        // Cria botões para visualizar o histograma da imagem selecionada
        botoes.push_back(new Botao(1075, -250, 150, 50, "Histograma Red", 1, 0, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                !imageManager.histograma ? imageManager.histograma = true : imageManager.histograma = false;
                imageManager.OpcaoHistograma = 1;
            }
        }));
        botoes.push_back(new Botao(1075, -325, 150, 50, "Histograma Green", 0, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                !imageManager.histograma ? imageManager.histograma = true : imageManager.histograma = false;
                imageManager.OpcaoHistograma = 2;
            }
        }));
        botoes.push_back(new Botao(1075, -400, 150, 50, "Histograma Blue", 0, 0, 1, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                !imageManager.histograma ? imageManager.histograma = true : imageManager.histograma = false;
                imageManager.OpcaoHistograma = 3;
            }
        }));
        botoes.push_back(new Botao(1075, -175, 150, 50, "Histograma Gray", 0.5, 0.5, 0.5, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                !imageManager.histograma ? imageManager.histograma = true : imageManager.histograma = false;
                imageManager.OpcaoHistograma = 4;
            }
        }));
        botoes.push_back(new Botao(900, -175, 150, 50, "Normalizacao", 1, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                    delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage = NormalizarHistograma(imageManager.fourthImage, imageManager.OpcaoHistograma);
            }
        }));
    }
}