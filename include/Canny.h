//
// Created by xuzf on 2021/2/6.
//

#ifndef LANE_DETECTION_CANNY_H
#define LANE_DETECTION_CANNY_H
#include "Img.hpp"
#include "Kernel.h"

// Sobel 算子计算梯度
void Sobel(const Img<uchar> &src, Img<uchar> &dst, Img<int> &theta);
#endif //LANE_DETECTION_CANNY_H
