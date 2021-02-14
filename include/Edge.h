//
// Created by xuzf on 2021/2/6.
//

#ifndef LANE_DETECTION_EDGE_DETECTION_H
#define LANE_DETECTION_EDGE_DETECTION_H

#include "Img.hpp"
#include "Kernel.h"

// 阈值分割
void TurnBinary(Img<uchar> &src, const double weight);

// 膨胀运算
void Dilation(const Img<uchar> &src, Img<uchar> &dst, int kernel_size);

// 腐蚀运算
void Erosion(const Img<uchar> &src, Img<uchar> &dst, int kernel_size);

// 遮盖无效部分
void RoiMask(Img<uchar> &src);

// Sobel 算子计算梯度
void Sobel(const Img<uchar> &src, Img<uchar> &dst, Img<double> &theta);

// 非极大值抑制
void NonMaxSuppression(const Img<uchar> &src, Img<uchar> &dst, const Img<double> &theta);

// 双阈值处理
void DoubleThreshold(Img<uchar> &image, const double weight = 0.9);

// Canny 边缘检测
void Canny(Img<uchar> &image, const double weight = 0.9);

#endif //LANE_DETECTION_EDGE_DETECTION_H