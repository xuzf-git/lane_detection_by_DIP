//
// Created by xuzf on 2021/2/5.
//

#ifndef LANE_DETECTION_IMG_H
#define LANE_DETECTION_IMG_H

#include <opencv2\opencv.hpp>
#include <string>
//typedef unsigned char uchar;

class Img
{
public:
    uchar** data;   // 存放数据
    int rows;   // 图像的函数
    int cols;   // 图像的列数

    Img(int rows, int cols);
    Img(std::string &path);
    Img(Img &cp);
    ~Img();

    uchar* operator[](const int idx) const;
    cv::Mat toMat() const;
    void show(std::string &name, int delay) const;
};


#endif //LANE_DETECTION_IMG_H
