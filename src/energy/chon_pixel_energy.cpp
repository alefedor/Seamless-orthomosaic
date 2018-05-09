#include "energy/chon_pixel_energy.h"

const int frameSize = 3;

static inline int getI(Image &a, int x, int y) {
    Pixel* p = a.getPixel(x, y);
    return (p[0] + (int)p[1] + p[2]);
}

static inline double getAverage(Image &a, Image &b, int x, int y) {
    double sum = 0;
    int num = 0;
    for (int dy = -frameSize; dy <= frameSize; dy++)
        for (int dx = -frameSize; dx <= frameSize; dx++)
            if (a.inside(x + dx, y + dy) && b.inside(x + dx, y + dy)) {
                num++;
                sum += getI(a, x + dx, y + dy);
            }
    return sum / num;
}

static inline double getNCC(Image &a, Image &b, int x, int y) {
    double af = getAverage(a, b, x, y); //ZNCC
    double ag = getAverage(b, a, x, y);

    //double af = 0; //NCC
    //double ag = 0;

    double result = 0;
    double sumf = 0;
    double sumg = 0;
    for (int dy = -frameSize; dy <= frameSize; dy++)
        for (int dx = -frameSize; dx <= frameSize; dx++)
            if (a.inside(x + dx, y + dy) && b.inside(x + dx, y + dy)) {
                double df = (getI(a, x + dx, y + dy) - af);
                double dg = (getI(b, x + dx, y + dy) - ag);
                result +=  df * dg;
                sumf += df * df;
                sumg += dg * dg;
            }

    if (sumf == 0 || sumg == 0)
        return -1;

    return result / sqrt(sumf * sumg);
}

ChonPixelEnergy::ChonPixelEnergy(Image &a, Image &b) : a(a), b(b) {
    top =  std::max(a.top, b.top);
    int bottom = std::min(a.top + a.height, b.top + b.height); // bottom not included
    int height = bottom - top;
    left =  std::max(a.left, b.left);
    int right = std::min(a.left + a.width, b.left + b.width); // right not included
    int width = right - left;

    cv::Mat im = cv::Mat::zeros(cv::Size(width, height), CV_8UC3);

    for (int y = top; y < bottom; y++)
        for (int x = left; x < right; x++) {
            int val = calcEnergy(x, y) * 255;
            assert(0 <= val <= 255);
            for (int i = 0; i < 3; i++)
                (im.data + 3 * ((y - top) * width + x - left))[i] = val;
        }
    cv::imwrite("chon_dijkstra_energy_map_result.jpg", im);

    energy.resize(height, std::vector<double>());
    for (int i = 0; i < energy.size(); i++)
        energy[i].resize(width);

    for (int y = top; y < bottom; y++)
        for (int x = left; x < right; x++)
            if (a.inside(x, y) && b.inside(x, y))
                energy[y - top][x - left] = (1 - getNCC(a, b, x, y)) / 2.0;
}

double ChonPixelEnergy::calcEnergy(int x, int y) {
    return energy[y - top][x - left];
}