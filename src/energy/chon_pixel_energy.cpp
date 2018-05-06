#include "energy/chon_pixel_energy.h"


ChonPixelEnergy::ChonPixelEnergy(Image &a, Image &b) : a(a), b(b) {}

const int frameSize = 2;

static inline int getI(Image &a, int x, int y) {
    Pixel* p = a.getPixel(x, y);
    return (p[0] + (int)p[1] + p[2]);
}

static inline double getAverage(Image &a, Image &b, int x, int y) {
    double sum = 0;
    int num = 0;
    for (int dy = -frameSize; dy <= frameSize; dy++)
        for (int dx = -frameSize; dx <= frameSize; dx++)
            if (a.inside(x + dx, y + dy) && b.inside(x + dx, y + dy)) {
                num++;
                sum += getI(a, x + dx, y + dy);
            }
    return sum / num;
}

static inline double getNCC(Image &a, Image &b, int x, int y) {
    double af = getAverage(a, b, x, y); //ZNCC
    double ag = getAverage(b, a, x, y);

    //double af = 0; //NCC
    //double ag = 0;

    double result = 0;
    double sumf = 0;
    double sumg = 0;
    for (int dy = -frameSize; dy <= frameSize; dy++)
        for (int dx = -frameSize; dx <= frameSize; dx++)
            if (a.inside(x + dx, y + dy) && b.inside(x + dx, y + dy)) {
                double df = (getI(a, x + dx, y + dy) - af);
                double dg = (getI(b, x + dx, y + dy) - ag);
                result +=  df * dg;
                sumf += df * df;
                sumg += dg * dg;
            }

    if (sumf == 0 || sumg == 0)
        return 1;

    return result / sqrt(sumf) / sqrt(sumg);
}

double ChonPixelEnergy::calcEnergy(int x, int y) {
    return (1 - getNCC(a, b, x, y)) / 2.0;
}