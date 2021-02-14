//
// Created by xuzf on 2021/2/6.
//

#include "../include/Edge.h"
#include <vector>
#include <algorithm>

/* 阈值分割，得到二值图像 */
void TurnBinary(Img<uchar> &src, double weight)
{
    std::vector<uchar> vec;
    // 高阈值取灰度分布图中 weight 对应的灰度值
    for (int i = 160; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            vec.push_back(src[i][j]);
        }
    }
    std::sort(vec.begin(), vec.end());
    int threshold = vec[(src.rows - 160)* src.cols * weight];
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            if (src[i][j] > threshold && i >= 160)
                src[i][j] = 255;
            else
                src[i][j] = 0;
        }
    }
}

/* 形态学：膨胀运算 */
void Dilation(const Img<uchar> &src, Img<uchar> &dst, int kernel_size)
{
    for (int i = kernel_size; i < src.rows - kernel_size; ++i)
    {
        for (int j = kernel_size; j < src.cols - kernel_size; ++j)
        {
            if (src[i][j] == 255)
            {
                for (int x = -kernel_size; x <= kernel_size; ++x)
                {
                    for (int y = -kernel_size; y <= kernel_size; ++y)
                    {
                        dst[i + x][j + y] = 255;
                    }
                }
            }
            else
                dst[i][j] = 0;
        }
    }
}

/* 形态学: 腐蚀运算 */
void Erosion(const Img<uchar> &src, Img<uchar> &dst, int kernel_size)
{
    bool is_kept;
    for (int i = kernel_size; i < src.rows - kernel_size; ++i)
    {
        for (int j = kernel_size; j < src.cols - kernel_size; ++j)
        {
            is_kept = true;
            for (int x = -kernel_size; x <= kernel_size; ++x)
            {
                if (!is_kept)
                    break;
                for (int y = -kernel_size; y <= kernel_size; ++y)
                {
                    if (src[i + x][j + y] != 255)
                    {
                        is_kept = false;
                        break;
                    }
                }
            }
            dst[i][j] = is_kept ? 255 : 0;
        }
    }
}

/* 遮盖无效部分 */
void RoiMask(Img<uchar> &src)
{
    // 梯形 ROI 区域进行 mask (400, 0) (220, 420) (200, 860), (400, 1280)
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            if (i <= 200)
                src[i][j] = 0;
            else if (i > 400)
                continue;
            else if (2.1 * (400 - i) > j || j > 1280 - 2.1 * (400 - i))
                src[i][j] = 0;
        }
    }
}

/* Sobel 算子：计算图像梯度 */
void Sobel(const Img<uchar> &src, Img<uchar> &dst, Img<double> &theta)
{
    assert(src.rows == dst.rows);
    assert(src.cols == dst.cols);

    const double sobelX_arr[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
    };
    const double sobelY_arr[3][3] = {
            {1,  2,  1},
            {0,  0,  0},
            {-1, -2, -1}
    };
    Kernel sobelX(3);
    Kernel sobelY(3);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            sobelX[i][j] = sobelX_arr[i][j];
            sobelY[i][j] = sobelY_arr[i][j];
        }
    }
    Img<double> imgGradX(src.rows, src.cols);
    Img<double> imgGradY(src.rows, src.cols);
    sobelX.convolve(src, imgGradX, false);
    sobelY.convolve(src, imgGradY, false);
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            dst[i][j] = sqrt(imgGradX[i][j] * imgGradX[i][j] + imgGradY[i][j] * imgGradY[i][j]);
            if (fabs(imgGradX[i][j]) < 1e-10)   // 防止除以0，导致溢出
                imgGradX[i][j] = 1e-10;
            theta[i][j] = atan((double) imgGradY[i][j] / imgGradX[i][j]);
        }
    }
}

/* 非极大值抑制 */
void NonMaxSuppression(const Img<uchar> &src, Img<uchar> &dst, const Img<double> &theta)
{
    assert(src.rows == dst.rows);
    assert(src.cols == dst.cols);

    // 将 src 的值拷贝到 dst 中
    dst = src;

    uchar local[3][3];
    uchar temp1, temp2;
    double weight;
    const double PI_2 = PI / 2;
    const double PI_4 = PI / 4;

    for (int i = 1; i < src.rows - 1; ++i)
    {
        for (int j = 1; j < src.cols - 1; ++j)
        {
            // 记录考察点的局部值
            for (int x = 0; x < 3; ++x)
            {
                for (int y = 0; y < 3; ++y)
                {
                    local[x][y] = src[i - 1 + x][j - 1 + y];
                }
            }
            if (theta[i][j] > -PI_2 && theta[i][j] <= -PI_4)
            {
                weight = fabs(1 / tan(theta[i][j]));
                temp1 = uchar(weight * local[2][2] + (1 - weight) * local[2][1]);
                temp2 = uchar(weight * local[0][0] + (1 - weight) * local[0][1]);
                if (local[1][1] <= temp1 || local[1][1] <= temp2)
                    dst[i][j] = 0;

            } else if (theta[i][j] > -PI_4 && theta[i][j] <= 0)
            {
                weight = fabs(tan(theta[i][j]));
                temp1 = uchar(weight * local[2][2] + (1 - weight) * local[1][2]);
                temp2 = uchar(weight * local[0][0] + (1 - weight) * local[1][0]);
                if (local[1][1] <= temp1 || local[1][1] <= temp2)
                    dst[i][j] = 0;
            } else if (theta[i][j] > 0 && theta[i][j] <= PI_4)
            {
                weight = tan(theta[i][j]);
                temp1 = uchar(weight * local[0][2] + (1 - weight) * local[1][2]);
                temp2 = uchar(weight * local[2][0] + (1 - weight) * local[1][0]);
                if (local[1][1] <= temp1 || local[1][1] <= temp2)
                    dst[i][j] = 0;
            } else if (theta[i][j] > PI_4 && theta[i][j] < PI_2)
            {
                weight = 1 / tan(theta[i][j]);
                temp1 = uchar(weight * local[0][2] + (1 - weight) * local[0][1]);
                temp2 = uchar(weight * local[2][0] + (1 - weight) * local[2][1]);
                if (local[1][1] <= temp1 || local[1][1] <= temp2)
                    dst[i][j] = 0;
            }
        }
    }
}

/* 双阈值检测 & 连接边缘 */
void DoubleThreshold(Img<uchar> &image, const double weight)
{
    double highThreshold;
    double lowThreshold;
    bool flag = false;
    std::vector<uchar> vec;

    // 高阈值取灰度分布图中 weight 对应的灰度值
    for (int i = 0; i < image.rows; ++i)
    {
        for (int j = 0; j < image.cols; ++j)
            vec.push_back(image[i][j]);
    }
    std::sort(vec.begin(), vec.end());
    highThreshold = vec[weight * image.rows * image.cols];
    // 低阈值为高阈值的 2/3
    lowThreshold = highThreshold / 1.5;

    for (int i = 1; i < image.rows - 1; ++i)
    {
        for (int j = 1; j < image.cols - 1; ++j)
        {
            if (image[i][j] < lowThreshold) // 检测低阈值
                image[i][j] = 0;
            else if (image[i][j] > highThreshold) // 检测高阈值
                image[i][j] = 255;
            else // 介于双阈值之间，连接边缘
            {
                // 检查邻域中是否有高于阈值点（排除孤立的局部极大值点)
                for (int x = -1; x < 2; ++x)
                {
                    if (flag) break;
                    for (int y = -1; y < 2; ++y)
                    {
                        if (image[i + x][j + y] > highThreshold)
                        {
                            image[i][j] = 255;
                            flag = true;
                            break;
                        }
                    }
                }
                if (!flag)
                    image[i][j] = 0;
            }
        }
    }
}

/* Canny 边缘检测 */
void Canny(Img<uchar> &image, const double weight)
{
    Img<double> theta(image.rows, image.cols);
    Img<uchar> grad(image.rows, image.cols);

    Sobel(image, grad, theta);
    NonMaxSuppression(grad, image, theta);
    DoubleThreshold(image, weight);
}
