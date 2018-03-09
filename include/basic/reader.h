#ifndef __READER_H_INCLUDED
#define __READER_H_INCLUDED

#include <vector>
#include "basic/image.h"

class Reader {
 public:
    static void readRegionSize(int &width, int &height);
    static void readImages(std::vector<Image> &result);
};

#endif
