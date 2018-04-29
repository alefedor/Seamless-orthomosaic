#pragma once

#include "basic/image.h"

// F = T(*)(int, int, Image, Image)
template<typename T>
T diikstra(Image &a, Image &b, vector<pair<int, int>> &start, vector<pair<int, int>> &end, Energy<T> energy);
