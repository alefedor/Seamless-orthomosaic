#pragma once

#include "basic/image.h"

template <typename T>
class PixelEnergy {
 public:
    virtual T calcEnergy(int x, int y) = delete;
};
