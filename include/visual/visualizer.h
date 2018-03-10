#ifndef __VISUALIZER_H_INCLUDED
#define __VISUALIZER_H_INCLUDED

#include "basic/image.h"

class Visualizer {
 public:
    static void showSeam(Image &a, Seam &s);
    static void markImage(Image &a, int id);
};

#endif
