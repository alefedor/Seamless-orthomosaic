#include "solutions/pan_dijkstra.h"
#include "bottleneck/dijkstra.h"
#include "bottleneck/dfs.h"
#include "energy/pan_energy.h"
#include "energy/pan_pixel_energy.h"
#include <set>
#include <vector>
#include <iostream>

#include <opencv2/imgproc/imgproc.hpp> // for bottleneck


static int dx[8] = {0, 0, 1, -1, 1, 1, -1, -1};
static int dy[8] = {1, -1, 0, 0, -1, 1, 1, -1};

static const int iterCount = 12;

static bool hasNear(std::pair<int, int> &a, std::vector<std::pair<int, int>> &v) {
    for (auto &b : v)
        if (abs(a.first - b.first) + abs(a.second - b.second) < 20)
            return true;
    return false;
}

// works only when intersection area is a single component
Seam PanDijkstra::getSeam(Image& a, Image& b) {
    Seam result;
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int intersectionHeight = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
    int intersectionWidth = intersectionRight - intersectionLeft;
    if (intersectionHeight < 1 || intersectionWidth < 1)
        return result; // no intersection => no Seam

    std::vector<std::pair<int, int>> intr;

    for (int y = intersectionTop; y < intersectionBottom; y++)
        for (int x = intersectionLeft; x < intersectionRight; x++)
            if (a.inside(x, y) && b.inside(x, y)) {
                bool good = false;
                for (int dy = -1; dy <= 1; dy++)
                    for (int dx = -1; dx <= 1; dx++){
                        int xx = x + dx;
                        int yy = y + dy;
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
        std::cout << "Diffucult area of intersection is ignored (photo " + b.filename + ")\n";
        result.addEdge(getEdge(0, 0, 0, 1)); //crutch to show that ignored
        return result;
    }

    PanPixelEnergy pixelEnergy(a, b, true);
    PanEnergy energy(pixelEnergy);

    std::pair<int, int> start = intr[0];
    std::pair<int, int> end = intr[1];

    double l = 0, r = 800;
    int interCnt = iterCount;

    while (interCnt--) {
        double m = (l + r) / 2;
        if (hasPath(a, b, start, end, pixelEnergy, m))
            r = m;
        else
            l = m;
    }

    #ifdef VISUALISATION

    cv::Mat image = cv::Mat::zeros(cv::Size(intersectionWidth, intersectionHeight), CV_8UC4);
    #pragma omp parallel for
    for (int y = intersectionTop; y < intersectionBottom; y++)
        for (int x = intersectionLeft; x < intersectionRight; x++)
            if (a.inside(x, y) && b.inside(x, y))
                if (pixelEnergy.calcEnergy(x, y) <= r) {
                    Pixel* to = image.data + (image.type() == CV_8UC4 ? 4 : 3) * ((y - intersectionTop) * intersectionWidth + x - intersectionLeft);
                    Pixel* from = b.getPixel(x, y);
                    if (image.type() == CV_8UC4)
                        to[3] = 255;

                    for (int i = 0; i < 3; i++)
                        to[i] = from[i];
                }

    cv::imwrite("pan_dijkstra_bottleneck_area_result.jpg", image);

    #endif


    std::vector<std::pair<int, int>> pixels = dijkstra(a, b, start, end, energy, pixelEnergy, r);
    for (auto &a : pixels)
        for (int i = 0; i < 4; i++) { // first 4 are without diagonals
            result.addEdge(getEdge(a.first, a.second, a.first + dx[i], a.second + dy[i]));
        }

    return result;
}