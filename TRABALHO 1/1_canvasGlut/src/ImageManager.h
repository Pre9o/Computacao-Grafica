#include <vector>
#include "Image.h"

#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

class ImageManager {
public:
    std::vector<Bmp*> images;
    Bmp* selectedImage = NULL;
    Bmp* fourthImage = NULL;
    Bmp* draggingImage = NULL;

    bool histograma = false;

    int OpcaoHistograma = 0;
};

#endif
