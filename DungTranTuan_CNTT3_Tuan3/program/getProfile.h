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
    string tenPhongBan;
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
bool ghiFile(const string& tenFile, NhanVien* head);
bool docFile(const string& tenFile, NhanVien*& head);
void outputByFile(NhanVien*& head);
void importByFile(NhanVien*& head);

//

void inputFrame(int x, int y, int width);
void showErrorPut();
void showAcceptPut();

void setProfileFrame();
void setProfile(NhanVien*& head);


#endif