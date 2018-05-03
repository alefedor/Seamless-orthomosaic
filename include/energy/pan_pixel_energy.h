#pragma once

#include "energy/pixel_energy.h"

class PanPixelEnergy : public PixelEnergy<double> {
 public:
    double calcEnergy(int x, int y) final;
    PanPixelEnergy(Image &a, Image &b, bool segmentation);

 private:
    Image &a;
    Image &b;
    bool segmented;
    cv::Mat image;
};
