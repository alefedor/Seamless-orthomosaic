#include "energy/pan_energy.h"

PanEnergy::PanEnergy(PixelEnergy<double> &energy) : energy(energy) {}

double PanEnergy::calcEnergy(int x1, int y1, int x2, int y2) {
     return fabs(energy.calcEnergy(x2, y2));
}
