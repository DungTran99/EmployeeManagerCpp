#include "getprofile.h"
#include "gui.h"
using namespace std;

bool kiemTraMaNhanVien(const string& ma) {
    if (ma.length() != 8) return false;
    for (char c : ma)
        if (!isdigit(c)) return false;
    return true;
}

string chuanHoaHoTen(string hoTen) {
    stringstream ss(hoTen);
    string token, result = "";
    while (ss >> token) {
        token[0] = toupper(token[0]);
        for (size_t i = 1; i < token.length(); ++i)
            token[i] = tolower(token[i]);
        result += token + " ";
    }
    return result.empty() ? "" : result.substr(0, result.size() - 1);
}

bool laNamNhuan(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

bool laNgayHopLe(int d, int m, int y) {
    if (d < 1 || m < 1 || m > 12 || y < 1900) return false;
    vector<int> ngayThang = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (laNamNhuan(y)) ngayThang[1] = 29;
    return d <= ngayThang[m - 1];
}

bool du18Tuoi(int d, int m, int y) {
    time_t t = time(0);
    tm* now = localtime(&t);
    int namHienTai = now->tm_year + 1900;
    int thangHT = now->tm_mon + 1;
    int ngayHT = now->tm_mday;

    if (namHienTai - y > 18) return true;
    if (namHienTai - y == 18) {
        if (m < thangHT || (m == thangHT && d <= ngayHT)) return true;
    }
    return false;
}

bool kiemTraNgaySinh(const string& ngay) {
    if (ngay.length() != 10 || ngay[2] != '/' || ngay[5] != '/') return false;
    int d = stoi(ngay.substr(0, 2));
    int m = stoi(ngay.substr(3, 2));
    int y = stoi(ngay.substr(6, 4));
    return laNgayHopLe(d, m, y) && du18Tuoi(d, m, y);
}

bool kiemTraLuong(double luong) {
    return luong >= 0;
}

bool ghiFile(const string& tenFile, NhanVien* head) {
    ofstream out(tenFile);
    if (!out) return false;
    for (NhanVien* p = head; p != nullptr; p = p->next) {
        out << p->maPhongBan << "," << p->maNhanVien << "," << p->hoTen << ","
            << p->chucVu << "," << p->ngaySinh << "," << p->luong << "\n";
    }
    out.close();
    return true;
}

bool docFile(const string& tenFile, NhanVien*& head) {
    ifstream in(tenFile);
    if (!in) return false;
    string line;
    NhanVien* tail = nullptr;
    while (getline(in, line)) {
        stringstream ss(line);
        string tmp;
        NhanVien* nv = new NhanVien();
        getline(ss, nv->maPhongBan, ',');
        getline(ss, nv->maNhanVien, ',');
        getline(ss, nv->hoTen, ',');
        getline(ss, nv->chucVu, ',');
        getline(ss, nv->ngaySinh, ',');
        getline(ss, tmp, '\n');
        nv->luong = stod(tmp);
        nv->next = nullptr;
        nv->prev = tail;
        if (!head) head = nv;
        if (tail) tail->next = nv;
        tail = nv;
    }
    in.close();
    return true;
    
}