#include "../include/Triangle.h"
#include "../include/Plane.h"
#include "../include/Operations.h"
#include "../include/UniversalVars.h"
// #include "trespontinhos.cpp"
#include <iostream>
#include <vector>

using namespace std;

// Construtores
Triangle::Triangle(string objName, Point a, Point b, Point c, Vector ka, Vector kd, Vector ks, double shininess, double kr, double kt, Texture* visualTexture, Texture* heightTexture) :
    Hittable(objName, ka, kd, ks, shininess, kr, kt, "Triangle", visualTexture, heightTexture),
    a(a),
    b(b),
    c(c) { recalculateNormal(); }

// Print do triangulo no formato <(x1, y1, z1), (x2, y2, z2), (x3, y3, 3z)>
void Triangle::print() const {
    std::cout << "<" << 
    "(" << a.getX() << ", " << a.getY() << ", " << a.getZ() << ")" << 
    "(" << b.getX() << ", " << b.getY() << ", " << b.getZ() << ")" << 
    "(" << c.getX() << ", " << c.getY() << ", " << c.getZ() << ")" << 
    ">" << std::endl;
}

// Getters
const Point& Triangle::getA() const { return a; }
const Point& Triangle::getB() const { return b; }
const Point& Triangle::getC() const { return c; }
const Vector& Triangle::getNormal() const { return normal; }
const Point Triangle::getCentroid() const {
    return getA() + (1.0/3.0 * (getB()-getA())) + (1.0/3.0 * (getC()-getA()));
}

// Setters
void Triangle::setA(Point newA) { a = newA; }
void Triangle::setB(Point newB) { b = newB; }
void Triangle::setC(Point newC) { c = newC; }
void Triangle::setNormal(Vector newNormal) { normal = newNormal; }
void Triangle::recalculateNormal() {
    setNormal(cross(c-b, a-b).normalized());
    //     c
    //    /|
    //   / |
    //a /__| b
}

/*
// Interseção de um vetor com o triangulo
// HitRecord Triangle::hit(const Ray& r) const {
//     unsigned option = 1;

//     if (option == 0) {
//         // Plano da face do triângulo
//         Plane p = Plane("tempPlane", getA(), getNormal(), getka(), getkd(), getks(), getshininess(), getkr(), getkt());
//         // rec guarda as informações se r intersecta o plano p
//         HitRecord rec = p.hit(r);

//         // Se r não intersecta o plano então também não intersecta o triângulo
//         // Se r intersecta o plano então temos que verificar se intersecta o triângulo
//         // Se alfa, beta e gama forem diferentes de 0 então r intersecta o triângulo
//         if (rec.t == numeric_limits<double>::max() || rec.t <= 0) { return HitRecord{}; }

//         if (has(rec.hit_point)) {
//             double dDotN = dot(r.direction, getNormal());

//             rec.t = rec.t;
//             rec.hit_point = r.origin + (rec.t * r.direction);
//             rec.normal = dDotN < 0 ? getNormal() : -1 * getNormal();
//             rec.ka = getka();
//             rec.ks = getks();
//             rec.kd = getkd();
//             rec.shininess = getshininess();
//             rec.kr = getkr();
//             rec.kt = getkt();
//             rec.obj = this;
//             return rec;
//         } else {
//             return HitRecord{};
//         }
//     } else if (option == 1) {
//         const double t_min = 0.0000001;
//         Vector edge1 = b - a;
//         Vector edge2 = c - a;
//         Vector h = cross(r.direction, edge2);
//         double det = dot(edge1, h);

//         // Passo 1: Verifica se o raio é paralelo ao triângulo.
//         // O determinante é próximo de zero.
//         if (det > -t_min && det < t_min) {
//             return HitRecord{}; // Retorna um registro vazio (sem colisão)
//         }

//         double inv_det = 1.0 / det;
//         Vector s = r.origin - a;
//         double u = inv_det * dot(s, h);

//         // Passo 2: Verifica se a coordenada baricêntrica U está fora do intervalo [0,1].
//         if (u < 0.0 || u > 1.0) {
//             return HitRecord{};
//         }

//         Vector q = cross(s, edge1);
//         double v = inv_det * dot(r.direction, q);

//         // Passo 3: Verifica se a coordenada baricêntrica V está fora do intervalo [0,1].
//         if (v < 0.0 || u + v > 1.0) {
//             return HitRecord{};
//         }

//         // Passo 4: Calcula 't', a distância até o ponto de interseção.
//         double t = inv_det * dot(edge2, q);

//         if (t > t_min) { // A interseção está na frente do raio.
//             HitRecord rec;
//             rec.t = t;
//             rec.hit_point = r.origin + (t * r.direction);
            
//             // Usa a lógica que já implementamos para a normal de frente/trás
//             Vector outward_normal = this->getNormal();
//             bool front_face = dot(r.direction, outward_normal) < 0;
//             rec.normal = front_face ? outward_normal : -1 * outward_normal;
            
//             // Preenche o resto com os materiais
//             rec.ka = this->getka();
//             rec.kd = this->getkd();
//             rec.ks = this->getks();
//             rec.shininess = this->getshininess();
//             rec.kr = this->getkr();
//             rec.kt = this->getkt();
//             rec.obj = this;
            
//             return rec;
//         }

//         // Se t não for positivo, o triângulo está atrás do raio.
//         return HitRecord{};
//     }

//     return HitRecord{};
// }
*/

HitRecord Triangle::hit(const Ray& r) const {
    // Plano da face do triângulo
    Plane p = Plane("tempPlane", getA(), getNormal(), getka(), getkd(), getks(), getshininess(), getkr(), getkt());
    // rec guarda as informações se r intersecta o plano p
    HitRecord rec = p.hit(r);

    // Se r não intersecta o plano então também não intersecta o triângulo
    // Se r intersecta o plano então temos que verificar se intersecta o triângulo
    // Se alfa, beta e gama forem diferentes de 0 então r intersecta o triângulo
    if (rec.t == numeric_limits<double>::max() || rec.t < t_min) { return HitRecord{}; }

    pair<double, double> vu = has(rec.hit_point);
    double u = vu.first;
    double v = vu.second;
    if (u != -1) {
        rec.obj = this;
        // Vector edge1 = b - a;
        // As coordenadas baricêntricas (u, v) são usadas como coordenadas de textura.
        // O mapeamento padrão para um triângulo é: A=(0,0), B=(1,0), C=(0,1)
        // Nossas coordenadas u e v correspondem a B e C, respectivamente.
        // if (heightTexture) {
        //     ReliefResult relief = reliefMapping(r, r.origin + (rec.t * r.direction), getNormal(), edge1, heightTexture, u, v);
        //     rec.hit_point = relief.displacedHitPoint;
        //     rec.normal = relief.perturbedNormal;
        // } else {
        //     // rec.hit_point já definido pela intersecção com o plano
        //     rec.normal = dot(r.direction, rec.normal) < 0 ? rec.normal : -1 * rec.normal;
        // }

        if (visualTexture) {
            rec.kd = visualTexture->value({u, v});
        } else {
            rec.kd = getkd();
        }

        rec.ka = getka();
        rec.ks = getks();
        rec.shininess = getshininess();
        rec.kr = getkr();
        rec.kt = getkt();
        return rec;
    } else {
        return HitRecord{};
    }
}

// Retorna os valores de alfa, beta e gama se o ponto puder ser definido
// a partir dos três vetores dados, isto é, o ponto está dentro do triângulo,
// caso contrário retorna {0, 0, 0}
pair<double, double> Triangle::has(Point p) const {
    // Sendo P o ponto que queremos testar e A, B e C os vertices do triangulo
    // P = A + (u * (B - A)) + (v * (C - A))
    // P - A = (u * (B - A)) + (v * (C - A))
    // PA = (u * (BA)) + (v * (CA))
    // 
    // Se x = y sendo x e y vetores, então dot(x, z) = dot(y, z)
    // para qualquer vetor z, logo, temos
    // dot(PA, BA) = dot((u * BA), BA) + dot((v * CA), BA) e
    // dot(PA, CA) = dot((u * BA), CA) + dot((v * CA), CA)
    // que é igual a
    // dot(PA, BA) = u * dot(BA, BA) + v * dot(CA, BA) e
    // dot(PA, CA) = u * dot(BA, CA) + v * dot(CA, CA)
    // que são duas equações com duas variáveis.
    // Assim conseguimos achar u e v

    Vector pa = p - getA();
    Vector ba = getB() - getA();
    Vector ca = getC() - getA();
    double paba = dot(pa, ba);
    double baba = dot(ba, ba);
    double caba = dot(ca, ba);
    double paca = dot(pa, ca);
    double baca = dot(ba, ca);
    double caca = dot(ca, ca);

    // Se x = uy + vz e a = ub + vc, então
    // v = (ay - xb) / (yc - zb)
    // u = (x - vz) / y
    double v;
    double u;
    try
    {
        v = (paca * baba - paba * baca) / (baba * caca - caba * baca);
        u = (paba - v * caba) / baba;
    }
    catch(const std::exception& e)
    {
        return {-1,-1};
    }

    // Para que o ponto esteja dentro do triângulo u e v precisam ser >= 0
    // e u + v <= 1, pois é preciso que o escalar "avance" mas não avance
    // tanto a ponto de sair do triângulo

    if (u < 0) { return {-1,-1}; }
    if (v < 0) { return {-1,-1}; }

    if (u + v <= 1) {
        return {u,v};
    } else {
        return {-1,-1};
    }
}

void Triangle::rotateAll(double angle) {
    // pivot é o centróide do triângulo, isto é, ponto referência para fazermos a rotação
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotation(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
}

void Triangle::rotateX(double angle) {
    // pivot é o centróide do triângulo, isto é, ponto referência para fazermos a rotação
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationX(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
}

void Triangle::rotateY(double angle) {
    // pivot é o centróide do triângulo, isto é, ponto referência para fazermos a rotação
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationY(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
}
void Triangle::rotateZ(double angle) {
    // pivot é o centróide do triângulo, isto é, ponto referência para fazermos a rotação
    Point pivot = getCentroid();
    Matrix translationOriginMatrix(4);
    Matrix rotationMatrix(4);
    Matrix translationBackMatrix(4);
    Matrix result(4);

    translationOriginMatrix.buildTranslation({-pivot.getX(), -pivot.getY(), -pivot.getZ()});
    rotationMatrix.buildRotationZ(angle);
    translationBackMatrix.buildTranslation({pivot.getX(), pivot.getY(), pivot.getZ()});

    // Compomos as três matrizes em apenas uma (result)
    result = translationBackMatrix * (rotationMatrix * translationOriginMatrix);
    setA(getA() * result);
    setB(getB() * result);
    setC(getC() * result);
}

void Triangle::transfer(Vector distances){
    Matrix m(4);
    m.buildTranslation(distances);
    
    setA(m*getA());
    setB(m*getB());
    setC(m*getC());
}

Hittable* Triangle::clone() const {
    return new Triangle(*this);
}

BoundingBox Triangle::getBoundingBox() const {
    return BoundingBox(
        getA(),
        getB()+cross(a-b, normal)
    );
}