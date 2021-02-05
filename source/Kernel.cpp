//
// Created by xuzf on 2021/2/5.
//
#include "../include/Kernel.h"
#include <iostream>
#include <cmath>

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

/* 含参构造函数 */
Kernel::Kernel(const int size, const double sigma)
{
    assert(size % 2 != 0);
    this->size = size;
    this->sigma = sigma;
    data = new double *[size];

    const double PI = 4.0 * atan(1.0); // 圆周率π赋值
    int center = size / 2;
    double sum = 0;

    for (int i = 0; i < size; i++)
    {
        data[i] = new double[size];
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
//            cout << data[i][j] << "  ";
        }
//        cout << endl << endl;
    }
}

/* 复制构造函数 */
Kernel::Kernel(Kernel &cp)
{
    size = cp.size;
    sigma = cp.sigma;
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
double *Kernel::operator[](const int idx)
{
    return data[idx];
}

/* 实现卷积操作 */
void Kernel::convolve(const Img &src, Img &dst)
{
    if (!src.data || src.rows != dst.rows || src.cols != dst.cols)
        return;

    int offset = size / 2;
    double val = 0;
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
                        val += src[x][y] * data[offset + dx][offset + dy];
                    }
                }
            }
            val = val > 255 ? 255 : val;
            val = val < 0 ? 0 : val;
            dst[i][j] = uchar(round(val));
        }
    }
}

