#include "basic/reader.h"
#include "basic/image.h"
#include "solutions/best_neighbour.h"
#include "visual/visualizer.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace std;
using namespace cv;

int main(int argnum, char** args) {
    if (argnum != 6) {
        std::cout << "Usage: ./orthomosaic left top width height [NORMAL | SEAM | ALL]";
        return 0;
    }
    
    int left = atoi(args[1]);
    int top = atoi(args[2]);
    int width = atoi(args[3]);
    int height = atoi(args[4]);
    int PYR_DOWN_COUNT = 1;
    
    int type;
    if (args[5][0] == 'N')
        type = 0;
    else if (args[5][0] == 'S')
        type = 1;
    else
        type = 2;
    //Reader::readRegionSize(width, height);
    
    Image image = Image(left, top, width, height);   
    
    vector<Image> images;
    Reader::readImages(images);
    while (!images.empty()) {
        Image &im = images.back();
        if (max(image.top, im.top) >= min(image.top + image.getHeight(), im.top + im.getHeight()) ||
            max(image.left, im.left) >= min(image.left + image.getWidth(), im.left + im.getWidth())) {
                images.pop_back();
                continue;
            }
        Seam seam = getSeamBestNeighbour(image, im);
        
        if (type == 2) {    
            Image copy = image.clone();    
            Image copyim = im.clone();
            Visualizer::markImage(copyim, 1);
            copy.combine(copyim, seam);
            Visualizer::showSeam(copy, seam);
            for (int i = 0; i < PYR_DOWN_COUNT; i++)
                 pyrDown(copy.image, copy.image, Size(copy.image.cols / 2, copy.image.rows / 2));
    
            imshow("Program result", copy.image);
            waitKey(0);
        } 
        
        image.combine(im, seam);
        
        if (type < 2) {
            Image copy = image.clone();
            
            if (type == 0) {
                //nothing to do            
            } else
            if (type == 1) {
                Visualizer::showSeam(copy, seam);
            }
            for (int i = 0; i < PYR_DOWN_COUNT; i++)
                 pyrDown(copy.image, copy.image, Size(copy.image.cols / 2, copy.image.rows / 2));
        
            imshow("Program result", copy.image);
            waitKey(0);
        }
        
        images.pop_back();
    }
    return 0;
}
