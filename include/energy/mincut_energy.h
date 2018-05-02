#pragma once

#include "energy/pan_pixel_energy.h"
#include "energy/energy.h"

class MinCutEnergy final : public Energy<int> {
 public:
    int calcEnergy(int x1, int y1, int x2, int y2) final;
    MinCutEnergy(Image &a, Image &b);

 private:
    Image &a, &b;
};
