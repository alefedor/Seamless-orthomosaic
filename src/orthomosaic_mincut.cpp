#include "basic/reader.h"
#include "solutions/mincut.h"

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main(int argnum, char** args) {
    if (argnum != 5) {
        std::cout << "Usage: ./orthomosaic left top width height";
        return 0;
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

        Seam seam = getSeamMinCut(image, im);
        num++;

        imwrite("result" + to_string(num) + ".jpg", image.getImage());

        images.pop_back();
    }
    return 0;
}
