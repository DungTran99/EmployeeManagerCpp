#ifndef STATISTIC_H
#define STATISTIC_H

#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>
#include <cmath>
#include "getProfile.h"

using namespace std;

// Định nghĩa cấu trúc mức lương
struct MucLuong {
    double tu;
    double den;
    string ten;
    int soLuong;
};

void drawFrameThongKePhongBan(int x, int y, int tongNV);
void drawFrameThongKeChucVu(int x, int y, int tongNV);
void drawFrameThongKeLuong(int x, int y, int tongNV);
void drawBarChart(int x, int y, const string labels[], const int values[], int n, int total);
void drawPieChart(int x, int y, const vector<pair<string, int>>& data, int total);
void drawHistogramLuong(int x, int y, const vector<MucLuong>& mucLuong, int total);
void drawFrameThongKeLuongTheoPB(int x, int y);

void thongKePhongBan(NhanVien* head, bool showChart = false);
void thongKeChucVu(NhanVien* head, bool showChart = false);
void thongKeMucLuong(NhanVien* head, bool showChart = false);
void thongKeLuongTheoPB(NhanVien* head);

void controlThongKePhongBan(NhanVien* head);
void controlThongKeChucVu(NhanVien* head);
void controlThongKeMucLuong(NhanVien* head);
void controlThongKeLuongTheoPB(NhanVien* head);

extern string tenFile;

#endif