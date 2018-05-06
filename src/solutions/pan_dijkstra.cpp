#include "solutions/pan_dijkstra.h"
#include "bottleneck/dijkstra.h"
#include "bottleneck/dfs.h"
#include "energy/pan_energy.h"
#include "energy/pan_pixel_energy.h"
#include <set>
#include <vector>

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
    //to add: segmentation to PanPixelEnergy

    PanPixelEnergy pixelEnergy(a, b, true);
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

    //VISUALISATION

    cv::Mat image = cv::Mat::zeros(cv::Size(intersectionWidth, intersectionHeight), CV_8UC4);
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

    cv::imwrite("result_bottleneck_area.jpg", image);

    //VISUALISATION

    std::vector<std::pair<int, int>> pixels = dijkstra(a, b, start, end, energy, pixelEnergy, r);
    for (auto &a : pixels)
        for (int i = 0; i < 4; i++) { // first 4 are without diagonals
            result.addEdge(getEdge(a.first, a.second, a.first + dx[i], a.second + dy[i]));
        }

    return result;
}