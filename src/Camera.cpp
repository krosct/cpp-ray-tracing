#include "../include/Camera.h"
#include "../include/Operations.h"
#include <cmath>
#include <iostream>

using namespace std;

// Construtores
Camera::Camera(string cameraName, const Point& location, const Point& pointingAt, const Vector& worldUp, double distance, unsigned int h_res, unsigned int v_res, double aspectRatio, double fieldOfView) :
    cameraName(cameraName),
    location(location), 
    pointingAt(pointingAt), 
    worldUp(worldUp), 
    distance(distance), 
    h_res(h_res), 
    v_res(v_res), 
    aspectRatio(aspectRatio),
    fieldOfView(fieldOfView), 
    U(0, 0, 0), 
    V(0, 0, 0), 
    W(0, 0, 0),
    type("Camera") {
    
    // Define a proporção padrão se não for fornecida (ou for inválida)
    double effectiveAspectRatio = (this->aspectRatio > 0) ? this->aspectRatio : (16.0 / 9.0); // 16:9 como padrão

    // Caso 1: FOV é zero
    if (this->fieldOfView == 0) {
        if (this->h_res == 0 || this->v_res == 0) {
            this->h_res = 960; // Resolução horizontal padrão
            this->v_res = static_cast<unsigned int>(this->h_res / effectiveAspectRatio);
            this->fieldOfView = 60.0; // FOV vertical padrão em graus
        } else {
            // FOV é zero, mas h_res e v_res são válidos. Calcular FOV.
            // Para calcular o FOV vertical a partir da resolução e distância,
            // precisamos de um 'tamanho físico' para a tela de visualização.
            // Assumimos um halfHeight padrão de 1.0 para este cálculo de FOV.
            double assumedHalfHeight = 1.0; 
            this->fieldOfView = (2 * atan(assumedHalfHeight / this->distance)) * 180 / M_PI; // FOV vertical em graus
        }
    } 
    // Caso 2: FOV não é zero, mas h_res ou v_res são zero
    else {
        if (this->h_res == 0 && this->v_res == 0) {
            this->h_res = 960; // Resolução horizontal padrão
            this->v_res = static_cast<unsigned int>(this->h_res / effectiveAspectRatio);
        } else if (this->h_res == 0) {
            this->h_res = static_cast<unsigned int>(this->v_res * effectiveAspectRatio);
        } else if (this->v_res == 0) {
            this->v_res = static_cast<unsigned int>(this->h_res / effectiveAspectRatio);
        }
    }

    // Garante que a proporção da câmera seja baseada nas resoluções finais
    this->aspectRatio = static_cast<double>(this->h_res) / this->v_res; 

    this->calculateBasis();
    
    double theta = this->fieldOfView * M_PI / 180; // FOV vertical para radianos
    double halfHeight = tan(theta/2);
    double halfWidth = this->aspectRatio * halfHeight;
    
    Point corner = this->location - (this->distance * this->W) - (halfWidth * this->U) - (halfHeight * this->V);
    this->createScreen(corner, 2*halfWidth*this->U, 2*halfHeight*this->V);
}

// Getters
Point Camera::getLocation() const { return location; }
Point Camera::getPointingAt() const { return pointingAt; }
Vector Camera::getVectorUp() const { return worldUp; }
double Camera::getDistance() const { return distance; }
unsigned int Camera::getVRes() const { return v_res; }
unsigned int Camera::getHRes() const { return h_res; }
double Camera::getFOV() const { return fieldOfView; }
Vector Camera::getU() const { return U; } // Vetor "direita"
Vector Camera::getV() const { return V; } // Vetor "para cima" real
Vector Camera::getW() const { return W; } // Vetor "para trás"
Screen Camera::getScreen() const { return screen; }
string Camera::getName() const { return cameraName; }
string Camera::getType() const { return type; }
double Camera::getAspectRatio() const { return aspectRatio; }

// Setters
void Camera::setLocation(const Point& loc) {
    location = loc;
    calculateBasis(); // Recalcula a base sempre que a câmera se move
}
void Camera::setPointingAt(const Point& point) {
    pointingAt = point;
    calculateBasis(); // Recalcula a base sempre que a mira muda
}
void Camera::setVectorUp(const Vector& up) { worldUp = up; }
void Camera::setDistance(double d) {
    distance = d;
}
void Camera::setVRes(unsigned int res) { v_res = res; }
void Camera::setHRes(unsigned int res) {
    h_res = res;
}
void Camera::setFOV(double fov) { fieldOfView = fov; }
void Camera::setU(const Vector& newU) { U = newU; } // Vetor "direita"
void Camera::setV(const Vector& newV) { V = newV; } // Vetor "para cima" real
void Camera::setW(const Vector& newW) { W = newW; } // Vetor "para trás"

// O coração da lógica da câmera
void Camera::calculateBasis() {
    // Calcula o vetor W (oposto à direção da mira), conforme a especificação
    W = (location - pointingAt).normalized();

    // Calcula o vetor U (eixo z da câmera, direção horizontal)
    // U é o produto vetorial do "para-cima" do mundo com W
    U = cross(worldUp, W).normalized();

    // Calcula o vetor V (eixo y da câmera, direção vertical)
    // V é o produto vetorial de W e U para garantir a ortogonalidade
    V = cross(W, U); // Não precisa normalizar, pois U e W já são ortonormais
}

void Camera::createScreen(const Point& p, Vector horizontal, Vector vertical) {
    screen.lower_left_corner = p;
    screen.horizontal = horizontal;
    screen.vertical = vertical;
}

// Print da camera
void Camera::print() const {
    cout << type << "(" << getName() << "): " << endl;
    cout << "location: "; getLocation().print();
    cout << "pointingAt: "; getPointingAt().print();
    cout << "worldUp: "; getVectorUp().print();
    cout << "h_res: " << getHRes() << endl;
    cout << "v_res: " << getVRes() << endl;
    cout << "aspectRatio: " << getAspectRatio() << endl;
    cout << "fieldOfView: " << getFOV() << endl;
}