#pragma once
#include "basic/image.h"

class Visualizer {
 public:
    static void showSeam(Image &a, Seam &s);
    static void markImage(Image &a, int id);
};

