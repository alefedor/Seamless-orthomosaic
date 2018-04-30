#pragma once

#include "basic/image.h"
#include "energy/energy.h"
#include "energy/pixel_energy.h"
#include <vector>
#include <queue>

// Bottleneck Dijkstra. Returns pixels in path
template<typename T>
std::vector<std::pair<int, int>> dijkstra(Image &a, Image &b, std::pair<int, int> start, std::pair<int, int> end, Energy<T> &energy, PixelEnergy<T> &pixelEnergy, T limit);

static inline int mDist(std::pair<int, int> a, std::pair<int, int> b);

// should be equal to that in dfs.cpp
template <typename T>
static inline bool satisfy(int x, int y, std::pair<int, int> start, std::pair<int, int> end, PixelEnergy<T> &energy, T limit);

template<typename T>
static inline T getDist(std::vector<std::vector<T>> &dist, std::pair<int, int> p, int left, int top) {
    return dist[p.second - top][p.first - left];
}

template<typename T>
std::vector<std::pair<int, int>> dijkstra(Image &a, Image &b, std::pair<int, int> start, std::pair<int, int> end,
                                          Energy<T> &energy, PixelEnergy<T> &pixelEnergy, T limit) {
    int top = std::max(a.top, b.top);
    int bottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int height = bottom - top;
    int left = std::max(a.left, b.left);
    int right = std::min(a.left + a.width, b.left + b.width); // right not included
    int width = right - left;

    std::vector<std::vector<std::pair<int, int>>> par;
    par.resize(height, std::vector<std::pair<int, int>>());
    for (auto &i : par)
        i.resize(width, {-1, -1});

    T inf = std::numeric_limits<T>::max();
    std::vector<std::vector<T>> dist;
    dist.resize(height, std::vector<T>());
    for (auto &i : dist)
        i.resize(width, inf);

    dist[start.second - top][start.first - left] = 0;

    std::priority_queue<std::pair<T, std::pair<int, int>>, std::vector<std::pair<T, std::pair<int, int>>>,
            std::greater<std::pair<T, std::pair<int, int>>>> st;
    st.push({0, start});

    T lastDist = 0;

    static int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
    static int dy[8] = {1, -1, 0, 0, -1, 1, 1, -1};

    while (!st.empty()) {
        while (!st.empty() && getDist(dist, st.top().second, left, top) < lastDist) {
            st.pop(); // this pixel has already been
        }

        if (st.empty())
            break;


        std::pair<int, int> p = st.top().second;
        st.pop();
        if (p == end)
            break; // no need for other calcs

        lastDist = getDist(dist, p, left, top);

        for (int i = 0; i < 8; i++) {
            int x = p.first + dx[i];
            int y = p.second + dy[i];
            if (a.inside(x, y) && b.inside(x, y) && satisfy(x, y, start, end, pixelEnergy, limit)) {
                T cost = energy.calcEnergy(p.first, p.second, x, y);
                if (getDist(dist, {x, y}, left, top) > lastDist + cost) {
                    dist[y - top][x - left] = lastDist + cost;
                    par[y - top][x - left] = p;
                    st.push({dist[y - top][x - left], std::make_pair(x, y)});
                }
            }
        }
    }

    if (getDist(dist, end, left, top) == inf) {
        throw std::runtime_error("No path found");
    }

    std::vector<std::pair<int, int>> result;
    std::pair<int, int> cur = end;
    while (cur.first != -1) {
        result.push_back(cur);
        cur = par[cur.second - top][cur.first - left];
    }

    return result;
};