//
// Created by xuzf on 2021/2/11.
//

#ifndef LANE_DETECTION_SAVE_RESULT_H
#define LANE_DETECTION_SAVE_RESULT_H

#include "Img.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

const double h_samples[56] = {160, 170, 180, 190, 200, 210, 220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 320, 330, 340,
                              350, 360, 370, 380, 390, 400, 410, 420, 430, 440, 450, 460, 470, 480, 490, 500, 510, 520, 530,
                              540, 550, 560, 570, 580, 590, 600, 610, 620, 630, 640, 650, 660, 670, 680, 690, 700, 710};

/* 根据车道线的参数，获取坐标向量 */
void GetLanes(Img<uchar> &src, vector<pair<int, int>> &params, vector<vector<int> > &lanes);

/* 将检测结果写入json文件 */
void WriteJson(string &raw_file, vector<vector<int> > &lanes, double run_time, ofstream &of);

/* 展示车道线检测结果 */
void polyLanes(const string &path, vector<vector<int>> &lanes, vector<pair<int, int>> &params, int delay);
#endif //LANE_DETECTION_SAVE_RESULT_H
