#include "energy/chon_energy.h"

ChonEnergy::ChonEnergy(Image &a, Image &b) : a(a), b(b), energy(a, b) {}

double ChonEnergy::calcEnergy(int x1, int y1, int x2, int y2) {
    return pow(energy.calcEnergy(x2, y2), 3);
}