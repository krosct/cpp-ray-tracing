#include "../include/Octree.h"
#include "../include/Operations.h"
#include "../include/UniversalVars.h"
#include <limits>

using namespace std;

Octree::Octree(const vector<Hittable*>& objs) {
    // Incluir caixas dos objetos dentro da caixa raiz
    BoundingBox globalBounds;
    for (const auto& obj : objs) {
        globalBounds.expand(obj->getBoundingBox());
    }

    root = new OctreeNode(globalBounds);
    for (auto& obj : objs) {
        root->insert(obj);
    }
    root->build();
}

void OctreeNode::insert(Hittable* obj) {
    objs.push_back(obj);
}

void OctreeNode::build() {
    if (objs.size() <= 8) return;
    Point center = bounds.min + (bounds.max - bounds.min) / 2.0;

    // Calcule os bounds dos 8 filhos
    Point min = bounds.min;
    Point max = bounds.max;

    // Dividimos o espaço 3D, definimos os limites de cada caixa
    // Filho 0: inferior, frente, esquerda
    children[0] = new OctreeNode(BoundingBox(min, center));
    // Filho 1: inferior, frente, direita
    children[1] = new OctreeNode(BoundingBox(Point(center.getX(), min.getY(), min.getZ()), Point(max.getX(), center.getY(), center.getZ())));
    // Filho 2: inferior, trás, esquerda
    children[2] = new OctreeNode(BoundingBox(Point(min.getX(), min.getY(), center.getZ()), Point(center.getX(), center.getY(), max.getZ())));
    // Filho 3: inferior, trás, direita
    children[3] = new OctreeNode(BoundingBox(Point(center.getX(), min.getY(), center.getZ()), Point(max.getX(), center.getY(), max.getZ())));
    // Filho 4: superior, frente, esquerda
    children[4] = new OctreeNode(BoundingBox(Point(min.getX(), center.getY(), min.getZ()), Point(center.getX(), max.getY(), center.getZ())));
    // Filho 5: superior, frente, direita
    children[5] = new OctreeNode(BoundingBox(Point(center.getX(), center.getY(), min.getZ()), Point(max.getX(), max.getY(), center.getZ())));
    // Filho 6: superior, trás, esquerda
    children[6] = new OctreeNode(BoundingBox(Point(min.getX(), center.getY(), center.getZ()), Point(center.getX(), max.getY(), max.getZ())));
    // Filho 7: superior, trás, direita
    children[7] = new OctreeNode(BoundingBox(center, max));

    // Objetos que são permanecerão com o pai
    vector<Hittable*> keepObjs;
    // for (auto& obj : objs) {
    //     BoundingBox objBox = obj->getBoundingBox();
    //     bool inserted = false;
    //     for (int i = 0; i < 8; ++i) {
    //         // Se o centro do objeto está no filho, insira lá.
    //         Point obj_center = objBox.min + (objBox.max - objBox.min) / 2.0;
    //         if (obj_center.getX() >= children[i]->bounds.min.getX() && obj_center.getX() < children[i]->bounds.max.getX() &&
    //             obj_center.getY() >= children[i]->bounds.min.getY() && obj_center.getY() < children[i]->bounds.max.getY() &&
    //             obj_center.getZ() >= children[i]->bounds.min.getZ() && obj_center.getZ() < children[i]->bounds.max.getZ())
    //         {
    //             children[i]->insert(obj);
    //             inserted = true;
    //             break;
    //         }
    //     }
    //     // Objetos muito grande que invadem várias caixas ou objetos infinitos 
    //     // como o plano ou tive o centro na fronteira entre duas caixas
    //     // são adicionados ao pai que é a caixa maiorzona
    //     if (!inserted) {
    //         keepObjs.push_back(obj);
    //     }
    // }
    for (auto& obj : objs) {
        if (obj->getName().find("guideOf") != string::npos) {
            continue;
        }
        
        BoundingBox objBox = obj->getBoundingBox();
        int targetChild = -1; // -1 significa que não encontrou um único filho
        int overlapCount = 0;

        // Verifica se a caixa do objeto está inclusa em mais de uma BoundingBox
        for (int i = 0; i < 8; ++i) {
            if (children[i]->bounds.partialInside(objBox)) {
                // Se entrar nesse if APENAS uma vez, targetChild irá guardar o filho que deve ficar com o obj
                // Se entrar nesse if MAIS de uma vez, overlapCount irá acusar e o obj ficará com o pai
                overlapCount++;
                targetChild = i; // Guarda o índice do último filho sobreposto
            }
        }

        // Se o objeto se sobrepõe a apenas 1 filho, move para lá.
        if (overlapCount == 1) {
            children[targetChild]->insert(obj);
        } else {
            // Se se sobrepõe a mais de 1
            // mantenha o objeto na lista do nó pai.
            keepObjs.push_back(obj);
        }
    }
    // Mantém objetos que cruzam múltiplos filhos no nó pai
    objs = keepObjs;

    // Construir as Octree dos filhos
    for (int i = 0; i < 8; ++i) {
        children[i]->build();
    }
}

HitRecord Octree::hit(const Ray& r) const {
    HitRecord closest_hit;
    hitNode(root, r, closest_hit);
    return closest_hit;
}

void Octree::hitNode(const OctreeNode* node, const Ray& r, HitRecord& closest_hit) const {
    // Se o raio não acertou nada ou não existe uma raiz, retorna
    if (!node || !node->bounds.hit(r)) {
        return;
    }

    // Tenta dar hit nos objetos neste nó
    for (const auto& obj : node->objs) {
        HitRecord rec = obj->hit(r);
        if (rec.t > t_min && rec.t < closest_hit.t) {
            closest_hit = rec;
        }
    }

    // Recursivamente testa os filhos
    for (int i = 0; i < 8; ++i) {
        if (node->children[i]) {
            hitNode(node->children[i], r, closest_hit);
        }
    }
}