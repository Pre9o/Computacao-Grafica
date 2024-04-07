#include <vector>
#include <algorithm>
#include <string>
#include "gl_canvas2d.h"

void ManipularVetorImagem(std::vector<Bmp*> &images, Bmp* &image);

void ArrastarImagem(Bmp* image, int x, int y);

void DesenharMoldura(Bmp* image);

void DrawImage(Bmp* image);

void InicializarParametros(Bmp* image, int offset);

void LoadImages(std::vector<Bmp*> &images, const char* path);

void Draw4thImage(Bmp* image);

void DesenharHistogramaRed(Bmp* image);