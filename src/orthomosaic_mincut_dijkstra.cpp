#include "solutions/mincut_dijkstra.h"
#include "runner/runner.h"

int main(int argnum, char** args) {
    Runner::run(argnum, args, MinCutDijkstra());
    return 0;
}
