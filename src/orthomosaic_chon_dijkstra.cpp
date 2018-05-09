#include "solutions/chon_dijkstra.h"
#include "runner/runner.h"

int main(int argnum, char** args) {
    Runner::run(argnum, args, ChonDijkstra(), "chon_dijkstra_");
    return 0;
}
