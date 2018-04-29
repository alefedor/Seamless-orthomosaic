#include "energy/pan_pixel_energy.h"

PanPixelEnergy::PanPixelEnergy(Image &a, Image &b) : a(a), b(b) {};

static int getI(Image &a, int x, int y) {
    Pixel* p = a.getPixel(x, y);
    return p[0] + p[1] + p[2];
}

double PanPixelEnergy::calcEnergy(int x, int y) {
    return abs(getI(a, x, y) - getI(b, x, y));
}