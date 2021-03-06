#include "energy/mincut_energy.h"

static inline int getI(Image &a, int x, int y) {
    Pixel *p = a.getPixel(x, y);
    return p[0] + (int)p[1] + p[2];
}

static inline int calcRegularEnergy(Image &a, Image &b, int x1, int y1, int x2, int y2) {
    return abs(getI(a, x1, y1) - getI(b, x2, y2));
}

// (x1, y1) in a and not in b, (x2, y2) in b
static inline int calcBorderEnergy(Image &a, Image&b, int x1, int y1, int x2, int y2) {
    return abs(getI(a, x1, y1) - getI(b, x2, y2));
}

MinCutEnergy::MinCutEnergy(Image &a, Image &b) : a(a), b(b) {}

int MinCutEnergy::calcEnergy(int x1, int y1, int x2, int y2) {
    if (!b.inside(x1, y1) || !a.inside(x2, y2))
        return calcBorderEnergy(a, b, x1, y1, x2, y2);
    if (!a.inside(x1, y1) || !b.inside(x2, y2))
        return calcBorderEnergy(a, b, x2, y2, x1, y1);

    return calcRegularEnergy(a, b, x1, y1, x2, y2);
}