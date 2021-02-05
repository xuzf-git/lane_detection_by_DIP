//
// Created by xuzf on 2021/2/5.
//
#include "../include/Kernel.h"
#include <iostream>
#include <cmath>

/*********************************************************
 * Kernel 类定义
 ********************************************************/

/* 构造空的滤波核 */
Kernel::Kernel(int size)
{
    assert(size % 2 != 0);
    this->size = size;
    data = new double *[size];
    for (int i = 0; i < size; ++i)
    {
        data[i] = new double[size];
        memset(data[i], 0, sizeof(double) * size);
    }
}

/* 拷贝构造函数 */
Kernel::Kernel(Kernel &cp)
{
    size = cp.size;
    data = new double *[size];
    for (int i = 0; i < size; ++i)
    {
        data[i] = new double[size];
        for (int j = 0; j < size; ++j)
        {
            data[i][j] = cp[i][j];
        }
    }
}

/* 析构函数 */
Kernel::~Kernel()
{
    for (int i = 0; i < size; ++i)
    {
        delete[] data[i];
    }
}

/* 访问指定行 */
double *Kernel::operator[](const int idx) const
{
    return data[idx];
}

/* 实现卷积操作 */
void Kernel::convolve(const Img<uchar> &src, Img<uchar> &dst) const
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
            val = val > 255 ? 255 : val;
            val = val < 0 ? 0 : val;
            dst[i][j] = uchar(round(val));
        }
    }
}

/*********************************************************
 * GaussianKernel 类定义
 ********************************************************/

/* 高斯 Kernel 的构造函数 */
GaussianKernel::GaussianKernel(const int size, const double sigma) : Kernel(size)
{
    this->sigma = sigma;

    const double PI = 4.0 * atan(1.0); // 圆周率π赋值
    int center = size / 2;
    double sum = 0;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            data[i][j] = (1 / (2 * PI * pow(sigma, 2)) *
                          exp(-(pow(i - center, 2) + pow(j - center, 2)) / (2 * pow(sigma, 2))));
            sum += data[i][j];
        }
    }
    // 归一化
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            data[i][j] /= sum;
        }
    }
}

/* 高斯 Kernel 的拷贝构造函数 */
GaussianKernel::GaussianKernel(GaussianKernel &cp) : Kernel(cp)
{
    sigma = cp.sigma;
}
