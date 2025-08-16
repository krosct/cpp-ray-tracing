#pragma once
#include "Hittable.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#include "Texture.h"

class Triangle : public Hittable {
private:
    Point a;
    Point b;
    Point c;
    Vector normal;
public:
    // Construtores
    Triangle(std::string objName, Point a, Point b, Point c, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture = nullptr, Texture* heightTexture = nullptr);

    // Print do triangulo no formato <(x1, y1, z1), (x2, y2, z2), (x3, y3, 3z)>
    void print() const override;

    // Getters
    const Point& getA() const;
    const Point& getB() const;
    const Point& getC() const;
    const Vector& getNormal() const;
    const Point getCentroid() const;

    // Setters
    void setA(Point newA);
    void setB(Point newB);
    void setC(Point newC);
    void setNormal(Vector newNormal);
    void recalculateNormal();

    // Interseção de um vetor com o triangulo
    HitRecord hit(const Ray& r) const override;

    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;

    // Retorna true se o ponto puder ser definido a partir dos três 
    // vetores dados, isto é, o ponto está dentro do triângulo,
    // caso contrário retorna false
    std::pair<double, double> has(Point p) const;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector distances) override;
};
