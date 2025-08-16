#pragma once
#include "Hittable.h"
#include "Point.h"
#include "Vector.h"
#include "Ray.h"
#include "Texture.h"
#include "Triangle.h"

class Square : public Hittable {
private:
    Point a;
    Point b;
    Point c;
    Point d;
    Vector normal;

public:
    // Construtores
    Square(std::string objName, Point a, Point c, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture = nullptr, Texture* heightTexture = nullptr);

    // Print do quadrado
    void print() const override;

    // Getters
    const Point& getA() const;
    const Point& getB() const;
    const Point& getC() const;
    const Point& getD() const;
    const Vector& getNormal() const;
    const Point getCentroid() const;

    // Setters
    void setA(Point newA);
    void setB(Point newB);
    void setC(Point newC);
    void setD(Point newD);
    void setNormal(Vector newNormal);
    void recalculateNormal();

    // Interseção de um vetor com o quadrado
    HitRecord hit(const Ray& r) const override;
    
    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;

    // Retorna o par u e v das coordenadass baricentricas
    std::pair<double, double> has(Point p) const;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector distances) override;
};