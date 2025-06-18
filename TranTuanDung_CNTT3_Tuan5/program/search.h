#ifndef SEARCH_H
#define SEARCH_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cctype>

using namespace std;

struct DieuKienTim {
    string maPB;
    string maNV;
    string hoTen;
    string chucVu;
    string ngaySinhTu;
    string ngaySinhDen;
    string luongTu;
    string luongDen;
};


void chinhXac(NhanVien* danhSach, Key keyMenu);
string toLowerCase(string s);
string nhapGiaTriTimKiem(Key key);
string nhapGiaTriTimKiemChoNS(Key key);
bool isNumber(const string& s);
void timKiemNhanhToanCuc(NhanVien*& danhSach) ;
void drawDanhSachNhanVienHighlightToanCuc(NhanVien* head, int page, const string& keyword);

#endif