#pragma once
#include "Hittable.h"
#include "Point.h"
#include "Vector.h"
#include "Texture.h"

class Point;
class Vector;

class Sphere : public Hittable {
private:
    Point center;
    double radius;
public:
    // Construtores
    Sphere(std::string objName, Point center, double radius, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture = nullptr);

    // Print do vetor no formato (center, radius, <x, y, z>)
    void print() const override;

    // Getters
    const Point& getCenter() const;
    const double& getRadius() const;

    // Setters
    void setCenter(Point newCenter);
    void setRadius(double newRadius);

    // Interseção de um vetor com a esfera
    HitRecord hit(const Ray& r) const override;

    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector distances) override;
};
