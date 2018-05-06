#pragma once

#include "energy/pixel_energy.h"


class ChonPixelEnergy : public PixelEnergy<double> {
public:
    double calcEnergy(int x, int y) final;
    ChonPixelEnergy(Image &a, Image &b);

private:
    Image &a;
    Image &b;
};
