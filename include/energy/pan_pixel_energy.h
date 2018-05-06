#pragma once

#include "energy/pixel_energy.h"
#include "mean_shift.h"

class PanPixelEnergy : public PixelEnergy<double> {
 public:
    double calcEnergy(int x, int y) final;
    PanPixelEnergy(Image &a, Image &b, bool segmentation);

 private:
    double getI(Image &a, int x, int y);

    int top, left, width;
    Image &a;
    Image &b;
    bool segmented;
    vector<char> isPR;
    vector<int> labels;
};
