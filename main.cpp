#include <io.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "include/Img.hpp"
#include "include/Kernel.h"
#include "include/Edge.h"
#include "include/Hough.h"
#include "include/SaveResult.h"

using namespace std;
using namespace cv;

/* 获取所有待处理的图片路径 */
void get_image_names(const string &inPath, vector<string> &file_names)
{
    string strFind = inPath + "/*";
    _finddata_t fileInfo{};
    intptr_t hFile = _findfirst(strFind.c_str(), &fileInfo);
    if (hFile == -1)
        return;
    do
    {
        // 如果为文件夹，则递归遍历
        if (fileInfo.attrib & _A_SUBDIR)
        {
            if ((strcmp(fileInfo.name, ".") != 0) && (strcmp(fileInfo.name, "..") != 0))
            {
                get_image_names(inPath + "/" + fileInfo.name, file_names);
            }
        }
        // 如果为单个文件直接push_back
        else
        {
            file_names.emplace_back(inPath + "/" + fileInfo.name);
        }

    } while (!_findnext(hFile, &fileInfo));

    _findclose(hFile);
}

int main()
{
    // 获取所有图片的路径
    vector<string> file_names;
    get_image_names("../data/selected test data", file_names);

    // 输出文件流接口
    ofstream out;
    // out.open("../result/best_predict.json", ios::out);
    out.open("../result/predict.json", ios::out);

    // 记录 run_time 运行时间
    clock_t begin_time, end_time;

    // 车道线检测
    for (auto &path : file_names)
    {
        begin_time = clock();
        Img<uchar> src(path.data());
        Img<uchar> dst(src.rows, src.cols);
        Img<uchar> dst_close(src.rows, src.cols);
        Img<double> theta(src.rows, src.cols);

        // 原图
        // src.show("origin", 10);

        // 高斯滤波
        GaussianKernel filter(3, 1);
        filter.convolve(src, dst, true);
        // dst.show("gaussian", 10);

        // Canny 边缘检测
        Canny(dst, 0.97);
        // dst.show("canny", 10);

        // 获取图像 Roi
        RoiMask(dst);
        // dst.show("roi mask", 10);

        // hough 变换
        vector<pair<int, int>> lanes_param;
        HoughTransform(dst, lanes_param, 100);

        // 将车道线转成标准格式
        vector<vector<int>> lanes;
        GetLanes(dst, lanes_param, lanes);
        end_time = clock();

        // 将预测结果保存在 json 文件中
        string raw_path = path;
        raw_path.replace(raw_path.find("../data/selected test data"), 26, "clips");
        WriteJson(raw_path, lanes, (double) (end_time - begin_time) / CLOCKS_PER_SEC, out);

        // 绘图展示检测出的直线
        // dst.show("final", 10);
        polyLanes(path, lanes, lanes_param, 50);
        cout << lanes_param.size() << endl;
    }
    out.close();

    return 0;
}
