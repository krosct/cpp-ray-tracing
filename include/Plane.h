#pragma once
#include "Hittable.h"
#include "Point.h"
#include "Vector.h"

class Plane : public Hittable {
private:
    Point planePoint;
    Vector normal;
public:
    // Constructor
    Plane(std::string objName, Point newPlanePoint, Vector newNormal, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt);

    // Getters
    const Point& getPlanePoint() const;
    const Vector& getNormal() const;
    
    // Setters
    void setPlanePoint(Point& planePoint);
    void setNormal(Vector& normal);

    // Hit
    HitRecord hit(const Ray& ray) const override;

    Hittable* clone() const override;
    BoundingBox getBoundingBox() const override;
    void print() const override;

    void rotateAll(double angle) override;
    void rotateX(double angle) override;
    void rotateY(double angle) override;
    void rotateZ(double angle) override;
    void transfer(Vector distances) override;
};
