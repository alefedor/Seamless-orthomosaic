#include "energy/pan_pixel_energy.h"

const int delta = 2100;

PanPixelEnergy::PanPixelEnergy(Image &a, Image &b, bool segmentation) : a(a), b(b), segmented(segmentation) {
    if (segmentation) {
        int intersectionTop = top = std::max(a.top, b.top);
        int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
        int height = intersectionBottom - intersectionTop;
        int intersectionLeft = left = std::max(a.left, b.left);
        int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
        width = intersectionRight - intersectionLeft;

        cv::Mat im = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);

        int sigmaS = 6;
        int sigmaR = 4;
        int minArea = 1000;
        SpeedUpLevel speedupLevel = HIGH_SPEEDUP;

        for (int y = intersectionTop; y < intersectionBottom; y++)
            for (int x = intersectionLeft; x < intersectionRight; x++)
                if (a.inside(x, y) && b.inside(x, y)) {
                    Pixel *to = im.data +
                                (im.type() == CV_8UC4 ? 4 : 3) * ((y - intersectionTop) * width + x - intersectionLeft);
                    Pixel *from = a.getPixel(x, y);
                    if (im.type() == CV_8UC4)
                        to[3] = 255;

                    for (int i = 0; i < 3; i++)
                        to[i] = from[i];
                }

        SegmentedRegions regions = meanShiftSegmentation(im.data, width, height, 3,
                                                         sigmaS, sigmaR, minArea, speedupLevel, false);

        std::vector<int> minX, maxX, minY, maxY;

        int num = regions.getNumRegions();

        minX.resize(num, 1e9);
        maxX.resize(num, -1e9);
        minY.resize(num, 1e9);
        maxY.resize(num, -1e9);

        for (size_t i = 0; i < num; i++) {
            std::vector<PixelPosition> border = regions.getRegionBorder(i);
            for (auto &x : border) {
                for (int j = 0; j < 3; j++)
                    (im.data + 3 * (x.row * width + x.column))[j] = 255;
                minX[i] = std::min(minX[i], x.column);
                maxX[i] = std::max(maxX[i], x.column);
                minY[i] = std::min(minY[i], x.row);
                maxY[i] = std::max(maxY[i], x.row);
            }
        }

        isPR.resize(num, false);
        labels = std::move(regions.getPixelLabels());

        for (int i = 0; i < num; i++)
            if (maxX[i] - minX[i] > delta || maxY[i] - minY[i] > delta)
                isPR[i] = true;


        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                if (isPR[labels[y * width + x]])
                    for (int j = 0; j < 3; j++)
                        (im.data + 3 * (y * width + x))[j] = magic[j];

        cv::imwrite("result_segmentation.jpg", im);
    }
};

inline double PanPixelEnergy::getI(Image &a, int x, int y) {
    Pixel* p = a.getPixel(x, y);
    return (p[0] + (int)p[1] + p[2]) * (segmented && isPR[labels[(y - top) * width + x - left]] ? 0.4 : 1);
}

double PanPixelEnergy::calcEnergy(int x, int y) {
    return fabs(getI(a, x, y) - getI(b, x, y));
}