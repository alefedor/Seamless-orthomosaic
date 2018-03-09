#ifndef __IMAGE_H_INCLUDED
#define __IMAGE_H_INCLUDED

#include "basic/seam.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <vector>

class Image {
 public:
    std::string filename;
    int left;
    int top;
    int centerX;
    int centerY;
    
    Image(const std::string& filename, int left, int top, int centerX, int centerY);
    Image(int left, int top, int width, int height);
    ~Image();
    
    int getWidth();
    int getHeight();
    bool inside(int x, int y);
    unsigned char* getPixel(int x, int y);
    void combine(Image& m, Seam& s);
    
 private:
    cv::Mat* image;
    
    inline cv::Mat* getImage();
    void dfs(int x, int y, std::vector<std::vector<char> > &used, Seam &s, cv::Mat* result);
};

#endif
