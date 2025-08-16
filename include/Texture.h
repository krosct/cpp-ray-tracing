#pragma once
#include "Vector.h"
#include "Ray.h"
#include <string>
#include <vector>
#include <utility>

class Texture {
public:
    Texture(const std::string& fileName);
    Vector value(std::pair<double, double> p) const;
    void toPpm(const std::string& fileName);
    std::pair<double, double> getTexelSize() const;
private:
    std::vector<unsigned char> data;
    int width, height;
};


