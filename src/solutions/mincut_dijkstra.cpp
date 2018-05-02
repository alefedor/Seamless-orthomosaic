#include "solutions/mincut_dijkstra.h"
#include <graph/dfs.h>
#include <set>
#include <vector>

#include <opencv2/imgproc/imgproc.hpp> // for bottleneck
#include <queue>
#include <energy/mincut_energy.h>


static int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
static int dy[8] = {1, -1, 0, 0, -1, 1, 1, -1};


static bool hasNear(std::pair<int, int> &a, std::vector<std::pair<int, int>> &v) {
    for (auto &b : v)
        if (abs(a.first - b.first) + abs(a.second - b.second) < 20)
            return true;
    return false;
}

static inline int getDist(std::vector<std::vector<int>> &dist, std::pair<int, int> p, int left, int top) {
    return dist[p.second - top][p.first - left];
}

static inline void getNormal(int x1, int y1, int x2, int y2, int &ax1, int &ay1, int &ax2, int &ay2) {
    if (x1 == x2) {
        ay1 = ay2 = std::min(y1, y2);
        ax1 = x1;
        ax2 = x1 - 1;
    } else {
        ax1 = ax2 = std::min(x1, x2);
        ay1 = y1;
        ay2 = y1 - 1;
    }
}

static inline int calcNormalEnergy(Energy<int> &energy, int x1, int y1, int x2, int y2) {
    int ax1, ay1, ax2, ay2;
    getNormal(x1, y1, x2, y2, ax1, ay1, ax2, ay2);
    return energy.calcEnergy(ax1, ay1, ax2, ay2);
}

static inline bool canNormal(Image &a, Image &b, int x1, int y1, int x2, int y2) {
    int ax1, ay1, ax2, ay2;
    getNormal(x1, y1, x2, y2, ax1, ay1, ax2, ay2);
    return (a.inside(ax1, ay1) && b.inside(ax1, ay1) && (a.inside(ax2, ay2) || b.inside(ax2, ay2))) ||
            (a.inside(ax2, ay2) && b.inside(ax2, ay2) && (a.inside(ax1, ay1) || b.inside(ax1, ay1)));
}

// x--o
// |  |    pixel(x, y)
// o--o

// works only when intersection area is a single component
Seam MinCutDijkstra::getSeam(Image& a, Image& b) {
    Seam result;
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int height = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
    int width = intersectionRight - intersectionLeft;
    if (height < 1 || width < 1)
        return result; // no intersection => no Seam

    std::vector<std::pair<int, int>> intr;

    for (int y = intersectionTop; y < intersectionBottom; y++)
        for (int x = intersectionLeft; x < intersectionRight; x++)
            if (a.inside(x, y) && b.inside(x, y)) {
                bool good = false;
                for (int i = 0; i < 8; i++) {
                    int xx = x + dx[i];
                    int yy = y + dy[i];
                    if (!a.inside(xx, yy) && !b.inside(xx, yy)) {
                        good = true;
                        break;
                    }
                }

                if (good) {
                    std::pair<int, int> p = {x, y};
                    if (!hasNear(p, intr))
                        intr.push_back(p);
                }
            }

    if (intr.size() < 2) {
        return result; // one image is in another => no seam is good seam
    }

    if (intr.size() > 2) {
        throw std::runtime_error("Difficult areas of intersection not supported yet (single component required). "
                                 + std::to_string(intr[0].first) + " "
                                 + std::to_string(intr[0].second) + "; "
                                 + std::to_string(intr[1].first) + " "
                                 + std::to_string(intr[1].second) + "; "
                                 + std::to_string(intr[2].first) + " "
                                 + std::to_string(intr[2].second) + "; ");
    }

    MinCutEnergy energy(a, b);

    for (auto &c : intr) { // changing coordinates
        for (int i = 0; i < 8; i++) {
            int xx = c.first + dx[i];
            int yy = c.second + dy[i];
            if (!a.inside(xx, yy) && !b.inside(xx, yy)) {
                if (dx[i] > 0)
                    c.first++;
                if (dy[i] > 0)
                    c.second++;
            }

            break;
        }
    }

    std::pair<int, int> start = intr[0];
    std::pair<int, int> end = intr[1];


    std::vector<std::vector<std::pair<int, int>>> par;
    par.resize(height + 1, std::vector<std::pair<int, int>>());
    for (auto &i : par)
        i.resize(width + 1, {-1, -1});

    int inf = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist;
    dist.resize(height, std::vector<int>());
    for (auto &i : dist)
        i.resize(width, inf);

    int top = intersectionTop;
    int left = intersectionLeft;

    dist[start.second - top][start.first - left] = 0;

    std::priority_queue<std::pair<int, std::pair<int, int>>, std::vector<std::pair<int, std::pair<int, int>>>,
            std::greater<std::pair<int, std::pair<int, int>>>> st;
    st.push({0, start});

    int lastDist = 0;

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
            if (canNormal(a, b, p.first, p.second, x, y)) {
                int cost = calcNormalEnergy(energy, p.first, p.second, x, y);
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

    std::vector<std::pair<int, int>> path;
    std::pair<int, int> cur = end;
    while (cur.first != -1) {
        path.push_back(cur);
        cur = par[cur.second - top][cur.first - left];
    }

    for (int i = 1; i < path.size(); i++) {
        int ax1, ay1, ax2, ay2;
        getNormal(path[i - 1].first, path[i - 1].second, path[i].first, path[i].second, ax1, ay1, ax2, ay2);
        result.addEdge(getEdge(ax1, ay1, ax2, ay2));
    }

    return result;
}