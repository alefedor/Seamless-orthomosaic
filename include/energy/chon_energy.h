#pragma once

#include "energy/energy.h"
#include "energy/chon_pixel_energy.h"

class ChonEnergy final : public Energy<double> {
public:
    double calcEnergy(int x1, int y1, int x2, int y2) final;
    ChonEnergy(Image &a, Image &b);

private:
    Image &a, &b;
    ChonPixelEnergy energy;
};
