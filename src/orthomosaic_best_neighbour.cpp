#include "basic/reader.h"
#include "solutions/best_neighbour.h"
#include "visual/visualizer.h"

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
    vector<vector<double>> dist = initialBestNeighbourDist(height, width);
    
    int num = 0;
    while (!images.empty()) {
        Image &im = images.back();
        if (max(image.top, im.top) >= min(image.top + image.height, im.top + im.height) ||
            max(image.left, im.left) >= min(image.left + image.width, im.left + im.width)) {
                images.pop_back();
                continue;
            }
        Seam seam = getSeamBestNeighbour(image, im, dist);
        num++;
        if (type == 2) {    
            Image copy = image.clone();    
            Image copyim = im.clone();
            Visualizer::markImage(copyim, 1);
            copy.combine(copyim, seam);
            Visualizer::showSeam(copy, seam);
            imwrite("result" + to_string(num) + ".jpg", copy.getImage());
            for (int i = 0; i < PYR_DOWN_COUNT; i++)
                 pyrDown(copy.getImage(), copy.getImage(), Size(copy.getImage().cols / 2, copy.getImage().rows / 2));
    
            imshow("Program result", copy.getImage());
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
            
            imwrite("result" + to_string(num) + ".jpg", copy.getImage());
            for (int i = 0; i < PYR_DOWN_COUNT; i++)
                 pyrDown(copy.getImage(), copy.getImage(), Size(copy.getImage().cols / 2, copy.getImage().rows / 2));
        
            //imshow("Program result", copy.getImage());
            //waitKey(0);
        }
        
        images.pop_back();
    }
    return 0;
}
