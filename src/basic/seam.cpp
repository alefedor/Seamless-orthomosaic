#include "basic/seam.h"

bool Edge::operator < (const Edge& e) const {
    if (x < e.x)
        return true;
    if (x > e.x)
        return false;
    if (y < e.y)
        return true;
    if (y > e.y)
        return false;
    return (dir < e.dir);     
}

bool Seam::has(const Edge &e) {
    return edges.count(e);
}

Edge getEdge(int x1, int y1, int x2, int y2) {
    if (x1 > x2) {
        std::swap(x1, x2);
    } else if (y1 > y2) {
        std::swap(y1, y2);
    }
    if (x1 < x2) 
        return Edge{x1, y1, 1};
    else
        return Edge{x1, y1, 0};
}
