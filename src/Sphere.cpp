#include "../include/Sphere.h"
#include "../include/Operations.h"
#include "../include/Ray.h"
#include <iostream>
#include <cmath>
#include "../include/UniversalVars.h"

using namespace std;

// Construtores
Sphere::Sphere(string objName, Point center, double radius, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Sphere", visualTexture),
    center(center),
    radius(radius)
    {}

// Print do vetor no formato (center, radius, <x, y, z>)
void Sphere::print() const {
    std::cout << "Center at <" << center.getX() << ", " << center.getY() << ", " << center.getZ() << ">" 
    << "; Radius of " << radius << std::endl;
}

// Getters
const Point& Sphere::getCenter() const { return center; }
const double& Sphere::getRadius() const { return radius; }

// Setters
void Sphere::setCenter(Point newCenter) { center = newCenter; }
void Sphere::setRadius(double newRadius) { radius = newRadius; }

// Interseção de um vetor com a esfera
HitRecord Sphere::hit(const Ray& r) const {
    Vector normal = {};
    // // Equação da esfera: (x-c1)² + (y-c2)² + (z-c3)² = r²,
    // Equação da esfera: |P - C|² = r²,
    // onde P é u ponto da superfície da esfera, C é o seu centro e r é o seu raio
    // Equação vetorial da reta: p(t) = o + td,
    // onde p(t) é um ponto qualquer do vetor, o é a origem do vetor, t é um escalar e d é o vetor diretor
    // |(o + td) - C|² = r²
    // |td + OC|² = r² -> |x|² = x . x
    // (td + OC) . (td + OC) = r²
    // (d . d)t² + 2(d . OC)t + (OC . OC) - r² = 0 -> Equação quadrática na forma at²+bt+c=0.
    // Discriminante de uma equação de segundo grau: b² - 4ac.

    Vector oc = r.origin - getCenter();
    double a = dot(r.direction, r.direction);
    double b = 2.0 * dot(r.direction, oc);
    double c = dot(oc, oc) - (getRadius() * getRadius());

    double discriminant = b * b - 4 * a * c;
    // Se o discriminante for < 0 então não temos raízes reais, ou seja não há interseção
    if (discriminant < 0) { return HitRecord{}; }
    
    bool hitForReal = false;
    HitRecord rec{};
    // t1 e t2 são os valores de t que satisfazem a(s) interseção(ões)
    double t1;
    double tres;
    // Um pequeno epsilon (t_min) para evitar artefatos de precisão (z-fighting)
    // Epsilon é importante para evitar que objetos "encostando" na câmera ou um no outro
    // causem problemas de interseção.
    // Se o discriminante for = 0 então temos apenas uma raíz, ou seja há interseção
    if (discriminant == 0) {
        t1 = (-b) / (2 * a);
        if (t1 > t_min) {
            hitForReal = true;
            tres = t1;
        } else {
            return HitRecord{};
        }
    } else {
        // Se o discriminante for > 0 então temos duas raízes, ou seja há interseção
        double root = sqrt(discriminant);
        double t2 = t1 = root;
        t1 = (-b + t1) / (2 * a);
        t2 = (-b - t2) / (2 * a);
        tres = t1 < t2 ? t1 : t2;
        if (tres > t_min) {
            hitForReal = true;
        } else {
            return HitRecord{};
        }
    }

    if (hitForReal) {
        rec.hit_point = r.origin + tres * r.direction;
        normal = (rec.hit_point - getCenter()).normalized();
        rec.normal = dot(r.direction, normal) < 0 ? normal : -1 * normal;
        if (visualTexture) {
            double u = (atan2(rec.normal.getZ(), rec.normal.getX()) + M_PI) / (2 * M_PI);
            double v = 0.5 - (asin(rec.normal.getY()) / M_PI);
            rec.kd = visualTexture->value({u, v});
        } else {
            rec.kd = getkd();
        }
        rec.ka = getka();
        rec.ks = getks();
        rec.t = t1;
        rec.kr = getkr();
        rec.kt = getkt();
        rec.shininess = getshininess();
        rec.obj = this;
    }
    
    return rec;
}

void Sphere::rotateAll(double angle) {
    cout << angle << "TODO SPHERE!" << endl;
}
void Sphere::rotateX(double angle) {
    cout << angle << "TODO SPHERE!" << endl;
}
void Sphere::rotateY(double angle) {
    cout << angle << "TODO SPHERE!" << endl;
}
void Sphere::rotateZ(double angle) {
    cout << angle << "TODO SPHERE!" << endl;
}
void Sphere::transfer(Vector distances) {
    cout << "TODO SPHERE!" << endl;
    distances.print();
}

Hittable* Sphere::clone() const {
    return new Sphere(*this);
}

BoundingBox Sphere::getBoundingBox() const {
    double r = getRadius();
    Point c = getCenter();
    return BoundingBox(
        Point(c.getX() - r, c.getY() - r, c.getZ() - r),
        Point(c.getX() + r, c.getY() + r, c.getZ() + r)
    );
}