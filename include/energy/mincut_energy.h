#pragma once

#include "energy/energy.h"

/**
 * |I_1 - I_2|
 */
class MinCutEnergy final : public Energy<int> {
 public:
    int calcEnergy(int x1, int y1, int x2, int y2) final;
    MinCutEnergy(Image &a, Image &b);

 private:
    Image &a, &b;
};
