#pragma once

#include <vector>
#include "basic/image.h"

class Reader {
 public:
    static void readRegionSize(int &width, int &height);
    static void readImages(std::vector<Image> &result);
};

