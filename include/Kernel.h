//
// Created by xuzf on 2021/2/5.
//

#ifndef LANE_DETECTION_KERNEL_H
#define LANE_DETECTION_KERNEL_H

#include "Img.hpp"

class Kernel
{
public:
    double **data;
    int size;

    Kernel(int size);   // 空的卷积核
    Kernel(Kernel &cp); // 拷贝构造函数
    ~Kernel();

    double *operator[](const int idx) const;
    void convolve(const Img<uchar> &src, Img<uchar> &dst) const;
};

class GaussianKernel : public Kernel
{
public:
    double sigma;

    GaussianKernel(const int size, const double sigma);
    GaussianKernel(GaussianKernel &cp);
};

#endif //LANE_DETECTION_KERNEL_H
