#include "solutions/chon_dijkstra.h"
#include "runner/runner.h"

int main(int argnum, char** args) {
    Runner::run(argnum, args, ChonDijkstra(), "_chon_dijkstra");
    return 0;
}
