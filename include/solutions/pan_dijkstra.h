#pragma once

#include "solutions/seam_solver.h"

class PanDijkstra final : SeamSolver {
    Seam getSeam(Image& a, Image& b) final;
};