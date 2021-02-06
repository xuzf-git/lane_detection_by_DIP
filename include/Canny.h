//
// Created by xuzf on 2021/2/6.
//

#ifndef LANE_DETECTION_CANNY_H
#define LANE_DETECTION_CANNY_H

#include "Img.hpp"
#include "Kernel.h"

// Sobel 算子计算梯度
void Sobel(const Img<uchar> &src, Img<uchar> &dst, Img<double> &theta);

// 非极大值抑制
void NonMaxSuppression(const Img<uchar> &src, Img<uchar> &dst, const Img<double> &theta);

// 双阈值处理
void DoubleThreshold(const Img<uchar> &image, const double weight = 0.9);

// Canny 边缘检测
void Canny(Img<uchar> &image, const double weight = 0.9);

#endif //LANE_DETECTION_CANNY_H