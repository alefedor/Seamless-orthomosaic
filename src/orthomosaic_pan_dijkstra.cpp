#include "solutions/pan_dijkstra.h"
#include "runner/runner.h"

int main(int argnum, char** args) {
    Runner::run(argnum, args, PanDijkstra(), "_pan_dijkstra");
    return 0;
}
