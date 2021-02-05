//
// Created by xuzf on 2021/2/5.
//

#include <iostream>
#include "../include/Img.h"

/* 构造空值图像 */
Img::Img(int rows, int cols)
{
    this->rows = rows;
    this->cols = cols;
    data = new uchar *[rows];
    for (int i = 0; i < rows; ++i)
    {
        data[i] = new uchar[cols];
        std::memset(data[i], 0, sizeof(uchar) * cols);
    }
}

/* 读入真实图像：灰度图 */
Img::Img(const char *path)
{
    cv::Mat img_mat = cv::imread(path, cv::IMREAD_GRAYSCALE);
    this->rows = img_mat.rows;
    this->cols = img_mat.cols;
    data = new uchar *[rows];
    for (int i = 0; i < this->rows; i++)
    {
        uchar *p = img_mat.ptr(i);
        data[i] = new uchar[cols];
        for (int j = 0; j < this->cols; j++)
        {
            data[i][j] = *p;
            p++;
        }
    }
}

/* 复制构造函数 */
Img::Img(Img &cp)
{
    this->rows = cp.rows;
    this->cols = cp.cols;
    data = new uchar *[this->rows];
    for (int i = 0; i < this->rows; i++)
    {
        data[i] = new uchar[this->cols];
        for (int j = 0; j < this->cols; j++)
        {
            data[i][j] = cp[i][j];
        }
    }
}

/* 析构函数 */
Img::~Img()
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] data[i];
    }
}

/* 访问图像的指定行 */
uchar *Img::operator[](const int idx) const
{
    return data[idx];
}

/* 将图像转换成 cv::Mat */
cv::Mat Img::toMat() const
{
    cv::Mat img_mat = cv::Mat::zeros(rows, cols, CV_8UC1);
    for (int i = 0; i < rows; ++i)
    {
        uchar *p = img_mat.ptr(i);
        for (int j = 0; j < cols; ++j)
        {
            *p = data[i][j];
            p++;
        }
    }
    return img_mat;
}

/* 展示图片 */
void Img::show(const char *name, int delay) const
{
    cv::imshow(name, this->toMat());
    cv::waitKey(delay);
}
