#include "runner/runner.h"
#include "basic/reader.h"
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <visual/visualizer.h>

using namespace std;
using namespace cv;

void Runner::run(int argnum, char **args, SeamSolver &&solver) {
    if (argnum != 5) {
        cout << "Usage: ./orthomosaic left top width height";
        return;
    }

    int left = atoi(args[1]);
    int top = atoi(args[2]);
    int width = atoi(args[3]);
    int height = atoi(args[4]);
    int PYR_DOWN_COUNT = 1;

    //Reader::readRegionSize(width, height);

    Image image = Image(left, top, width, height);

    vector<Image> images;
    Reader::readImages(images);

    int num = 0;
    while (!images.empty()) {
        Image &im = images.back();
        if (max(image.top, im.top) >= min(image.top + image.height, im.top + im.height) ||
            max(image.left, im.left) >= min(image.left + image.width, im.left + im.width)) {
            images.pop_back();
            continue;
        }


        Seam seam = solver.getSeam(image, im);


        Image copySeam = image.clone();
        Image copyMark = image.clone();


        image.combine(im, seam);

        copySeam.combine(im, seam);
        Visualizer::showSeam(copySeam, seam);
        Visualizer::markImage(im, 1);
        copyMark.combine(im, seam);
        num++;

        imwrite("result" + to_string(num) + ".jpg", image.getImage());
        imwrite("result_seam" + to_string(num) + ".jpg", copySeam.getImage());
        imwrite("result_mark" + to_string(num) + ".jpg", copyMark.getImage());

        images.pop_back();
    }
}