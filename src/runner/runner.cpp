#include "runner/runner.h"
#include "basic/reader.h"
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>
#include <visual/visualizer.h>

using namespace std;
using namespace cv;

static inline bool isWhite(Pixel* pixel) {
    return pixel[0] >= 250 && pixel[1] >= 250 && pixel[2] >= 250;
}

static int dx[4] = {1, -1, 0, 0};
static int dy[4] = {0, 0, 1, -1};

static int iLeft;
static int iRight;
static int iTop;
static int iBottom;

static inline bool insideArea(int x, int y) {
    return iLeft <= x && x < iRight && iTop <= y && y < iBottom;
}

static void eliminateWhite(int x, int y, vector<vector<char>> &used, Image &image) {
    Pixel *to = image.getPixel(x, y);
    for (int i = 0; i < 3; i++)
        to[i] = magic[i];

    used[y - iTop][x - iLeft] = true;

    for (int i = 0; i < 4; i++) {
        int xx = x + dx[i];
        int yy = y + dy[i];
        if (image.inside(x, y) && insideArea(x, y) &&!used[yy - iTop][xx - iLeft] && isWhite(image.getPixel(x, y)))
            eliminateWhite(xx, yy, used, image);
    }
}

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

        im.loadImage();

        // WHITE ELIMINATION

        int intersectionTop = std::max(image.top, im.top);
        int intersectionBottom = std::min(image.top + image.height, im.top + im.height); // bottom not included
        int intersectionHeight = intersectionBottom - intersectionTop;
        int intersectionLeft = std::max(image.left, im.left);
        int intersectionRight = std::min(image.left + image.width, im.left + im.width); // right not included
        int intersectionWidth = intersectionRight - intersectionLeft;

        intersectionLeft--;
        intersectionBottom--;
        intersectionHeight += 2;
        intersectionWidth += 2;

        std::vector<std::vector<char> > used;
        used.resize(intersectionHeight, std::vector<char>());
        for (auto &i : used)
            i.resize(intersectionWidth, false);

        iLeft = intersectionLeft;
        iRight = intersectionRight;
        iTop = intersectionTop;
        iBottom = intersectionBottom;

        for (int y = intersectionTop; y < intersectionBottom; y++)
            for (int x = intersectionLeft; x < intersectionRight; x++)
                if (im.inside(x, y) && isWhite(im.getPixel(x, y))) {
                    bool good = false;
                    for (int i = 0; i < 4; i++) {
                        int xx = x + dx[i];
                        int yy = y + dy[i];
                        if (!im.inside(xx, yy))
                            good = true;
                    }

                    if (good) {
                        eliminateWhite(x, y, used, im);
                    }
                }

        // WHITE ELIMINATION

        //imwrite("result_" + im.filename.substr(im.filename.size() - 7, 3) + ".jpg", im.getImage());

        Seam seam = solver.getSeam(image, im);


        Image copySeam = image.clone();
        Image copyMark = image.clone();


        image.combine(im, seam);

        copySeam.combine(im, seam);
        Visualizer::showSeam(copySeam, seam);
        Visualizer::markImage(im, 1);
        copyMark.combine(im, seam);
        num++;

        imwrite("result" + to_string(num) + ".jpg", image.image);
        imwrite("result_seam" + to_string(num) + ".jpg", copySeam.image);
        imwrite("result_mark" + to_string(num) + ".jpg", copyMark.image);

        images.pop_back();
    }
}