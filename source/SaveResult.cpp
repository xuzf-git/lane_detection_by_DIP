//
// Created by xuzf on 2021/2/11.
//

#include "../include/SaveResult.h"

void GetLanes(Img<uchar> &src, vector<pair<int, int>> &params, vector<vector<int>> &lanes)
{
    double sin_val, cos_val, tan_val, r;
    int val;
    bool is_empty;
    for (auto & param : params)
    {
        vector<int> lane;
        sin_val = sin(param.first * PI / 180);
        cos_val = cos(param.first * PI / 180);
        tan_val = sin_val / cos_val;
        r = param.second;
        is_empty = true;

        for (double h_sample : h_samples)
        {
            val = int((r * sin_val - h_sample) * tan_val + r * cos_val);
            if (val < 0 || val >= src.cols || h_sample <= 230)
                val = -2;
            lane.push_back(val);
            if (val != -2 && is_empty)
                is_empty = false;
        }
        if (!is_empty)
            lanes.push_back(lane);
        lane.clear();
    }
}

void WriteJson(string &raw_file, vector<vector<int> > &lanes, double run_time, ofstream &of)
{
    // 打印 lanes
    of << R"({"lanes": [)";
    for (int i = 0; i < lanes.size() && i < 4; ++i)
    {
        of << "[";
        for (int j = 0; j < 55; ++j)
        {
            of << lanes[i][j] << ", ";
        }
        of << lanes[i][55] << "]";
        if (i != lanes.size() - 1 && i != 3)
            of << ", ";
    }
    of << "], ";

    // 打印 h_samples
    of << R"("h_samples": [)";
    for (int j = 0; j < 55; ++j)
    {
        of << h_samples[j] << ", ";
    }
    of << h_samples[55] << "], ";

    // 打印 raw_file
    of << R"("raw_file": ")" << raw_file << R"(", )";

    // 打印 run_time
    of << R"("run_time": )" << run_time << " }" << endl;
}

void polyLanes(const string &path, vector<vector<int>> &lanes, vector<pair<int, int>> &params, int delay)
{
    cv::Mat src_mat = cv::imread(path);
    cv::Mat show_img;
    cv::Point2d last_point(-2, -2);
    cv::Point2d current_point;
    int k = 0;
    for (auto &lane : lanes)
    {
        if (k >= 4)
            break;
        k++;
        last_point.y = -2;
        last_point.x = -2;
        for (int j = 0; j < 56; ++j)
        {
            if (lane[j] < 0)
                continue;
            if (last_point.y < 0)
            {
                last_point.y = h_samples[j];
                last_point.x = lane[j];
                continue;
            }
            current_point.y = h_samples[j];
            current_point.x = lane[j];
            cv::line(src_mat, last_point, current_point, cv::Scalar(0, 255, 0), 2);
            last_point.x = current_point.x;
            last_point.y = current_point.y;
        }
    }
    // cv::Point2d pt1, pt2;
    // for (auto param : params)
    // {
    //     // 极坐标中的r长度
    //     float rho = param.second;
    //     // 极坐标中的角度
    //     float theta = param.first * PI / 180;
    //     double a = cos(theta), b = sin(theta);
    //     double x0 = a*rho, y0 = b*rho;
    //     // 转换为平面坐标的四个点
    //     pt1.x = cvRound(x0 + 2000 * (-b));
    //     pt1.y = cvRound(y0 + 2000 * (a));
    //     pt2.x = cvRound(x0 - 2000 * (-b));
    //     pt2.y = cvRound(y0 - 2000 * (a));
    //     line(src_mat, pt1, pt2, cv::Scalar(0, 0, 255), 2);
    // }
    // 缩放图片
    // cv::resize(src_mat, show_img, cv::Size(src_mat.cols / 2.0, src_mat.rows / 2.0), 0, 0, cv::INTER_LINEAR);
    cv::imshow("lines", src_mat);
    cv::waitKey(delay);
}
