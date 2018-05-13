#include "energy/pan_differential_energy.h"

PanDifferentialEnergy::PanDifferentialEnergy(PixelEnergy<double> &energy) : energy(energy) {}

double PanDifferentialEnergy::calcEnergy(int x1, int y1, int x2, int y2) {
    return fabs(energy.calcEnergy(x1, y1) - energy.calcEnergy(x2, y2));
}
