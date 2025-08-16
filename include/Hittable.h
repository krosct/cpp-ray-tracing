#pragma once
#include "UniversalVars.h"
#include "BoundingBox.h"
#include "Point.h"
#include "Vector.h"
// #include <limits>
// #include <string>
// #include <cmath>

class Texture;
class Ray;
class Hittable;

struct HitRecord {
    double t = maxDouble; // Ele nos diz a que distância ao longo do raio a colisão ocorreu. O t menor é sempre a colisão mais próxima.
    Point hit_point{}; // O ponto exato da colisão no espaço 3D.
    Vector normal{}; // Este vetor é perpendicular à superfície e é essencial para calcular sombreamento e reflexos.
    Vector ka{}; // Coeficiente ambiente
    Vector kd{}; // Coeficiente difuso
    Vector ks{}; // Coeficiente especular
    double shininess = 0; // Expoente n para definir o brilho
    double kr = 0; // Coeficiente de reflexão
    double kt = 0; // Coeficiente de transmissão
    const Hittable* obj = nullptr; // Ponteiro para o objeto atingido
};

class Hittable {
public:
    std::string objName;
    Vector ka; // Coeficiente ambiente (pode ser um escalar ou vetor para cor)
    Vector kd; // Coeficiente difuso
    Vector ks; // Coeficiente especular
    double shininess; // Expoente n
    double kr; // Coeficiente de reflexão
    double kt; // Coeficiente de transmissão
    std::string type;
    Texture* visualTexture;
    Texture* heightTexture;

    Hittable(std::string objName, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt,std::string type, Texture* visualTexture = nullptr, Texture* heightTexture = nullptr) : 
    objName(objName), ka(ka), kd(kd), ks(ks), shininess(shininess), kr(kr), kt(kt), type(type), visualTexture(visualTexture), heightTexture(heightTexture) {}

    // O 'virtual' permite que classes filhas reimplementem esta função.
    // O '= 0' a torna uma "função virtual pura", o que significa que
    // a classe Hittable é abstrata e não pode ser instanciada.
    // Classes filhas SÃO OBRIGADAS a implementar esta função.
    virtual HitRecord hit(const Ray& r) const = 0;
    virtual void rotateAll(double angle) = 0;
    virtual void rotateX(double angle) = 0;
    virtual void rotateY(double angle) = 0;
    virtual void rotateZ(double angle) = 0;
    virtual void transfer(Vector distances) = 0;
    virtual void print() const {}
    virtual Hittable* clone() const = 0;
    virtual BoundingBox getBoundingBox() const = 0;
    
    inline const Vector& getka() const { return ka; }
    inline const Vector& getkd() const { return kd; }
    inline const Vector& getks() const { return ks; }
    inline const double& getshininess() const { return shininess; }
    inline const double& getkr() const { return kr; }
    inline const double& getkt() const { return kt; }
    inline const std::string& getName() const { return objName; }
    inline const std::string& getType() const { return type; }
    
    // Destrutor virtual é importante para classes base polimórficas
    virtual ~Hittable() = default;
};

// Comparação entre dois HitRecord
bool operator==(const HitRecord& a, const HitRecord& b);
bool operator!=(const HitRecord& a, const HitRecord& b);
