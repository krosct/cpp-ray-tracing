#pragma once
#include "Hittable.h"
#include "Point.h"
#include "Vector.h"
#include "Triangle.h"
#include <vector>

class Cube : public Hittable {
private:
    Point a, b, c, d, e, f, g, h;
    std::vector<Triangle> triangles;

public:
    // Construtores
    Cube(std::string objName, Point a, Point b, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture = nullptr, Texture* heightTexture = nullptr);

    void print() const override;

    // Getters
    const Point& getA() const;
    const Point& getB() const;
    const Point& getC() const;
    const Point& getD() const;
    const Point& getE() const;
    const Point& getF() const;
    const Point& getG() const;
    const Point& getH() const;
    std::vector<Triangle>& getTriangles();
    const std::vector<Vector>& getNormals() const;
    const Point getCentroid() const;

    void resetPoints();
    void addTriangle(Point a1, Point b1, Point c1);

    // Interseção de um vetor com o cubo
    HitRecord hit(const Ray& r) const override;
    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector distances) override;
};

