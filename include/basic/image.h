#pragma once

#include "basic/seam.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

typedef unsigned char Pixel;

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

    Image(const std::string& filename, int left, int top, int centerX, int centerY, int width, int height);
    Image(int left, int top, int width, int height);
    
    int getWidth();
    int getHeight();
    cv::Mat& getImage();

    bool inside(int x, int y);
    Pixel* getPixel(int x, int y);
    void combine(Image& m, Seam& s);
    Image clone();
    
 private:
    cv::Mat image;
    void dfs(int x, int y, std::vector<std::vector<char> > &used, Seam &s, cv::Mat& result);
    
    Image& operator=(Image& im) = delete;
};

