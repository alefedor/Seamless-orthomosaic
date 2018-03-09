#ifndef __SEAM_H_INCLUDED
#define __SEAM_H_INCLUDED

#include <set>


struct Edge {
    int x;
    int y;
    bool dir; // 0 -- down, 1 -- right
    
    bool operator <(const Edge &e) const;
};

struct Seam {
    std::set<Edge> edges;
    
    bool has(const Edge &e);
};

Edge getEdge(int x1, int y1, int x2, int y2);

#endif
