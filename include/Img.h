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
    uchar **data;   // 存放数据
    int rows;   // 图像的函数
    int cols;   // 图像的列数

    Img(int rows, int cols);    /* 构造空值图像 */
    Img(const char *path);     /* 读入图像：灰度图 */
    Img(Img &cp);               /* Img类的复制构造函数 */
    ~Img();

    uchar *operator[](const int idx) const;

    cv::Mat toMat() const;      /* 将图像转换成 cv::Mat */
    void show(const char *name, int delay) const;  /* 展示图片 */
};


#endif //LANE_DETECTION_IMG_H
