#include "basic/reader.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

bool intersects(int x, int y, int w, int h, Image &m) {
    return max(x, m.left) < min(x + w, m.left + m.width) &&
           max(y, m.top) < min(y + h, m.top + m.height);
}

const int significance = 100;

bool intersectsSignificantly(int x, int y, int w, int h, Image &m) {
    return max(x, m.left) + significance < min(x + w, m.left + m.width) &&
           max(y, m.top) + significance < min(y + h, m.top + m.height);
}

int square(int x, int y, int w, int h, Image &m) {
    if (!intersects(x, y, w, h, m))
        return 0;

    return (min(x + w, m.left + m.width)-max(x, m.left)) * (min(y + h, m.top + m.height) - max(y, m.top));
}

int main(int argnum, char** args) {
    if (argnum != 4) {
        std::cout << "Usage: ./variants number_of_variant width height";
        return 0;
    }

    int num = atoi(args[1]);
    int width = atoi(args[2]);
    int height = atoi(args[3]);
    int s = width * height;
    vector<Image> images;
    int regionWidth;
    int regionHeight;
    Reader::readImages(images);
    Reader::readRegionSize(regionWidth, regionHeight);
    ofstream out("variants.txt", std::ofstream::out);
    out << "left top width height number_of_intersections number_of_significant_intersections\n";
    for (int i = 0; i < num; i++) {
        int x = random() % (regionWidth - width);
        int y = random() % (regionHeight - height);
        x -= x % 100;
        y -= y % 100;
        int numN = 0;
        int numS = 0;
        bool bad = false;
        for (auto &image : images) {
            if (intersects(x, y, width, height, image)) {
                numN++;
                if (intersectsSignificantly(x, y, width, height, image))
                    numS++;
                if (square(x, y, width, height, image) > 0.6 * s) {
                    bad = true;
                    break;
                }
            }
        }
        if (bad) {
            i--;
            continue;
        }
        out << x << " " << y << " " << width << " " << height << " " << numN << " " << numS << "\n";
    }
    return 0;
}