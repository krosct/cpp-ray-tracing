#include "../include/Cylinder.h"
#include "../include/Operations.h"
#include "../include/Matrix.h"
#include <iostream>
#include <cmath>

using namespace std;

Cylinder::Cylinder(string objName, Point center, double radius, double height, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Cylinder"),
    center(center),
    radius(radius),
    height(height),
    visualTexture(visualTexture) {}

// Getters
const Point& Cylinder::getCenter() const { return center; }
double Cylinder::getRadius() const { return radius; }
double Cylinder::getHeight() const { return height; }

// Setters
void Cylinder::setCenter(Point newCenter) { center = newCenter; }
void Cylinder::setRadius(double newRadius) { radius = newRadius; }
void Cylinder::setHeight(double newHeight) { height = newHeight; }

HitRecord Cylinder::hit(const Ray& r) const {
    if (!getBoundingBox().hit(r)) {
        return HitRecord{};
    }

    HitRecord rec{}; // Cria um registro para guardar os dados da colisão
    bool hitForReal = false;
    double minT = numeric_limits<double>::max();
    double lateral = numeric_limits<double>::max();
    double bottom = numeric_limits<double>::max();
    double top = numeric_limits<double>::max();
    Vector normal = {};

    // Raio em relação ao centro do cilindro
    // Subtraímos a origem do raio do centro do cilindro
    // Isso move a geometria para que o cilindro fique na origem (0,0,0), o que simplifica os cálculos
    Vector oc = r.origin - center;

    // Equação para a parte lateral do cilindro
    // O cilindro lateral pode ser descrito pela equação x² + z² = r²
    // O raio é uma linha reta: P(t) = O + t*D, onde O é a origem e D é a direção
    // Substituindo P(t) na equação do cilindro, obtemos uma equação do segundo grau (ax² + bx + c = 0)
    // para 't', o parâmetro de distância
    double a = r.direction.getX() * r.direction.getX() + r.direction.getZ() * r.direction.getZ();
    // 'a' é o termo do raio (direção do raio ao quadrado) para o cálculo do t
    
    double b = 2.0 * (oc.getX() * r.direction.getX() + oc.getZ() * r.direction.getZ());
    // 'b' é o termo do raio e da origem do cilindro

    double c = oc.getX() * oc.getX() + oc.getZ() * oc.getZ() - radius * radius;
    // 'c' é o termo constante da equação

    double discriminant = b * b - 4 * a * c;
    // O 'discriminante' determina quantas soluções a equação tem
    // Se for < 0, o raio não toca o cilindro (nenhuma colisão)
    // Se for = 0, o raio toca em um ponto (tangente)
    // Se for > 0, o raio toca em dois pontos (colisão e saída)

    if (discriminant >= 0) {
        // Encontra o ponto de colisão mais próximo, ou seja, o menor valor de 't'
        // lateral é a fórmula de Bhaskara para a equação do segundo grau
        lateral = (-b - sqrt(discriminant)) / (2.0 * a);
        if (lateral > 0) {
            Point hit_point = r.origin + lateral * r.direction;
            double y = hit_point.getY();
            
            // Verifica se a colisão está dentro da altura do cilindro
            // O cilindro tem um início em y = center.getY() e termina em y = center.getY() + height
            if (y >= center.getY() && y <= center.getY() + height) {
                hitForReal = true;
                minT = lateral;
                normal = Vector(hit_point.getX() - center.getX(), 0, hit_point.getZ() - center.getZ()).normalized();
            }
        }
    }
    
    // Tampa inferior (y = center.getY())
    // PosicaoY_do_Raio = r.origin.getY() + t * r.direction.getY()
    // PosicaoY_da_Base = center.getY()
    // r.origin.getY() + t * r.direction.getY() = center.getY()
    bottom = (center.getY() - r.origin.getY()) / r.direction.getY();
    // 'bottom' é a distância até o ponto onde o raio cruza o plano da tampa inferior
    if (bottom > 0) {
        Point hit_point = r.origin + bottom * r.direction;
        // Verificar se o ponto de colisão está dentro do raio do círculo da tampa:
        // (x - cx)² + (z - cz)² <= r²
        if (pow(hit_point.getX() - center.getX(), 2) + pow(hit_point.getZ() - center.getZ(), 2) <= pow(radius, 2)) {
            // Compara com a colisão lateral (se existir) para ver qual é a mais próxima
            if (bottom < minT) {
                hitForReal = true;
                minT = bottom;
                // A normal da tampa inferior sempre aponta para baixo, no eixo Y negativo
                normal = Vector(0, -1, 0);
            }
        }
    }
    
    // Tampa superior (y = center.getY() + height)
    // PosicaoY_do_Raio = r.origin.getY() + t * r.direction.getY()
    // PosicaoY_da_Base = y = center.getY() + height
    // r.origin.getY() + t * r.direction.getY() = center.getY() + height
    top = (center.getY() + height - r.origin.getY()) / r.direction.getY();
    if (top > 0) {
        Point hit_point = r.origin + top * r.direction;
        if (pow(hit_point.getX() - center.getX(), 2) + pow(hit_point.getZ() - center.getZ(), 2) <= pow(radius, 2)) {
            if (top < minT) {
                hitForReal = true;
                minT = top;
                // A normal da tampa superior sempre aponta para cima, no eixo Y positivo
                normal = Vector(0, 1, 0);
            }
        }
    }

    if (hitForReal) {
        rec.hit_point = r.origin + minT * r.direction;
        if (visualTexture) {
            Vector hitPointRelative = rec.hit_point - center;
            // 'u' e 'v' são coordenadas de textura entre 0 e 1
            // 'u' é o ângulo em torno do cilindro
            // 'v' é a altura
            // atan2 calcula o angulo teta (em radianos) em um plano 2d (como se estivesse vendo de cima) do ponto (x, y) com o eixo x
            // DIREITA = 0
            // TRAS = -PI/2
            // ESQUERDA = -PI E +PI
            // FRENTE = PI/2
            // somamos + M_PI ao final para ficarmos apenas com valores positivos
            // DIREITA = PI
            // TRAS = +PI/2
            // ESQUERDA = 0 E 2PI
            // FRENTE = 3PI/2
            // depois dividimos por (2 * M_PI) para calcularmos a porcentagem da volta que o ponto bateu
            double u = (atan2(hitPointRelative.getZ(), hitPointRelative.getX()) + M_PI) / (2 * M_PI);
            // Normaliza o valor de v para ficar entre 0.0 e 1.0
            double v = (hitPointRelative.getY()) / height;

            rec.kd = visualTexture->value({u, v});
        } else {
            rec.kd = getkd();
        }
        
        rec.t = minT;
        rec.ka = getka();
        rec.ks = getks();
        rec.kr = getkr();
        rec.kt = getkt();
        rec.shininess = getshininess();
        rec.normal = dot(r.direction, normal) < 0 ? normal : -normal;
        rec.obj = this;
    }
    
    return rec;
}

// Métodos de rotação e translação
void Cylinder::rotateX(double angle) {
    cout << "Cylinder::rotateX not implemented" << angle << endl;
}

void Cylinder::rotateY(double angle) {
    cout << "Cylinder::rotateY not implemented" << angle << endl;
}

void Cylinder::rotateZ(double angle) {
    cout << "Cylinder::rotateZ not implemented" << angle << endl;
}

void Cylinder::rotateAll(double angle) {
    cout << "Cylinder::rotateAll not implemented" << angle << endl;
}

void Cylinder::transfer(Vector d) {
    center = center + d;
}

void Cylinder::print() const {
    std::cout << "Cylinder <Center: (" << center.getX() << ", " << center.getY() << ", " << center.getZ()
              << "), Radius: " << radius
              << ", Height: " << height
              << ">" << std::endl;
}

Hittable* Cylinder::clone() const {
    return new Cylinder(*this);
}

BoundingBox Cylinder::getBoundingBox() const {
    double r = getRadius();
    Point c = getCenter();
    return BoundingBox(
        Point(c.getX() - r, c.getY(), c.getZ() - r),
        Point(c.getX() + r, c.getY() + getHeight(), c.getZ() + r)
    );
}