#pragma once

#include "energy/energy.h"

/**
 * Manhattan dist between (b1, g1, r1) and (b2, g2, r2)
 */
class MinCutEnergyMht final : public Energy<int> {
public:
    int calcEnergy(int x1, int y1, int x2, int y2) final;
    MinCutEnergyMht(Image &a, Image &b);

private:
    Image &a, &b;
};
