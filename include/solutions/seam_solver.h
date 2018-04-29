#pragma once

#include "basic/seam.h"
#include "basic/image.h"

class SeamSolver {
 public:
    virtual Seam getSeam(Image& a, Image& b) = 0;
};
