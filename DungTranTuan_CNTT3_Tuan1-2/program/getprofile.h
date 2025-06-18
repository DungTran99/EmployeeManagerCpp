#ifndef GETPROFILE_H
#define GETPROFILE_H
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <cctype>
using namespace std;

typedef struct NhanVien {
    string maPhongBan;
    string maNhanVien;
    string hoTen;
    string chucVu;
    string ngaySinh; // dd/mm/yyyy
    double luong;

    NhanVien* prev = nullptr;
    NhanVien* next = nullptr;
}nv;

// ====== Kiem tra va chuan hoa ======
bool kiemTraMaNhanVien(const string& ma);
bool kiemTraNgaySinh(const string& ngay);
bool du18Tuoi(int d, int m, int y);
bool kiemTraLuong(double luong);
bool laNgayHopLe(int d, int m, int y);
string chuanHoaHoTen(string hoTen);

// ====== File ======
bool ghiFile(const string& tenFile, nv* head);
bool docFile(const string& tenFile, nv*& head);

NhanVien* danhSach = nullptr;
#endif