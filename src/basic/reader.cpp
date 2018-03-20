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

    getline(in, s1, '=');
    getline(in, s1);
    width = atoi(s1.c_str());

    getline(in, s2, '=');
    getline(in, s2);
    width = atoi(s2.c_str());

    if (in.fail())
        throw std::runtime_error("Couldn't read region size info");
}


void Reader::readImages(std::vector<Image> &result) {
    std::ifstream in("images.csv", std::ifstream::in);
    if (in.fail())
        throw std::runtime_error("Couldn't find or open file with image info");
        
    std::string s;
    getline(in, s);

    while (!in.eof()) {
        std::string filename;
        getline(in, filename, ',');
        filename = "orthophotos/" + filename + ".jpg";

        getline(in, s, ',');
        int left = atoi(s.c_str());

        getline(in, s, ',');
        int top = atoi(s.c_str());

        getline(in, s, ',');
        int centerX = atoi(s.c_str());

        getline(in, s, ',');
        int centerY = atoi(s.c_str());

        getline(in, s, ',');
        int width = atoi(s.c_str());

        getline(in, s);
        int height = atoi(s.c_str());

        result.emplace_back(filename, left, top, centerX, centerY, width, height);
    }
}
