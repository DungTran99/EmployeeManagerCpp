#include "getProfile.h"
#include "gui.h"
#include "showProfile.h"

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
        out << p->maPhongBan << "," << p->tenPhongBan << "," << p->maNhanVien << "," << p->hoTen << ","
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
        getline(ss, nv->tenPhongBan, ',');
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

void setProfile(NhanVien*& head) {
    console::clearScreen();
    NhanVien* nv = new NhanVien();
    string ngay, thang, nam;
    int pos = 0;

    setProfileFrame(); // vẽ khung nhập, hiển thị nhãn
    console::cursorStatus(25,true);
    while (true) {
        switch (pos) {
            case 0: // Mã nhân viên
                console::gotoxy(22, 5); // ví dụ vị trí
                getline(cin, nv->maPhongBan);
                break;

            case 1: // Mã nhân viên
                console::gotoxy(24, 8); // ví dụ vị trí
                getline(cin, nv->tenPhongBan);
                break;
            case 2: // Họ tên
                console::gotoxy(20, 11);
                getline(cin, nv->hoTen);
                nv->hoTen = chuanHoaHoTen(nv->hoTen);
                break;
            case 3: // Chức vụ
                console::gotoxy(20, 14);
                getline(cin, nv->chucVu);
                break;
            case 4: // Ngày/Tháng/Năm
                console::gotoxy(20, 17);
                getline(cin, ngay);
                console::gotoxy(23, 17);
                cout<<"/";
                getline(cin, thang);
                console::gotoxy(26, 17);
                cout<<"/";
                getline(cin, nam);
                break;
            case 5: // Lương
                console::gotoxy(20, 20);
                string sLuong;
                getline(cin, sLuong);
                try {
                    nv->luong = stod(sLuong);
                } catch (...) {
                    nv->luong = -1;
                }
                if (!kiemTraLuong(nv->luong)) {
                    // báo lỗi
                    continue;
                }
                break;
        }

        // Xử lý điều hướng
        char key = _getch();
        if (key == -32) key = _getch();
        switch (key) {
            case KEY_UP:
                pos = (pos - 1 + 7) % 7;
                break;
            case KEY_DOWN:
                pos = (pos + 1) % 7;
                break;
            case KEY_ENTER:
                if (pos == 6) { // cuối cùng
                    break;
                } else {
                    pos++;
                }
                break;
        }

        // nếu nhập hết thì validate ngày/tháng/năm
        if (pos == 6 && !ngay.empty() && !thang.empty() && !nam.empty()) {
            string ngaySinh = (ngay.length() == 1 ? "0" + ngay : ngay) + "/" +
                              (thang.length() == 1 ? "0" + thang : thang) + "/" + nam;
            if (!kiemTraNgaySinh(ngaySinh)) {
                // báo lỗi
                pos = 3; // quay lại nhập ngày
                continue;
            }
            nv->ngaySinh = ngaySinh;
        }

        if (key == KEY_ESC) {
            delete nv;
            return;
        }

        if (key == KEY_ENTER && pos == 6) break;
    }

    // Thêm vào danh sách liên kết
    nv->next = nullptr;
    nv->prev = nullptr;
    if (!head) head = nv;
    else {
        NhanVien* tail = head;
        while (tail->next) tail = tail->next;
        tail->next = nv;
        nv->prev = tail;
    }
    console::cursorStatus(25,false);
}



void setProfileFrame(){
    console::clearScreen();
    int x = 0, y = 0, width = 60;
    
    // Tiêu đề
    console::gotoxy(x+2, y+1);
    console::setColor(4);
    cout << "THEM NHAN VIEN MOI";
    console::resetColor();

    // Thông tin file
    console::gotoxy(x + 20, y+1);
    cout << " - FILE: "<<tenFile;
    
    // Khung nhập liệu

    inputFrame(x + 6,y + 4,width);
    console::gotoxy(x + 7, y + 5);
    cout << "Ma phong ban: ";

    inputFrame(x + 6,y + 7,width);
    console::gotoxy(x + 7, y + 8);
    cout << "Ten phong ban: ";
    
    inputFrame(x + 6,y + 10,width);
    console::gotoxy(x + 7, y + 11);
    cout << "Ho va ten: ";

    inputFrame(x + 6,y + 13,width);
    console::gotoxy(x + 7, y + 14);
    cout << "Chuc vu: ";

    inputFrame(x + 6,y + 16,width);
    console::gotoxy(x + 7, y + 17);
    cout << "Ngay sinh: ";

    inputFrame(x + 6,y + 19,width);
    console::gotoxy(x + 7, y + 20);
    cout << "Luong: ";

    
    // Các nút điều hướng (tương tự như trong drawDanhSachNhanVien)
    console::gotoxy(x + 15, MENU_Y+1 + 12);
    console::setColor(11);
    cout << "Luu(Enter)";
    console::gotoxy(x, MENU_Y+1 + 12);
    console::setColor(12);
    cout << "Thoat(ESC)";
    console::resetColor();
}



void clearLine(int x, int y, int width) {
    console::gotoxy(x, y);
    for (int i = 0; i < width; ++i) cout << " ";
}
//khung nhap file
void inputFrame(int x, int y, int width){
    console::gotoxy(x, y);
    cout << char(218); // ┌ góc trên trái
    for (int i = 0; i < width - 2; ++i) cout << char(196); // ─ đường ngang
    cout << char(191); // ┐ góc trên phải

    // Viền giữa (2 cạnh dọc)
    console::gotoxy(x, y + 1);
    cout << char(179); // │	trái
    console::gotoxy(x + width - 1, y + 1);
    cout << char(179); // │	phải

    // Viền dưới
    console::gotoxy(x, y + 2);
    cout << char(192); // └ góc dưới trái
    for (int i = 0; i < width - 2; ++i) cout << char(196); // ─ đường ngang
    cout << char(217); // ┐ góc dưới phải

    
}
//nhap ten file
bool setNameFile(){
    int x = MENU_X + 50, y = MENU_Y, width = 60;
    inputFrame(x,y,width);
    // Label
    console::gotoxy(x + 2, y + 1);
    cout << "Nhap ten file: ";
    // Gợi ý
    drawBox(x, y + 3, 45, 4, 4);
    console::gotoxy(x+2, y + 4);
    cout<<"Nhan nut bat ky de tiep tuc!";
    console::gotoxy(x+2, y + 5);
    cout<<"Nhan ESC hoac LEFT ARROW DE THOAT";

    char key;
    key = _getch();
    if (key == -32 || key == 0) key = _getch();
    switch(key){
        case KEY_ENTER:
        case KEY_RIGHT:
            break;
        case KEY_LEFT:
        case KEY_ESC:
            return false;
    }
    
    console::cursorStatus(25,TRUE);
    
    const string fileMacDinh = "Dulieu.dat";

    // Gợi ý
    for (int i = 0; i < 2; ++i) {
        console::clearLine(x+2, y + 4 + i, 30);
    }
    drawBox(x, y + 3, 45, 4, 4);
    console::gotoxy(x+2, y + 4);
    cout<<"File mac dinh: Dulieu.dat";
    console::gotoxy(x+2, y + 5);
    cout<<"Nhan Enter neu khong can nhap file moi!";

    console::gotoxy(x + 18, y + 1);
    getline(cin, tenFile); // nhập tên file

    // Nếu người dùng không nhập gì thì dùng file mặc định
    if (tenFile.empty()) {
        tenFile = fileMacDinh;
        console::gotoxy(x + 18, y + 1);
        cout << fileMacDinh; // hiện lại tên mặc định lên
    }

    // Thông báo đã chọn
    for (int i = 0; i < 2; ++i) {
        console::clearLine(x+2, y + 4 + i, 40);
    }
    drawBox(x, y + 3, 45, 4, 4);
    console::gotoxy(x+2, y + 4);
    cout << "Da chon file: " << tenFile;
    console::gotoxy(x+2, y + 5);
    cout << "Nhan phim bat ky de tiep tuc...";
    console::cursorStatus(25,FALSE);
    _getch();
    // Xóa khung nhập
    for (int i = 0; i < 7; ++i) {
        clearLine(x, y + i, width);
    }
    return true;
}

void importByFile(NhanVien*& head){
    if(!setNameFile()){
        return;
    }
    if(docFile(tenFile, head)){
        controlPrintProfile(head);
    }else{
        showErrorPut();
    }
}

void outputByFile(NhanVien*& head){
    if(!setNameFile()){
        return;
    }
    if(ghiFile(tenFile, head)){
        showAcceptPut();
    }else{
        showErrorPut();
    }
}

void showErrorPut(){
    int x = MENU_X + 50, y = MENU_Y;
    for (int i = 0; i < 2; ++i) {
        console::clearLine(x+2, y + 4 + i, 40);
    }
    drawBox(x, y + 3, 45, 4, 4);
    console::gotoxy(x+2, y + 4);
    cout << "File khong ton tai!";
    console::gotoxy(x+2, y + 5);
    cout << "Nhan phim bat ky de tiep tuc...";
    console::cursorStatus(25,FALSE);
    cin.get();
}

void showAcceptPut(){
    int x = MENU_X + 50, y = MENU_Y;
    for (int i = 0; i < 2; ++i) {
        console::clearLine(x+2, y + 4 + i, 60);
    }
    drawBox(x, y + 3, 45, 4, 4);
    console::gotoxy(x+2, y + 4);
    cout << "Xuat File thanh cong!";
    console::gotoxy(x+2, y + 5);
    cout << "Nhan phim bat ky de tiep tuc...";
    console::cursorStatus(25,FALSE);
    cin.get();
}