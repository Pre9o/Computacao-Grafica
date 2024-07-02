/**  
  A classe ImageManager é usada para gerenciar várias imagens BMP. Ela mantém um vetor de ponteiros para imagens BMP,
  bem como ponteiros para a imagem selecionada, a quarta imagem e a imagem que está sendo arrastada.
  
  A classe também mantém um booleano para indicar se o histograma está ativo e um inteiro para a opção de histograma selecionada.
 */

#include <vector>
#include "Image.h"

#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

class ImageManager {
public:
    std::vector<Bmp*> images; // Vetor de ponteiros para imagens BMP.
    Bmp* selectedImage = NULL; // Ponteiro para a imagem selecionada.
    Bmp* fourthImage = NULL; // Ponteiro para a quarta imagem.
    Bmp* draggingImage = NULL; // Ponteiro para a imagem que está sendo arrastada.

    bool histograma = false; // Indica se o histograma está ativo.

    int OpcaoHistograma = 0; // Opção de histograma selecionada.
};

#endif