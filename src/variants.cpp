#include "basic/reader.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

bool intersects(int x, int y, int w, int h, Image &m) {
    return max(x, m.left) < min(x + w, m.left + m.width) &&
           max(y, m.top) < min(y + h, m.top + m.height);
}


int square(int l, int t, int r, int b) {
    return (r - l) * (b - t);
}

int main(int argnum, char** args) {
    if (argnum != 4) {
        std::cout << "Usage: ./variants number_of_variant width height";
        return 0;
    }

    int num = atoi(args[1]);
    int width = atoi(args[2]);
    int height = atoi(args[3]);
    vector<Image> images;
    int regionWidth;
    int regionHeight;
    Reader::readImages(images);
    Reader::readRegionSize(regionWidth, regionHeight);
    ofstream out("tests.txt", std::ofstream::out);
    out << "left top width height im1 im2\n";
    for (int i = 0; i < num; i++) {
        bool found = false;
        while (!found) {
            int x = random() % (regionWidth - width);
            int y = random() % (regionHeight - height);
            if (i == 0) {
                x = 68200 + (7000 - width);
                y = 21200;
            }
            x -= x % 100;
            y -= y % 100;
            for (auto &im1 : images)
                if (!found && intersects(x, y, width, height, im1)) {
                    int t1 = max(y, im1.top);
                    int l1 = max(x, im1.left);
                    int r1 = min(x + width, im1.left + im1.width);
                    int b1 = min(y + height, im1.top + im1.height);
                    for (auto &im2 : images)
                        if ((im1.top != im2.top || im1.left != im2.left) && intersects(x, y, width, height, im2)) {
                            int t2 = max(y, im2.top);
                            int l2 = max(x, im2.left);
                            int r2 = min(x + width, im2.left + im2.width);
                            int b2 = min(y + height, im2.top + im2.height);
                            if (max(t1, t2) >= min(b1, b2) || max(l1, l2) >= min(r1, r2))
                                continue;

                            int ti = max(t1, t2);
                            int li = max(l1, l2);
                            int ri = min(r1, r2);
                            int bi = min(b1, b2);

                            int s1 = square(l1, t1, r1, b1);
                            int s2 = square(l2, t2, r2, b2);
                            int si = square(li, ti, ri, bi);
                            int total = width * height;

                            bool good = true;
                            good &= (si != s1);
                            good &= (si != s2);
                            good &= (total * 0.5 < si);

                            if (good) {
                                found = true;
                                out << x << " " << y << " " << width << " " << height << " " << im1.filename << " " << im2.filename << "\n";
                                break;
                            }
                        }
                }
        }
    }
    return 0;
}