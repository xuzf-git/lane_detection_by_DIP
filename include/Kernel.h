//
// Created by xuzf on 2021/2/5.
//

#ifndef LANE_DETECTION_KERNEL_H
#define LANE_DETECTION_KERNEL_H
#include "Img.h"
class Kernel
{
public:
    double **data;
    int size;
    double sigma;
    Kernel(int size);   // 空的卷积核
    Kernel(const int size, const double sigma); // 高斯kernel
    Kernel(Kernel& cp);
    ~Kernel();
    double *operator[](const int idx);
    void convolve(const Img &src, Img &dst);

};
#endif //LANE_DETECTION_KERNEL_H
