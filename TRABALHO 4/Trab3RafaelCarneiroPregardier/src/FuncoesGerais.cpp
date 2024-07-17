#include "Cilindro.cpp"
#include "Engrenagem.cpp"

class FuncoesGerais3D{
    public:
        void setNewX(float newX){
            this->newX = newX;
        }

        void setNewY(float newY){
            this->newY = newY;
        }

        float getNewX(){
            return this->newX;
        }

        float getNewY(){
            return this->newY;
        }

        void gerarConexaoVirabrequim(Modelos *modelos[]){
            float angulo = modelos[2]->rotacaoAplicada.z + M_PI;
            float newX = -200 * cos(angulo);
            float newY = -200 * sin(angulo);
            modelos[4] = new Cilindro(Vector3(newX, newY, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);

            setNewX(newX);
            setNewY(newY);
        }

        void gerarPistaoECilindro(Modelos *modelos[]){
            float angulo = modelos[2]->rotacaoAplicada.z + M_PI;
            modelos[4] = new Cilindro(Vector3(this->newX, this->newY, -280), Vector3(M_PI / 2, 0, 0), 15, 220, 4);

            float angleOffset = M_PI * 3 / 2;
            float pistonToCylinderAngle = atan2(600 - this->newY, 0 - this->newX) + angleOffset;
            
            modelos[7] = new Cilindro(Vector3(0, 600, -250), Vector3(0, 0, pistonToCylinderAngle + M_PI), 25, 400, 20);
            modelos[8] = new Cilindro(Vector3(this->newX, this->newY, -250), Vector3(0, 0, pistonToCylinderAngle), 15, 400, 20);
        }

        void executar3D(Modelos *modelos[], Vector3 cameraPosition, Vector3 cameraRotation, float distancia, float velocidadeRotacao){
            for(int i = 0; i < 7; i++){
                    if(i == 6){
                        modelos[i]->aplicarRotacaoDoModelo(-velocidadeRotacao);
                    }
                    else{
                        modelos[i]->aplicarRotacaoDoModelo(velocidadeRotacao);
                    }
            }

            this->gerarConexaoVirabrequim(modelos);
            this->gerarPistaoECilindro(modelos);

            for(int i = 0; i < 9; i++){
                modelos[i]->aplicarPipelineParaDesenho(cameraPosition, cameraRotation, distancia);
            }

        }

    private:
        float newX;
        float newY;
        
};
