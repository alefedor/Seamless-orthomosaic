#include "basic/image.h"

Image::Image(const std::string &filename, int left, int top, int centerX, int centerY, int width, int height):
    filename(filename), left(left), top(top), centerX(centerX), centerY(centerY), width(width), height(height), loaded(false) {}
    
Image::Image(int left, int top, int width, int height): left(left), top(top), loaded(true), width(width), height(height) {
    image = cv::Mat::zeros(cv::Size(width, height), CV_8UC4);
    centerX = centerY = -1;
    filename = "";
}
    
int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

bool Image::inside(int x, int y) {
    return x >= left && y >= top && x < left + getWidth() && y < top + getHeight() && (getImage().type() != CV_8UC4 || getPixel(x, y)[3] != 0);
}

Pixel* Image::getPixel(int x, int y) {
    return getImage().data + (getImage().type() == CV_8UC4 ? 4 : 3) * ((y - top) * getWidth() + (x - left)); 
}


inline cv::Mat& Image::getImage() {
    if (loaded)
        return image;
    image = cv::imread(filename, CV_LOAD_IMAGE_UNCHANGED);
    loaded = true;

    if (image.data == nullptr)
        throw std::runtime_error("Couldn't find or read file with image " + filename);
    if (image.type() != CV_8UC4 && image.type() != CV_8UC3)
        throw std::runtime_error("Incorrect type of image " + std::to_string(image.type()));
    return image;
}

static Pixel* getMatPixel(cv::Mat &image, int x, int y, int left, int top) {
    return image.data + (image.type() == CV_8UC4 ? 4 : 3) * ((y - top) * image.cols + (x - left));
}

void Image::combine(Image& m, Seam& s) {
    int height = getHeight();
    int width = getWidth();
    cv::Mat result = cv::Mat::zeros(cv::Size(width, height), CV_8UC4);
    
    std::vector<std::vector<char> > used;
    used.resize(height, std::vector<char>());
    for (auto &i : used)
        i.resize(width, false);
       
    for (int y = top; y < top + height; y++)
        for (int x = left; x < left + width; x++) {
            if (!used[y - top][x - left] && inside(x, y) && !m.inside(x, y)) { // in the part that belongs only to the first picture
                dfs(x, y, used, s, result);               
            }
        }  
    
    // the rest should be taken from second picture
    for (int y = top; y < top + height; y++)
        for (int x = left; x < left + width; x++) 
            if (!used[y - top][x - left]){
                Pixel* pixel = getMatPixel(result, x, y, left, top);
                if (m.inside(x, y)) {
                    Pixel *toPlace = m.getPixel(x, y);
                    for (int i = 0; i < 3; i++)
                        pixel[i] = toPlace[i];
                    pixel[3] = 255;
                }
            }   
    
    std::swap(image, result);
}

static int dx[4] = {1, -1, 0, 0};
static int dy[4] = {0, 0, 1, -1};

void Image::dfs(int x, int y, std::vector<std::vector<char> > &used, Seam &s, cv::Mat& result) {
    used[y - top][x - left] = true;
    
    Pixel *pixel = getMatPixel(result, x, y, left, top);
    Pixel *toPlace = getPixel(x, y);
    for (int i = 0; i < 3; i++)
        pixel[i] = toPlace[i];
    pixel[3] = 255;
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (!inside(nx, ny) || used[ny - top][nx - left] || s.has(getEdge(x, y, nx, ny))) {
            continue;
        }
        dfs(nx, ny, used, s, result);
    }
}

Image Image::clone() {
    Image result = Image(filename, left, top, centerX, centerY, width, height);
    if (loaded) {
        result.image = image.clone();
        result.loaded = true;
    }
    return result;
}
