#include <iostream>
#include <string>
#include "include/Img.hpp"
#include "include/Kernel.h"
#include "include/Canny.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string path = "../data/20.jpg";
    Img<uchar> data(path.data());
    // cout << data.rows << ' ' << data.cols << endl;
    // 原图
    // data.show("OriginImg", 1000);
    // 高斯滤波
    Img<uchar> smooth_dst(data.rows, data.cols);
    GaussianKernel filter(3, 1);
    filter.convolve(data, smooth_dst);
    // smooth_dst.show("GaussianRes", 1000);
    // Sobel 算子
    Img<uchar> sobel_img(data.rows, data.cols);
    Img<int> theta(data.rows, data.cols);
    Sobel(smooth_dst, sobel_img, theta);
    return 0;
}