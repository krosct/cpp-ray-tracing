#include "Camera.h"
#include "Operations.h"
#include <cmath>

// Construtores
Camera::Camera(const Point& location, const Point& pointingAt, const Vector& worldUp, double distance, unsigned int h_res, unsigned int v_res, double fieldOfView) :
    location(location), pointingAt(pointingAt), worldUp(worldUp), distance(distance), h_res(h_res), v_res(v_res), fieldOfView(fieldOfView), U(0, 0, 0), V(0, 0, 0), W(0, 0, 0) {
    calculateBasis();
    aspectRatio = h_res/v_res;
    double theta = fieldOfView * M_PI / 180; // Para radianos
    double halfHeight = tan(theta/2);
    double halfWidth = aspectRatio * halfHeight;
    Point corner = location - (distance * W) - (halfWidth * U) - (halfHeight * V);
    createScreen(corner, 2*halfWidth*U, 2*halfHeight*V);
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

    // Calcula o vetor U (eixo x da câmera, "direita")
    // U é o produto vetorial do "para-cima" do mundo com W
    U = cross(worldUp, W).normalized();

    // Calcula o vetor V (eixo y da câmera, "para-cima" real)
    // V é o produto vetorial de W e U para garantir a ortogonalidade
    V = cross(W, U); // Não precisa normalizar, pois U e W já são ortonormais
}

void Camera::createScreen(const Point& p, Vector horizontal, Vector vertical) {
    screen.lower_left_corner = p;
    screen.horizontal = horizontal;
    screen.vertical = vertical;
}