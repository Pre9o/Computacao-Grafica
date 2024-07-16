#include "Modelos.cpp"

class Cilindro : public Modelos {
    private:
        void DrawCilindro() {

            // Desenhar arestas da base inferior
            for (int i = 0; i < numeroDeLados; i++) {
                //CV::color(0, 0, 1);
                CV::line(vertices2D[i], vertices2D[(i + 1) % numeroDeLados]);
            }

            // Desenhar arestas da base superior
            for (int i = numeroDeLados; i < 2 * numeroDeLados; i++) {
                //CV::color(0, 1, 1);
                CV::line(vertices2D[i], vertices2D[(i + 1) % numeroDeLados + numeroDeLados]);
            }

            // Desenhar arestas verticais
            for (int i = 0; i < numeroDeLados; i++) {
                //CV::color(1, 0, 1);
                CV::line(vertices2D[i], vertices2D[i + numeroDeLados]);
            }

            vertices2D.clear();
            vertices3D.clear();
        }


    public:
        float raio;
        float altura;
        int numeroDeLados;

        Cilindro(Vector3 coordenadasIniciais, Vector3 rotacaoAplicada, float raio, float altura, int numeroDeLados) : Modelos(coordenadasIniciais, rotacaoAplicada) {
            this->raio = raio;
            this->altura = altura;
            this->numeroDeLados = numeroDeLados;
        }

        void GerarVertices(){
            float anguloRad = (2 * M_PI / numeroDeLados);

            for(int i = 0; i < numeroDeLados; i++){
                float x = raio * cos(anguloRad * i);
                float y = 0;
                float z = raio * sin(anguloRad * i);

                vertices3D.push_back(Vector3(x, y, z));
            }

            for(int i = 0; i < numeroDeLados; i++){
                float x = raio * cos(anguloRad * i);
                float y = altura;
                float z = raio * sin(anguloRad * i);

                vertices3D.push_back(Vector3(x, y, z));
            }
        }

        void aplicarPipelineParaDesenho(Vector3 cameraPosition, Vector3 cameraRotation, float distance){
            GerarVertices();
            Pipeline(vertices3D, cameraPosition, cameraRotation, distance);
            DrawCilindro();
        }
};