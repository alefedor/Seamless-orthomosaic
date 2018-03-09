#include "basic/reader.h"

#include <string>
#include <fstream>
#include <exception>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

void Reader::readRegionSize(int &width, int &height) {
    std::ifstream in("general.txt", std::ifstream::in);
    std::string s1, s2;
    in >> s1;
    int pos1 = (find(s1.begin(), s1.end(), '=') - s1.begin());
    width = atoi(s1.substr(pos1 + 1, s1.size()).c_str());
    
    in >> s2;
    int pos2 = (find(s2.begin(), s2.end(), '=') - s2.begin());
    height = atoi(s2.substr(pos2 + 1, s2.size()).c_str());
    
    if (in.fail())
        throw std::runtime_error("Couldn't read region size info");
}


void Reader::readImages(std::vector<Image*> &result) {
    std::ifstream in("images.csv", std::ifstream::in);
    if (in.fail())
        throw std::runtime_error("Couldn't find or open file with image info");
        
    std::string s;
    in >> s; //first line
    
    while (in >> s) {
        int pos;
        pos = s.find(',');
        std::string filename = s.substr(0, pos) + ".j";
        
        int nextpos;
        nextpos = s.find(',', pos + 1);
        int left = atoi(s.substr(pos + 1, nextpos - pos - 1).c_str());
        
        pos = nextpos;
        nextpos = s.find(',', pos + 1);
        int top = atoi(s.substr(pos + 1, nextpos - pos - 1).c_str());
        
        pos = nextpos;
        nextpos = s.find(',', pos + 1);
        int centerX = atoi(s.substr(pos + 1, nextpos - pos - 1).c_str());
        
        pos = nextpos;
        nextpos = s.size();
        int centerY = atoi(s.substr(pos + 1, nextpos - pos - 1).c_str());
        
        Image* image = new Image(filename, left, top, centerX, centerY);   
               
        result.push_back(image);
    }
}
