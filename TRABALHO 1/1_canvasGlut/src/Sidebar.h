#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "Botao.h"
#include "Image.h"
#include "ImagesFunctions.h"
#include "ImageManager.h"

class Sidebar {
public:
    std::vector<Botao*> botoes;

    void ConstruirBotoes(ImageManager &imageManager){
    if(botoes.size() == 0){
        botoes.push_back(new Botao(1075, -475, 150, 50, "Flip Vertical", 1, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                imageManager.selectedImage->flipVertical();
            }
        }));
        botoes.push_back(new Botao(1250, -475, 150, 50, "Flip Horizontal", 1, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                imageManager.selectedImage->flipHorizontal();
            }
        }));
        botoes.push_back(new Botao(1250, -175, 150, 50, "Grayscale", 0.5, 0.5, 0.5, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if (imageManager.fourthImage != NULL) {
                delete imageManager.fourthImage;
                }
            imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
            imageManager.fourthImage->image_Gray();
            }
        }));
        botoes.push_back(new Botao(1250, -250, 150, 50, "Red", 1, 0, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->image_R();
            }
        }));
        botoes.push_back(new Botao(1250, -325, 150, 50, "Green", 0, 1, 0, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->image_G();
            }
        }));
        botoes.push_back(new Botao(1250, -400, 150, 50, "Blue", 0, 0, 1, [&imageManager](){
            if (imageManager.selectedImage != NULL) {
                if(imageManager.fourthImage != NULL){
                delete imageManager.fourthImage;
                }
                imageManager.fourthImage = new Bmp(*imageManager.selectedImage);
                imageManager.fourthImage->image_B();
            }
        }));
        botoes.push_back(new Botao(900, -400, 150, 50, "Load Image 3", 0.75, 1, 1, [&imageManager](){
            LoadImages(imageManager.images, ".\\1_canvasGlut\\images\\img1.bmp");
        }));
        botoes.push_back(new Botao(900, -325, 150, 50, "Load Image 2", 1, 0.75, 1, [&imageManager](){
            LoadImages(imageManager.images, ".\\1_canvasGlut\\images\\gato.bmp");
        }));
        botoes.push_back(new Botao(900, -250, 150, 50, "Load Image 1", 1, 1, 0.75, [&imageManager](){
            LoadImages(imageManager.images, ".\\1_canvasGlut\\images\\snail.bmp");
        }));
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
    }
    }
};
