#pragma once

#include "solutions/seam_solver.h"

class ChonDijkstra final : public SeamSolver {
    Seam getSeam(Image& a, Image& b) final;
};