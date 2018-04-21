#include "solutions/mincut.h"
#include "basic/image.h"
#include "graph_cut.h"

static int getI(Image &a, int x, int y) {
    Pixel *p = a.getPixel(x, y);
    return p[0] + (int)p[1] + p[2];
}

static int calcEnergy(Image &a, Image &b, int x1, int y1, int x2, int y2) {
    return (abs(getI(a, x1, y1) - getI(b, x2, y2)) + abs(getI(b, x1, y1) - getI(a, x2, y2))) / 2;
}

// (x1, y1) in a and not in b, (x2, y2) in b
static int calcBorderEnergy(Image &a, Image&b, int x1, int y1, int x2, int y2) {
    return abs(getI(a, x1, y1) - getI(b, x2, y2));
}

static int dx[2] = {1, 0};
static int dy[2] = {0, 1};

static unsigned int getId(int x, int y, int intersectionTop, int intersectionLeft, int intersectionWidth) {
    return (y - intersectionTop) * intersectionWidth + x - intersectionLeft;
}

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

    MaxFlow<unsigned int> maxFlow((intersectionWidth + 2) * (intersectionHeight + 2));

    //  source = image a, sink = image b

    for (int y = intersectionTop; y < intersectionBottom; y++)
        for (int x = intersectionLeft; x < intersectionRight; x++) {
            int id = getId(x, y, intersectionTop, intersectionLeft, intersectionWidth);
            maxFlow.addNode(id);
        }

    for (int y = intersectionTop - 1; y < intersectionBottom; y++)
        for (int x = intersectionLeft - 1; x < intersectionRight; x++) {
            bool ina = a.inside(x, y);
            bool inb = b.inside(x, y);
            if (!ina && !inb)
                continue;

            int id = getId(x, y, intersectionTop, intersectionLeft, intersectionWidth);
            for (int i = 0; i < 2; i++) {
                int x2 = x + dx[i];
                int y2 = y + dy[i];
                int id2 = getId(x2, y2, intersectionTop, intersectionLeft, intersectionWidth);
                bool ina2 = a.inside(x2, y2);
                bool inb2 = b.inside(x2, y2);
                if (ina && ina2 && inb && inb2) {
                    int energy = calcEnergy(a, b, x, y, x2, y2);
                    maxFlow.addEdge(id, id2, energy, energy);
                } else if (ina && !inb && inb2) {
                    int energy = calcBorderEnergy(a, b, x, y, x2, y2);
                    maxFlow.addTweights(id2, energy, 0);
                } else if (ina2 && !inb2 && inb) {
                    int energy = calcBorderEnergy(a, b, x2, y2, x, y);
                    maxFlow.addTweights(id, energy, 0);
                } else if (inb && !ina && ina2) {
                    int energy = calcBorderEnergy(b, a, x, y, x2, y2);
                    maxFlow.addTweights(id2, 0, energy);
                } else if (inb2 && !ina2 && ina) {
                    int energy = calcBorderEnergy(b, a, x2, y2, x, y);
                    maxFlow.addTweights(id, 0, energy);
                }
            }
        }

    maxFlow.computeMaxFlow();

    for (int y = intersectionTop - 1; y < intersectionBottom; y++)
        for (int x = intersectionLeft - 1; x < intersectionRight; x++) {
            bool ina = a.inside(x, y);
            bool inb = b.inside(x, y);
            if (!ina && !inb)
                continue;

            int id = getId(x, y, intersectionTop, intersectionLeft, intersectionWidth);
            for (int i = 0; i < 2; i++) {
                int x2 = x + dx[i];
                int y2 = y + dy[i];
                int id2 = getId(x2, y2, intersectionTop, intersectionLeft, intersectionWidth);
                bool ina2 = a.inside(x2, y2);
                bool inb2 = b.inside(x2, y2);
                if (ina && ina2 && inb && inb2) {
                    if (maxFlow.isNodeOnSrcSide(id) != maxFlow.isNodeOnSrcSide(id2)) {
                        result.addEdge(getEdge(x, y, x2, y2));
                    }
                } else if (ina && !inb && inb2) {
                    if (!maxFlow.isNodeOnSrcSide(id2)) {
                        result.addEdge(getEdge(x, y, x2, y2));
                    }
                } else if (ina2 && !inb2 && inb) {
                    if (!maxFlow.isNodeOnSrcSide(id)) {
                        result.addEdge(getEdge(x, y, x2, y2));
                    }
                } else if (inb && !ina && ina2) {
                    if (maxFlow.isNodeOnSrcSide(id2)) {
                        result.addEdge(getEdge(x, y, x2, y2));
                    }
                } else if (inb2 && !ina2 && ina) {
                    if (maxFlow.isNodeOnSrcSide(id)) {
                        result.addEdge(getEdge(x, y, x2, y2));
                    }
                }
            }
        }

    return result;
}