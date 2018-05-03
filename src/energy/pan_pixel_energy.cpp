#include <opencv2/imgproc/imgproc.hpp>
#include "energy/pan_pixel_energy.h"

PanPixelEnergy::PanPixelEnergy(Image &a, Image &b, bool segmentation) : a(a), b(b), segmented(segmentation) {
    if (segmentation) {
        int intersectionTop = std::max(a.top, b.top);
        int intersectionBottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
        int height = intersectionBottom - intersectionTop;
        int intersectionLeft = std::max(a.left, b.left);
        int intersectionRight = std::min(a.left + a.width, b.left + b.width); // right not included
        int width = intersectionRight - intersectionLeft;

        cv::Mat im = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);

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

        int spatialRadius = 15;
        int colorRadius = 20;
        int pyramidLevels = 4;
        cv::pyrMeanShiftFiltering(im, image, spatialRadius, colorRadius, pyramidLevels);
        cv::imwrite("result_segmentation.jpg", image);
    }
};

static int getI(Image &a, int x, int y) {
    Pixel* p = a.getPixel(x, y);
    return p[0] + p[1] + p[2];
}

double PanPixelEnergy::calcEnergy(int x, int y) {
    return abs(getI(a, x, y) - getI(b, x, y));
}