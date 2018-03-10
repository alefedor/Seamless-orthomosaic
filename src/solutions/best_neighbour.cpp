#include "solutions/best_neighbour.h"

static double getDist(Image &a, int x, int y) {
    return sqrt((x - a.centerX) * (double)(x - a.centerX) + (y - a.centerY) * (double)(y - a.centerY));
}

static bool better(Image &a, Image &b, int x, int y, std::vector<std::vector<double>>& dist) {
    return dist[y - a.top][x - a.left] < getDist(b, x, y);
}

// true = first, false = second
static bool getBestVariant(Image &a, Image &b, int x, int y, std::vector<std::vector<double>>& dist) {
    return (!b.inside(x, y) || (a.inside(x, y) && better(a, b, x, y, dist)));
}

static bool inSeam(Image &a, Image &b, int x1, int y1, int x2, int y2, std::vector<std::vector<double>>& dist) {
    if ((!a.inside(x1, y1) && !b.inside(x1, y1)) ||
        (!a.inside(x2, y2) && !b.inside(x2, y2)))
        return false;
    return getBestVariant(a, b, x1, y1, dist) ^ getBestVariant(a, b, x2, y2, dist); //in Seam if different variants
}

Seam getSeamBestNeighbour(Image& a, Image& b, std::vector<std::vector<double>>& dist) {
    Seam result;
    int intersectionTop = std::max(a.top, b.top);
    int intersectionBottom = std::min(a.top + a.getHeight(), b.top + b.getHeight()); // bottom not included
    int intersectionHeight = intersectionBottom - intersectionTop;
    int intersectionLeft = std::max(a.left, b.left);
    int intersectionRight = std::min(a.left + a.getWidth(), b.left + b.getWidth()); // right not included
    int intersectionWidth = intersectionRight - intersectionLeft;
    if (intersectionHeight < 1 || intersectionWidth < 1)
        return result; // no intersection => no Seam
        
    for (int y = intersectionTop - 1; y < intersectionBottom; y++) 
        for (int x = intersectionLeft - 1; x < intersectionRight; x++) { // for in intersection + left column + top row
            if (inSeam(a, b, x, y, x + 1, y, dist)) 
                result.addEdge(getEdge(x, y, x + 1, y));
            if (inSeam(a, b, x, y, x, y + 1, dist)) 
                result.addEdge(getEdge(x, y, x, y + 1));
    }
    
    for (int y = intersectionTop; y < intersectionBottom; y++) 
        for (int x = intersectionLeft; x < intersectionRight; x++)
            if (b.inside(x, y))
                dist[y - a.top][x - a.left] = std::min(dist[y - a.top][x - a.left], getDist(b, x, y));
    
    return result;
}

std::vector<std::vector<double>> initialBestNeighbourDist(int height, int width) {
    std::vector<std::vector<double>> dist;
    dist.resize(height, std::vector<double>());
    for (int i = 0; i < (int)dist.size(); i++)
        dist[i].resize(width, 1e10);
    return dist;
}
