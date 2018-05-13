#pragma once

#include "energy/pan_pixel_energy.h"
#include "energy/energy.h"

class PanDifferentialEnergy final : public Energy<double> {
public:
    // (x1, y1) and (x2, y2) are neighbouring (maybe by diagonal)
    double calcEnergy(int x1, int y1, int x2, int y2) final;
    PanDifferentialEnergy(PixelEnergy<double> &energy);

private:
    PixelEnergy<double> &energy;
};
