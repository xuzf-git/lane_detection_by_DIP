//
// Created by xuzf on 2021/2/5.
//

#ifndef LANE_DETECTION_IMG_HPP
#define LANE_DETECTION_IMG_HPP

#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>
//typedef unsigned char uchar;
const double PI = acos(-1.0); // 圆周率

template<typename T>
class Img
{
public:
    T **data;   // 存放数据
    int rows; // 图像的行数
    int cols; // 图像的列数

    Img(int rows, int cols); /* 构造空值图像 */
    Img(const char *path);   /* 读入图像：灰度图 */
    Img(Img &cp);            /* Img类的复制构造函数 */
    ~Img();

    T *operator[](const int idx) const;
    Img &operator=(const Img &cp);

    void show(const char *name, int delay) const; /* 展示图片 */
private:
    cv::Mat toMat() const;                        /* 将图像转换成 cv::Mat */
};

/* 构造空值图像 */
template<typename T>
Img<T>::Img(int rows, int cols) : rows(rows), cols(cols)
{
    data = new T *[rows];
    for (int i = 0; i < rows; ++i)
    {
        data[i] = new T[cols];
        std::memset(data[i], 0, sizeof(uchar) * cols);
    }
}

/* 读入真实图像：灰度图 */
template<typename T>
Img<T>::Img(const char *path) : rows(0), cols(0)
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
template<typename T>
Img<T>::Img(Img &cp) : rows(cp.rows), cols(cp.cols)
{
    data = new T *[this->rows];
    for (int i = 0; i < this->rows; i++)
    {
        data[i] = new T[this->cols];
        for (int j = 0; j < this->cols; j++)
        {
            data[i][j] = cp[i][j];
        }
    }
}

/* 析构函数 */
template<typename T>
Img<T>::~Img()
{
    for (int i = 0; i < rows; ++i)
    {
        delete[] data[i];
    }
}

/* 访问图像的指定行 */
template<typename T>
T *Img<T>::operator[](const int idx) const
{
    return data[idx];
}

template<typename T>
Img<T> &Img<T>::operator=(const Img &cp)
{
    assert(this->rows == cp.rows);
    assert(this->cols == cp.cols);
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            data[i][j] = cp[i][j];
        }
    }
    return *this;
}

/* 将图像转换成 cv::Mat */
template<typename T>
cv::Mat Img<T>::toMat() const
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
template<typename T>
void Img<T>::show(const char *name, int delay) const
{
    cv::imshow(name, this->toMat());
    cv::waitKey(delay);
}

#endif //LANE_DETECTION_IMG_HPP
