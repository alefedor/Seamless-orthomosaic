#pragma once

#include "basic/seam.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

typedef unsigned char Pixel;

const Pixel magic[3] = {255, 158, 250};

class Image {
 public:
    std::string filename;
    int left;
    int top;
    int centerX;
    int centerY;
    int width;
    int height;
    bool loaded;
    cv::Mat image;

    Image(const std::string& filename, int left, int top, int centerX, int centerY, int width, int height);
    Image(int left, int top, int width, int height);

    void loadImage();

    bool inside(int x, int y);
    Pixel* getPixel(int x, int y);
    void combine(Image& m, Seam& s);
    Image clone();
    
 private:
    void dfs(int x, int y, std::vector<std::vector<char> > &used, Seam &s, cv::Mat& result);
    
    Image& operator=(Image& im) = delete;
};

inline bool Image::inside(int x, int y) {
    return x >= left && y >= top && x < left + width && y < top + height && (image.type() != CV_8UC4 || getPixel(x, y)[3] != 0)
            && (getPixel(x, y)[0] != magic[0] || getPixel(x, y)[1] != magic[1] || getPixel(x, y)[2] != magic[2]); // crutch
}

inline Pixel* Image::getPixel(int x, int y) {
    return image.data + (image.type() == CV_8UC4 ? 4 : 3) * ((y - top) * width + (x - left));
}