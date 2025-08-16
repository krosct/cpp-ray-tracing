#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Point.h"
#include "Vector.h"
#include "Texture.h"

class Cylinder : public Hittable {
private:
    Point center;
    double radius;
    double height;
    Texture* visualTexture;
public:
    Cylinder(std::string objName, Point center, double radius, double height, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture = nullptr);
    
    // Print do cilindro
    void print() const;

    // Getters
    const Point& getCenter() const;
    double getRadius() const;
    double getHeight() const;

    // Setters
    void setCenter(Point newCenter);
    void setRadius(double newRadius);
    void setHeight(double newHeight);

    // Interseção de um vetor com o cilindro
    HitRecord hit(const Ray& r) const override;

    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector d) override;
};
