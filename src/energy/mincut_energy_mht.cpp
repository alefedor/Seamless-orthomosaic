#include "energy/mincut_energy_mht.h"

static inline int mhtDist(Pixel *a, Pixel *b) {
    return abs(a[0] - b[0]) + abs(a[1] - b[1]) + abs(a[2] - b[2]);
}

static inline int calcRegularEnergy(Image &a, Image &b, int x1, int y1, int x2, int y2) {
    return mhtDist(a.getPixel(x1, y1), b.getPixel(x2, y2));
}

// (x1, y1) in a and not in b, (x2, y2) in b
static inline int calcBorderEnergy(Image &a, Image&b, int x1, int y1, int x2, int y2) {
    return mhtDist(a.getPixel(x1, y1), b.getPixel(x2, y2));
}

MinCutEnergyMht::MinCutEnergyMht(Image &a, Image &b) : a(a), b(b) {}

int MinCutEnergyMht::calcEnergy(int x1, int y1, int x2, int y2) {
    if (!b.inside(x1, y1) || !a.inside(x2, y2))
        return calcBorderEnergy(a, b, x1, y1, x2, y2);
    if (!a.inside(x1, y1) || !b.inside(x2, y2))
        return calcBorderEnergy(a, b, x2, y2, x1, y1);

    return calcRegularEnergy(a, b, x1, y1, x2, y2);
}