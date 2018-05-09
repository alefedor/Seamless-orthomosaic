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
        if (image.inside(xx, yy) && insideArea(xx, yy) && !used[yy - iTop][xx - iLeft] && isWhite(image.getPixel(xx, yy)))
            eliminateWhite(xx, yy, used, image);
    }
}

static void check(int x, int y, Image &im, vector<vector<char>> &used) {
    if (im.inside(x, y) && insideArea(x, y) && isWhite(im.getPixel(x, y))) {
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
}

void Runner::run(int argnum, char **args, SeamSolver &&solver, string prefix) {
    if (argnum != 7) {
        cout << "Usage: ./orthomosaic left top width height image1 image2";
        return;
    }

    int left = atoi(args[1]);
    int top = atoi(args[2]);
    int width = atoi(args[3]);
    int height = atoi(args[4]);
    vector<string> filenames;
    filenames.emplace_back(args[5]);
    filenames.emplace_back(args[6]);
    int PYR_DOWN_COUNT = 1;

    //Reader::readRegionSize(width, height);

    Image image = Image(left, top, width, height);

    vector<Image> images;
    Reader::readImages(images);

    int num = 0;
    while (!images.empty()) {
        Image &im = images.back();

        if (find(filenames.begin(), filenames.end(), im.filename) == filenames.end())
            continue;

        if (max(image.top, im.top) >= min(image.top + image.height, im.top + im.height) ||
            max(image.left, im.left) >= min(image.left + image.width, im.left + im.width)) {
            images.pop_back();
            continue;
        }

        im.loadImage();

        // WHITE ELIMINATION

        int intersectionTop = std::max(image.top, im.top);
        int intersectionBottom = std::min(image.top + image.height, im.top + im.height); // bottom not included
        int intersectionHeight = intersectionBottom - intersectionTop + 2;
        int intersectionLeft = std::max(image.left, im.left);
        int intersectionRight = std::min(image.left + image.width, im.left + im.width); // right not included
        int intersectionWidth = intersectionRight - intersectionLeft + 2;

        std::vector<std::vector<char> > used;
        used.resize(intersectionHeight, std::vector<char>());
        for (auto &i : used)
            i.resize(intersectionWidth, false);

        iLeft = intersectionLeft - 1;
        iRight = intersectionRight + 1;
        iTop = intersectionTop - 1;
        iBottom = intersectionBottom + 1;

        for (int y : {im.top, im.top + im.height - 1})
            for (int x = intersectionLeft - 1; x <= intersectionRight; x++)
                check(x, y, im, used);

        for (int y = intersectionTop - 1; y <= intersectionBottom; y++)
            for (int x : {im.left, im.left + im.width - 1})
                check(x, y, im, used);


        // WHITE ELIMINATION

        Mat preimage = Mat::zeros(cv::Size(width, height), CV_8UC3);
        for (int y = intersectionTop; y < intersectionBottom; y++)
            for (int x = intersectionLeft; x < intersectionRight; x++)
                if (im.inside(x, y)) {
                    Pixel *from = im.getPixel(x, y);
                    Pixel* to = preimage.data + 3 * ((y - top) * width + x - left);
                    for (int i = 0; i < 3; i++)
                        to[i] = from[i];
                }

        imwrite("image" + to_string(num) + "_result.jpg", preimage);

        //imwrite("result_" + im.filename.substr(im.filename.size() - 7, 3) + ".jpg", im.getImage());

        Seam seam = solver.getSeam(image, im);

        image.combine(im, seam);

        num++;

        if (num != 1) {
            Image copySeam = image.clone();
            Image copyMark = image.clone();

            copySeam.combine(im, seam);
            Visualizer::showSeam(copySeam, seam);
            Visualizer::markImage(im, 1);
            copyMark.combine(im, seam);

            imwrite(prefix + "result" + ".jpg", image.image);
            imwrite(prefix + "seam_result" + ".jpg", copySeam.image);
            imwrite(prefix + "mark_result" +".jpg", copyMark.image);
        }

        images.pop_back();
    }
}