#pragma once
#include <vector>
#include <memory>
#include "../include/Hittable.h"
#include "../include/BoundingBox.h"

class OctreeNode {
public:
    BoundingBox bounds;
    std::vector<Hittable*> objs;
    OctreeNode* children[8];

    OctreeNode(const BoundingBox& b) : bounds(b) { for (auto& c:children) c=nullptr; }
    void insert(Hittable* obj);
    void build();
};

class Octree {
public:
    OctreeNode* root;

    Octree(const std::vector<Hittable*>& objs);
    HitRecord hit(const Ray& r) const;

private:
    void hitNode(const OctreeNode* node, const Ray& r, HitRecord& closest_hit) const;
};