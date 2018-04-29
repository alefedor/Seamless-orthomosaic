#pragma once

#include "basic/image.h"
#include <vector>

template<typename T>
bool hasPath(Image &a, Image &b, std::pair<int, int> start, std::pair<int, int> end, PixelEnergy<T> &energy, T limit);
