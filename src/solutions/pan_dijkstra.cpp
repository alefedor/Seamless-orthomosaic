#include "solutions/pan_dijkstra.h"
#include "graph/dijkstra.h"
#include <graph/dfs.h>
#include "energy/pan_energy.h"
#include "energy/pan_pixel_energy.h"
#include <set>
#include <vector>

static int dx[8] = {0, 0, 1, 1, 1, -1, -1, -1};
static int dy[8] = {1, -1, 0, 1, -1, 0, 1, -1};

static const int iterCount = 15;

// works only when intersection area is a single component
Seam PanDijkstra::getSeam(Image& a, Image& b) {
    //to add: segmentation to PanPixelEnergy

    PanPixelEnergy pixelEnergy(a, b);
    PanEnergy energy(pixelEnergy);

    Seam result;
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int intersectionHeight = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
    int intersectionWidth = intersectionRight - intersectionLeft;
    if (intersectionHeight < 1 || intersectionWidth < 1)
        return result; // no intersection => no Seam

    std::set<std::pair<int, int>> intr;

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
                    intr.insert({x, y});
                }
            }

    if (intr.empty()) {
        return result; // one image is in another => no seam is good seam
    }

    std::vector<std::pair<int, int>> start;
    std::vector<std::pair<int, int>> end;

    find_component(intr.begin() -> first, intr.begin() -> second, intr, start);

    if (intr.empty()) {
        return result; // one image is in another => no seam is good seam
    }

    find_component(intr.begin() -> first, intr.begin() -> second, intr, end);

    if (!intr.empty()) {
        throw std::runtime_error("Difficult areas of intersection not supported yet (single component required)");
    }

    double l = 0, r = 1000;
    int interCnt = iterCount;

    while (interCnt--) {
        double m = (l + r) / 2;
        if (hasPath(a, b, *start.begin(), *end.begin(), pixelEnergy, m))
            r = m;
        else
            l = m;
    }


}