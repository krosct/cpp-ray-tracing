#pragma once

#include "Hittable.h"
#include "Ray.h"
#include "Point.h"
#include "Vector.h"

class Rectangle : public Hittable {
private:
    Point p0;        // Canto inicial
    Vector edgeU;    // Vetor da largura (X)
    Vector edgeV;    // Vetor do comprimento (Y)
    Vector normal;   // Normal da face
public:
    Rectangle(std::string objName, Point p0, double width, double length, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt);
    
    // Print do vetor no formato <P0: ('value'), Width: ('value'), Length: ('value')>
    void print() const;

    // Getters
    const Point& getP0() const;
    const Vector& getWidth() const;
    const Vector& getLength() const;
    const Vector& getNormal() const;

    // Setters
    void setP0(Point newP0);
    void setWidth(double newWidth);
    void setLength(double newLength);
    void setNormal(Vector newNormal);

    // Interseção de um vetor com o retangulo
    HitRecord hit(const Ray& r) const override;

    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector d) override;
};