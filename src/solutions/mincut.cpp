#include "solutions/mincut.h"
#include "basic/image.h"
#include "graph_cut.h"
#include "energy/mincut_energy.h"

static int dx[2] = {1, 0};
static int dy[2] = {0, 1};

static unsigned int getId(int x, int y, int intersectionTop, int intersectionLeft, int intersectionWidth) {
    return (y - intersectionTop) * intersectionWidth + x - intersectionLeft;
}

Seam MinCut::getSeam(Image& a, Image& b) {
    Seam result;
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int intersectionHeight = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
    int intersectionWidth = intersectionRight - intersectionLeft;
    if (intersectionHeight < 1 || intersectionWidth < 1)
        return result; // no intersection => no Seam

    MinCutEnergy energy(a, b);

    MaxFlow<unsigned int> maxFlow(intersectionWidth * intersectionHeight);

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

                //just cases on how intersection area and pixels are connected
                if (ina && ina2 && inb && inb2) {
                    int e = energy.calcEnergy(x, y, x2, y2);
                    maxFlow.addEdge(id, id2, e, e);
                } else if (ina && !inb && inb2) {
                    int e = energy.calcEnergy(x, y, x2, y2);
                    maxFlow.addTweights(id2, e, 0);
                } else if (ina2 && !inb2 && inb) {
                    int e = energy.calcEnergy(x2, y2, x, y);
                    maxFlow.addTweights(id, e, 0);
                } else if (inb && !ina && ina2) {
                    int e = energy.calcEnergy(x2, y2, x, y);
                    maxFlow.addTweights(id2, 0, e);
                } else if (inb2 && !ina2 && ina) {
                    int e = energy.calcEnergy(x, y, x2, y2);
                    maxFlow.addTweights(id, 0, e);
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