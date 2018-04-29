#pragma once

#include "basic/image.h"
#include "energy/energy.h"
#include "energy/pixel_energy.h"
#include <vector>

// Bottleneck Dijkstra. Returns pixels in Seam
template<typename T>
std::vector<std::pair<int, int>> diikstra(Image &a, Image &b, const std::vector<std::pair<int, int>> &start, const std::vector<std::pair<int, int>> &end, Energy<T> &energy, PixelEnergy<T> pixelEnergy, T limit);
