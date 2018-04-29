#pragma once

#include "basic/image.h"

template <typename T>
class Energy {
 public:
    virtual T calcEnergy(int x1, int y1, int x2, int y2) = 0;
};