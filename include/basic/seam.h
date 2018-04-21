#pragma once

#include <set>

struct Edge {
    int x;
    int y;
    bool dir; // 0 -- down, 1 -- right
    
    bool operator <(const Edge e) const;
};

class Seam {
 public:
    std::set<Edge> edges;
 
    bool has(const Edge e);
    void addEdge(const Edge e);
    Seam();
    Seam(Seam && a);
};

Edge getEdge(int x1, int y1, int x2, int y2);
