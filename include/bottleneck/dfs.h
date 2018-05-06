#pragma once

#include "basic/image.h"
#include <vector>
#include "energy/pixel_energy.h"

template<typename T>
bool hasPath(Image &a, Image &b, std::pair<int, int> start, std::pair<int, int> end, PixelEnergy<T> &energy, T limit);

static inline int mDist(std::pair<int, int> a, std::pair<int, int> b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

template <typename T>
static inline bool satisfy(int x, int y, std::pair<int, int> start, std::pair<int, int> end, PixelEnergy<T> &energy, T limit) {
    return mDist({x, y}, end) < 10 || mDist({x, y}, start) < 10 || energy.calcEnergy(x, y) <= limit;
}

template<typename T>
static bool dfs(Image &a, Image &b, std::pair<int, int> v, std::pair<int, int> &start, std::pair<int, int> &end,
                int left, int top, std::vector<std::vector<char> > &used, PixelEnergy<T> &energy, T limit) {
    if (v == end)
        return true;

    used[v.second - top][v.first - left] = true;

    static int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
    static int dy[8] = {1, -1, 0, 0, -1, 1, 1, -1};

    for (int i = 0; i < 8; i++) {
        int x = v.first + dx[i];
        int y = v.second + dy[i];
        if (a.inside(x, y) && b.inside(x, y) && !used[y - top][x - left] &&
            satisfy(x, y, start, end, energy, limit)) {
            if (dfs(a, b, {x, y}, start, end, left, top, used, energy, limit))
                return true;
        }
    }

    return false;
}

template<typename T>
bool hasPath(Image &a, Image &b, std::pair<int, int> start, std::pair<int, int> end, PixelEnergy<T> &energy, T limit) {
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int intersectionHeight = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
    int intersectionWidth = intersectionRight - intersectionLeft;

    std::vector<std::vector<char> > used;
    used.resize(intersectionHeight, std::vector<char>());
    for (auto &i : used)
        i.resize(intersectionWidth, false);

    return dfs(a, b, start, start, end, intersectionLeft, intersectionTop, used, energy, limit);
}