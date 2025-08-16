#pragma once
#include "Point.h"
#include "Vector.h"

struct Screen {
    Point lower_left_corner;
    Vector horizontal;
    Vector vertical;
};

class Camera {
private:
    std::string cameraName;
    Point location;      // Ponto C
    Point pointingAt;    // Ponto M
    Vector worldUp;      // Vetor V_up (temporário)
    double distance;     // Distância d
    unsigned int h_res;  // Resolução horizontal
    unsigned int v_res;  // Resolução vertical
    double aspectRatio;  // Proporção da tela
    double fieldOfView;  // Angulo do campo de visão da câmera em graus
    Vector U, V, W;      // Vetores da base da câmera. W é para trás. U é para direita. V é para cima.
    Screen screen;       // Tela de projeção
    std::string type;

    void calculateBasis();

public:
    // Construtores
    Camera(std::string cameraName, const Point& location, const Point& pointingAt, const Vector& worldUp, double distance = 1, unsigned int h_res = 0, unsigned int v_res = 0, double aspectRatio = 0, double fieldOfView = 0);

    // Getters
    Point getLocation() const;
    Point getPointingAt() const;
    Vector getVectorUp() const;
    double getDistance() const;
    unsigned int getVRes() const;
    unsigned int getHRes() const;
    double getFOV() const;
    Vector getU() const;
    Vector getV() const;
    Vector getW() const;
    Screen getScreen() const;
    std::string getName() const;
    std::string getType() const;
    double getAspectRatio() const;

    // Setters
    void setLocation(const Point& loc);
    void setPointingAt(const Point& point);
    void setVectorUp(const Vector& up);
    void setDistance(double d);
    void setVRes(unsigned int res);
    void setHRes(unsigned int res);
    void setFOV(double fov);
    void setU(const Vector& newU);
    void setV(const Vector& newV);
    void setW(const Vector& newW);
    void createScreen(const Point& p, Vector horizontal, Vector vertical);

    // Print da camera
    void print() const;
};