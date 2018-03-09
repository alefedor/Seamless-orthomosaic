#include "basic/reader.h"
#include "basic/image.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

unsigned char* getPixel3(Mat& image, int x, int y) {
    return image.data + 3 * (y * image.cols + x); 
}

int main() {
    int width, height;
    Reader::readRegionSize(width, height);
    
    Mat result = Mat(height, width, CV_8UC3);
    
    vector<Image> images;
    Reader::readImages(images);
    for (int y = 0; y < height; y++) 
        for (int x = 0; x < width; x++) {
            unsigned char* pixel = getPixel3(result, x, y);
            pixel[0] = pixel[1] = pixel[2] = 255;
            double sum[4] = {0, 0, 0, 0};
            for (Image& image : images) { 
                if (image.inside(x, y)) {
                    unsigned char *p = image.getPixel(x, y);
                    if (p[3] == 0)
                        continue;
                    double w = 0;
                    for (int i = 0; i < 3; i++)
                        sum[i] += w * p[i];
                    sum[3] += w;
                }
            }
                
           if (abs(sum[3]) > 1e-6) {
                for (int i = 0; i < 3; i++)
                    pixel[i] = (int)(sum[i] / sum[3]); 
           } 
        }
        
    imwrite("result.jpg", result);
    
    for (int i = 0; i < 3; i++)
        pyrDown(result, result, Size(result.cols / 2, result.rows / 2));
        
    imshow("Program result", result);
    waitKey(0);
    return 0;
}
