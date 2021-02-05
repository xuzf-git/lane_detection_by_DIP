//
// Created by xuzf on 2021/2/6.
//

#include "../include/Canny.h"

/*********************************************************
 * Sobel 算子定义
 ********************************************************/

/* Sobel 算子：计算图像梯度 */
void Sobel(const Img<uchar> &src, Img<uchar> &dst, Img<int> &theta)
{
    assert(src.rows == dst.rows);
    assert(src.cols == dst.cols);

    const double sobelX_arr[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    const double sobelY_arr[3][3] = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
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
    Img<uchar> imgGradX(src.rows, src.cols);
    Img<uchar> imgGradY(src.rows, src.cols);
    sobelX.convolve(src, imgGradX);
    sobelY.convolve(src, imgGradY);
    for (int i = 0; i < src.rows; ++i)
    {
        for (int j = 0; j < src.cols; ++j)
        {
            dst[i][j] = sqrt(imgGradX[i][j] * imgGradX[i][j] + imgGradY[i][j] * imgGradY[i][j]);
        }
    }
    dst.show("Grad", 0);
}