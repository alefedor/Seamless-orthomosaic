#pragma once

#include "energy/pixel_energy.h"
#include "mean_shift.h"
#include <vector>

class PanPixelEnergy : public PixelEnergy<double> {
 public:
    double calcEnergy(int x, int y) final;
    PanPixelEnergy(Image &a, Image &b, bool segmentation);

 private:
    int getI(Image &a, int x, int y);

    int top, left, width;
    Image &a;
    Image &b;
    bool segmented;
    std::vector<std::vector<double>> energy;
};
