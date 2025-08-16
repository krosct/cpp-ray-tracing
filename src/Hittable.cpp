#include "../include/Hittable.h"
#include "../include/Operations.h"

// Comparação entre dois HitRecord
bool operator==(const HitRecord& a, const HitRecord& b) {
    if (a.t != b.t) { return false; }
    if (a.normal != b.normal) { return false; }
    if (a.hit_point != b.hit_point) { return false; }
    if (a.ka != b.ka) { return false; }
    if (a.kd != b.kd) { return false; }
    if (a.kr != b.kr) { return false; }
    if (a.ks != b.ks) { return false; }
    if (a.kt != b.kt) { return false; }
    if (a.shininess != b.shininess) { return false; }
    if (a.obj != b.obj) { return false; }
    return true;
}

bool operator!=(const HitRecord& a, const HitRecord& b) {
    return !(a == b);
}
