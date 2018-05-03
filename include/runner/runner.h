#pragma once

#include "solutions/seam_solver.h"
#include <string>

namespace Runner {
    void run(int argnum, char** args, SeamSolver &&solver, std::string suffix);
}

