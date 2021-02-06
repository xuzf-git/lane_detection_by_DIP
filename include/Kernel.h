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

    template<typename T1, typename T2>
    void convolve(const Img<T1> &src, Img<T2> &dst, const bool is_clip = true) const;    // 卷积操作
};

class GaussianKernel : public Kernel
{
public:
    double sigma;

    GaussianKernel(const int size, const double sigma);

    GaussianKernel(GaussianKernel &cp);
};

/* 实现卷积操作 */
template<typename T1, typename T2>
void Kernel::convolve(const Img<T1> &src, Img<T2> &dst, const bool is_clip) const
{
    if (!src.data || src.rows != dst.rows || src.cols != dst.cols)
        return;

    int offset = size / 2;
    double val;
    int x, y;

    for (int i = 0; i < dst.rows; ++i)
    {
        for (int j = 0; j < dst.cols; ++j)
        {
            /* 计算目标图像上每个像素点的值 */
            val = 0;
            for (int dx = -offset; dx <= offset; ++dx)
            {
                for (int dy = -offset; dy <= offset; ++dy)
                {
                    x = i + dx;
                    y = j + dy;
                    if (x >= 0 && x < dst.rows && y >= 0 && y <= dst.cols)
                    {
                        val += double(src[x][y]) * double(data[offset + dx][offset + dy]);
                    }
                }
            }
            if (is_clip)    // 截断到 0 ~ 255
            {
                val = val > 255 ? 255 : val;
                val = val < 0 ? 0 : val;
                val = round(val);
            }
            dst[i][j] = T2(val);
        }
    }
}

#endif //LANE_DETECTION_KERNEL_H
