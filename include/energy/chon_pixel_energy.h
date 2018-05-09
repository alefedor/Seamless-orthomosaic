#pragma once

#include "energy/pixel_energy.h"
#include <vector>

class ChonPixelEnergy : public PixelEnergy<double> {
public:
    double calcEnergy(int x, int y) final;
    ChonPixelEnergy(Image &a, Image &b);

private:
    Image &a;
    Image &b;
    int top, left;
    std::vector<std::vector<double>> energy;
};
