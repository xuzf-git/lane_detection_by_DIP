//
// Created by xuzf on 2021/2/8.
//

#ifndef LANE_DETECTION_HOUGH_H
#define LANE_DETECTION_HOUGH_H
#include <vector>
#include <cmath>
#include "Img.hpp"
using namespace std;

void HoughTransform(Img<uchar> &src, vector<pair<int, int>> &lines, int threshold);

#endif //LANE_DETECTION_HOUGH_H
