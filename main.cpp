#include <iostream>
#include <string>
#include "include/Img.h"
#include "include/Kernel.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string path = "../data/20.jpg";
    Img data(path.data());
    // cout << data.rows << ' ' << data.cols << endl;
    // 原图
    // data.show("OriginImg", 1000);
    // 高斯滤波
    Img smooth_dst(data.rows, data.cols);
    GaussianKernel filter(3,1);
    filter.convolve(data, smooth_dst);
    // smooth_dst.show("GaussianRes", 1000);
    // Sobel 算子
    Img sobel_img(data.rows, data.cols);
    Sobel(smooth_dst, sobel_img);
    return 0;
}