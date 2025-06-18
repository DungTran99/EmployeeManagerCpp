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

void textBoxInput(int x, int y, int width,int color = 7);
void showFileErrorPut();
void showAcceptPut();
void nhapProfile(NhanVien*& head);
void setProfileFrame();
bool setProfile(NhanVien*& head, const vector<string>& input);
bool checkError(const vector<string>& input, string& errorMessage, NhanVien* head);
bool isNumberhuh(const string &s, double &so, string &errorMessage);
bool CanParseDate(const string date, int &ngay, int &thang, int &nam, string &errorMessage);
bool isNameClean(const string &ten, string &errorMessage);
bool ToInt(const string &str, int &out);
bool isDouble(const string& s, double& out);
#endif