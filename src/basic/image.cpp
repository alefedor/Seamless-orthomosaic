#include "basic/image.h"
#include "basic/seam.h"
#include <exception>

Image::Image(const std::string &filename, int left, int top, int centerX, int centerY):
    filename(filename), left(left), top(top), centerX(centerX), centerY(centerY), image(nullptr) {}
    
Image::Image(int left, int top, int width, int height): left(left), top(top){
    image = new cv::Mat(height, width, CV_8UC4);
    centerX = centerY = -1;
}
    
int Image::getWidth() {
    return getImage() -> cols;
}

int Image::getHeight() {
    return getImage() -> rows;
}

bool Image::inside(int x, int y) {
    return x >= left && y >= top && x < left + getWidth() && y < top + getHeight() && getPixel(x, y)[3] != 0;
}

unsigned char* Image::getPixel(int x, int y) {
    return getImage() -> data + 4 * ((y - top) * getWidth() + (x - left)); 
}


inline cv::Mat* Image::getImage() {
    if (image)
        return image;
    image = new cv::Mat(cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED));
    if (image -> data == nullptr)
        throw std::runtime_error("Couldn't find or read file with image " + filename);
    return image;
}

Image::~Image() {
    if (image)
        delete image;
}

static unsigned char* getMatPixel(cv::Mat* image, int x, int y, int left, int top) {
    return image -> data + 4 * ((y - top) * image -> cols + (x - left)); 
}

void Image::combine(Image& m, Seam& s) {
    int height = getHeight();
    int width = getWidth();
    cv::Mat *result = new cv::Mat(cv::Mat::zeros(cv::Size(height, width), CV_8UC4));
    
    std::vector<std::vector<char> > used;
    used.resize(height, std::vector<char>());
    for (int i = 0; i < (int)used.size(); i++)
        used[i].resize(width, false);
       
    for (int y = top; y < top + height; y++)
        for (int x = left; x < left + width; x++) {
            if (!used[y - top][x - width] && inside(x, y) && !m.inside(x, y)) { // in the part that belongs only to the first picture
                dfs(x, y, used, s, result);               
            }
        }  
    
    // the rest should be taken from second picture
    for (int y = top; y < top + height; y++)
        for (int x = left; x < left + width; x++) {
            unsigned char* pixel = getMatPixel(result, x, y, left, top);
            if (pixel[3] == 0 && m.inside(x, y)) {
                unsigned char *toPlace = m.getPixel(x, y);
                for (int i = 0; i < 4; i++)
                    pixel[i] = toPlace[i];
            }
        }   
    
    std::swap(image, result);
    delete result;
}

static int dx[4] = {1, -1, 0, 0};
static int dy[4] = {0, 0, 1, -1};

void Image::dfs(int x, int y, std::vector<std::vector<char> > &used, Seam &s, cv::Mat* result) {
    used[y][x] = true;
    
    unsigned char *pixel = getMatPixel(result, x, y, left, top);
    unsigned char *toPlace = getPixel(x, y);
    for (int i = 0; i < 4; i++)
        pixel[i] = toPlace[i];
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (!inside(nx, ny) || used[ny - top][nx - left] || s.has(getEdge(x, y, nx, ny))) {
            continue;
        }
        dfs(nx, ny, used, s, result);
    }
}

