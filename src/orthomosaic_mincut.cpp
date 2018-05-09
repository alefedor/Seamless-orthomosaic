#include "solutions/mincut.h"
#include "runner/runner.h"

int main(int argnum, char** args) {
    Runner::run(argnum, args, MinCut(), "mincut_");
    return 0;
}
