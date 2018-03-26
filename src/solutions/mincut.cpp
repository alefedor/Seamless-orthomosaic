#include "solutions/mincut.h"
#include "basic/image.h"
#include "graph_cut.h"

static int getI(Image &a, int x, int y) {
    Pixel *p = a.getPixel(x, y);
    return p[0] + (int)p[1] + p[2];
}

static int calcEnergy(Image &a, int x1, int y1, int x2, int y2) {
    return abs(getI(a, x1, y1) - getI(a, x2, y2));
}

int dx[2] = {1, 0};
int dy[2] = {0, 1};

Seam getSeamMinCut(Image& a, Image& b) {
    Seam result;
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.getHeight(), b.top + b.getHeight()); // bottom not included
    int intersectionHeight = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.getWidth(), b.left + b.getWidth()); // right not included
    int intersectionWidth = intersectionRight - intersectionLeft;
    if (intersectionHeight < 1 || intersectionWidth < 1)
        return result; // no intersection => no Seam

    MaxFlow<std::pair<int, int>> maxFlow((intersectionWidth + 1) * (intersectionHeight + 1));

    for (int y = intersectionTop - 1; y <= intersectionBottom; y++)
        for (int x = intersectionLeft - 1; x <= intersectionRight; x++) {
            bool ina = a.inside(x, y);
            bool inb = b.inside(x, y);
            if (ina || inb)
            for (int i = 0; i < 2; i++) {
                int x2 = x + dx[i];
                int y2 = y + dy[i];
                bool ina2 = a.inside(x2, y2);
                bool inb2 = b.inside(x2, y2);
                if (ina && ina2 && inb && inb2) {
                    int energy = calcEnergy(a, x, y, x2, y2);
                    maxFlow.addEdge({x, y}, {x2, y2}, energy, energy);
                }
            }
        }
}