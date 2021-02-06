#include <iostream>
#include <string>
#include "include/Img.hpp"
#include "include/Kernel.h"
#include "include/Canny.h"

using namespace std;

int main(int argc, char const *argv[])
{
    string path = "../data/lena.jpg";
    Img<uchar> src(path.data());
    Img<uchar> dst(src.rows, src.cols);
    Img<double> theta(src.rows, src.cols);

    // 原图
    src.show("OriginImg", 1000);
    // 高斯滤波
    GaussianKernel filter(3, 1);
    filter.convolve(src, dst, true);
    // Canny 边缘检测
    Canny(dst, 0.9);
    dst.show("edge detection", 0);
    // opencv
    // cv::Mat cv_img = cv::imread("../data/20.jpg", cv::IMREAD_GRAYSCALE);
    // cv::Mat cv_dst;
    // cv::Canny(cv_img, cv_dst, 150, 100);
    // cv::imshow("opencv", cv_dst);
    // cv::waitKey(0);
    return 0;
}