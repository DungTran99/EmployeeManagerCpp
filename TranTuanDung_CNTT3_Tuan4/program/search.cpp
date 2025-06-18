#include "search.h"
#include "getProfile.h"
#include "showProfile.h"
#include "gui.h"

using namespace std;

string nhapGiaTriTimKiem(Key key) {
    int x = 74, y = 16;
    console::gotoxy(x,y);
    console::cursorStatus(5, true);
    string label;
    switch (key) {
        case MAPB: label = "Nhap tu khoa can tim: "; break;
        case MANV: label = "Nhap tu khoa can tim: "; break;
        case HOVATEN: label = "Nhap tu khoa can tim: "; break;
        case NGAYSINH: label = "Nhap ngay sinh (dd/mm/yyyy): "; break;
        case LUONG: label = "Nhap tu khoa can tim: "; break;
        case CHUCVU: label = "Nhap tu khoa can tim: "; break;
        case TENPB: label = "Nhap tu khoa can tim: "; break;
        default: label = "Nhap tu khoa can tim: "; break;
    }
    console::gotoxy(x,y);
    cout << label;
    string input;
    getline(cin, input);
    console::cursorStatus(5, false);
    return input;
}

bool isNumber(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

string toLowerCase(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}
string nhapGiaTriTimKiemChoNS() {
    int x = 74, y = 16;
    string label = "Nhap ngay sinh (dd/mm/yyyy)";
    for(int i = 0; i< 5; i++){
        console::clearLine(x-1, y-2+i, 45);
    }
    //ve khung
    drawBox(x-1,y-1,29,3,7);
    console::gotoxy(x,y); cout<<label;
    
    drawBox(x-1 + 30,y-1,4,3,3);
    console::gotoxy(x-1 + 30 + 4,y); cout<<"/";
    drawBox(x-1 + 35, y-1,4,3,3);
    console::gotoxy(x-1 + 39,y ); cout<<"/";
    drawBox(x-1 + 40, y-1,6,3,3);

    //lay du lieu ngay thang nam
    string ngay,thang,nam;
    console::gotoxy(x-1 + 30 + 1,y);
    console::cursorStatus(5, true); 
    getline(cin, ngay); 
    console::gotoxy(x-1 + 35 + 1, y);
    console::cursorStatus(5, true); 
    getline(cin, thang);
    console::gotoxy(x-1 + 40 + 1, y);
    console::cursorStatus(5, true); 
    getline(cin, nam);
    console::cursorStatus(5, false);
    //chuyen doi ngay thang nam
    if (!isNumber(ngay) || !isNumber(thang) || !isNumber(nam)) {
        return ""; // hoặc thông báo lỗi
    }

    int d = stoi(ngay);   
    int m = stoi(thang);
    string dStr = (d < 10 ? "0" : "") + to_string(d);
    string mStr = (m < 10 ? "0" : "") + to_string(m);
    string yStr = nam; while (yStr.length() < 4) yStr = "0" + yStr;

    return (dStr + "|" + mStr + "|" + yStr);
}

void chinhXac(NhanVien* danhSach, Key keyMenu) {
    int x = 74, y = 16;
    string giaTriTim;
    if(keyMenu == NGAYSINH){
        giaTriTim = nhapGiaTriTimKiemChoNS();
    }else{
        giaTriTim = nhapGiaTriTimKiem(keyMenu);
    }    

    if (keyMenu == HOVATEN) {
        giaTriTim = chuanHoaHoTen(giaTriTim); 
    }
    
    if (keyMenu == MAPB) {
        for(char &s : giaTriTim)
        s = toupper(s); 
    }

    if (keyMenu == CHUCVU) {
        giaTriTim[0] = toupper(giaTriTim[0]);
    }

    vector<NhanVien*> ketQua;
    for (NhanVien* p = danhSach; p != nullptr; p = p->next) {
        switch (keyMenu) {
            case MAPB:
                if (p->maPhongBan == giaTriTim) ketQua.push_back(p);
                break;
            case MANV:
                if (p->maNhanVien == giaTriTim) ketQua.push_back(p);
                break;
            case HOVATEN:
                if (p->hoTen == giaTriTim) ketQua.push_back(p);
                break;
            case NGAYSINH:
                if (p->ngaySinh == giaTriTim) ketQua.push_back(p);
                break;
            case CHUCVU:
                if (p->chucVu == giaTriTim) ketQua.push_back(p);
                break;
            case LUONG:
                if (stod(giaTriTim) == p->luong) ketQua.push_back(p);
                break;
            default:
                break;
        }
    }

    if (ketQua.empty()) {
        console::clearScreen();
        console::gotoxy(x, y);
        cout << "Khong tim thay ket qua nao.";
        console::gotoxy(x, y);
        cout << "Nhan phim bat ky de quay lai.";
        _getch();
        return;
    }

    // Tạo danh sách tạm để in
    NhanVien* head = nullptr;
    NhanVien* tail = nullptr;
    for (NhanVien* nv : ketQua) {
        NhanVien* newNode = new NhanVien(*nv); // copy
        newNode->prev = tail;
        newNode->next = nullptr;
        if (!head) head = newNode;
        else tail->next = newNode;
        tail = newNode;
    }

    controlPrintProfileHighlight(head, keyMenu, giaTriTim, danhSach);

    // Giải phóng bộ nhớ danh sách tạm
    while (head) {
        NhanVien* tmp = head;
        head = head->next;
        delete tmp;
    }

}

bool nhapTieuChiTimKiem(DieuKienTim& dk) {
    const vector<string> keys = {
        "Ma phong ban:",
        "Ma nhan vien:",
        "Ho va ten:",
        "Chuc vu:",
        "Ngay sinh tu (dd/mm/yyyy):",
        "Ngay sinh den (dd/mm/yyyy):",
        "Luong tu:",
        "Luong den:"
    };

    vector<string*> fields = {
        &dk.maPB, &dk.maNV, &dk.hoTen, &dk.chucVu,
        &dk.ngaySinhTu, &dk.ngaySinhDen,
        &dk.luongTu, &dk.luongDen
    };

    int selected = 0;
    char key;
    int xinchu = 8, yinchu = 3, width = 60;

    while (true) {
        console::cursorStatus(25, false);
        console::clearScreen();
        setProfileFrame();
        console::clearLine(2,1,100);
        console::gotoxy(2, 1);
        console::setColor(4);
        cout << "TIM KIEM THEO TIEU CHI:";
        console::resetColor();

        for (int i = 0; i < keys.size(); i++) {
            if (i == selected) {
                textBoxInput(xinchu - 2, yinchu - 1 + i * 3, width, 4);
                console::setColor(4);
            } else {
                textBoxInput(xinchu - 2, yinchu - 1 + i * 3, width);
            }

            console::gotoxy(xinchu, yinchu + i * 3);
            cout << keys[i];
            console::gotoxy(xinchu + keys[i].length() + 1, yinchu + i * 3);
            cout << *fields[i];
            console::resetColor();
        }

        console::gotoxy(xinchu + keys[selected].length() + fields[selected]->length() + 1, yinchu + selected * 3);
        console::cursorStatus(25, true);
        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_UP: selected = (selected + keys.size() - 1) % keys.size(); break;
            case KEY_DOWN:
            case KEY_TAB: selected = (selected + 1) % keys.size(); break;
            case KEY_BACKSPACE:
                if (!fields[selected]->empty()){ 
                    fields[selected]->pop_back();
                }
                break;
            case KEY_ENTER: return true;
            case KEY_ESC: return false;
            default:
                if (isprint(key)) {
                    if (fields[selected]->length() < width - keys[selected].length() - 5)
                        *fields[selected] += key;
                }
        }
    }
}
NhanVien* timKiemTheoTieuChi(NhanVien* danhSach, const DieuKienTim& dk) {
    NhanVien* head = nullptr, *tail = nullptr;

    for (NhanVien* p = danhSach; p; p = p->next) {
        bool match = true;

        // Tìm 1 phần mã phòng ban (không phân biệt hoa thường)
        if (!dk.maPB.empty() && toLowerCase(p->maPhongBan).find(toLowerCase(dk.maPB)) == string::npos)
            match = false;

        // Tìm 1 phần mã nhân viên
        if (!dk.maNV.empty() && toLowerCase(p->maNhanVien).find(toLowerCase(dk.maNV)) == string::npos)
            match = false;

        // Tìm 1 phần họ tên
        if (!dk.hoTen.empty() && toLowerCase(p->hoTen).find(toLowerCase(dk.hoTen)) == string::npos)
            match = false;

        // Tìm 1 phần chức vụ
        if (!dk.chucVu.empty() && toLowerCase(p->chucVu).find(toLowerCase(dk.chucVu)) == string::npos)
            match = false;

        // So sánh khoảng ngày sinh
        if (!dk.ngaySinhTu.empty() && p->ngaySinh < dk.ngaySinhTu)
            match = false;
        if (!dk.ngaySinhDen.empty() && p->ngaySinh > dk.ngaySinhDen)
            match = false;

        // So sánh khoảng lương
        if (!dk.luongTu.empty() && p->luong < stod(dk.luongTu))
            match = false;
        if (!dk.luongDen.empty() && p->luong > stod(dk.luongDen))
            match = false;

        if (match) {
            NhanVien* node = new NhanVien(*p);
            node->next = nullptr;
            node->prev = tail;
            if (!head) head = node;
            else tail->next = node;
            tail = node;
        }
    }

    return head;
}

void timKiemTheoTieuChiMenu(NhanVien*& danhSach) {
    DieuKienTim dk;

    // 1. Giao diện nhập điều kiện tìm kiếm
    if (!nhapTieuChiTimKiem(dk)) return;

    // 2. Thực hiện tìm kiếm
    NhanVien* ketQua = timKiemTheoTieuChi(danhSach, dk);

    // 3. Nếu không có kết quả
    if (ketQua == nullptr) {
        console::clearScreen();
        console::gotoxy(74, 16);
        cout << "Khong tim thay ket qua nao.";
        console::gotoxy(74, 17);
        cout << "Nhan phim bat ky de quay lai.";
        _getch();
        return;
    }

    // 4. Hiển thị kết quả với highlight theo nhiều điều kiện
    controlPrintProfileHighlightMulti(ketQua, dk, danhSach);

    // 5. Giải phóng bộ nhớ kết quả
    while (ketQua != nullptr) {
        NhanVien* temp = ketQua;
        ketQua = ketQua->next;
        delete temp;
    }
}

void timKiemNhanhToanCuc(NhanVien*& danhSach) {
    string tuKhoa = "";
    int page = 1;
    char key;

    while (true) {
        console::clearScreen();
        console::cursorStatus(25, false);

        // === Vẽ thanh tìm kiếm ===
        console::gotoxy(2, 1);
        console::setColor(4);
        cout << "Nhap thong tin can tim kiem: ";
        console::resetColor();
        cout << tuKhoa;

        // === Thực hiện tìm kiếm theo tất cả field ===
        NhanVien* kq = nullptr, *tail = nullptr;
        string keywordLower = toLowerCase(tuKhoa);

        for (NhanVien* p = danhSach; p; p = p->next) {
            bool match =
                toLowerCase(p->maPhongBan).find(keywordLower) != string::npos ||
                toLowerCase(p->tenPhongBan).find(keywordLower) != string::npos ||
                toLowerCase(p->maNhanVien).find(keywordLower) != string::npos ||
                toLowerCase(p->hoTen).find(keywordLower) != string::npos ||
                toLowerCase(p->chucVu).find(keywordLower) != string::npos ||
                toLowerCase(p->ngaySinh).find(keywordLower) != string::npos ||
                to_string((int)p->luong).find(tuKhoa) != string::npos;

            if (match) {
                NhanVien* copy = new NhanVien(*p);
                copy->next = nullptr;
                copy->prev = tail;
                if (!kq) kq = copy;
                else tail->next = copy;
                tail = copy;
            }
        }

        if (kq)
            drawDanhSachNhanVienHighlightToanCuc(kq, page, tuKhoa);
        else {
            console::gotoxy(2, 4);
            cout << "Khong tim thay ket qua.";
        }

        // Xóa kết quả tạm
        while (kq) {
            NhanVien* tmp = kq;
            kq = kq->next;
            delete tmp;
        }

        // === Nhận phím ===
        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_LEFT:  page--; break;
            case KEY_RIGHT: page++; break;
            case KEY_ESC:
            case KEY_ENTER:
                return;
            case KEY_BACKSPACE:
                if (!tuKhoa.empty()) tuKhoa.pop_back(); break;
            default:
                if (isprint(key)) tuKhoa += key;
        }
    }
}
void drawDanhSachNhanVienHighlightToanCuc(NhanVien* head, int page, const string& keyword) {
    int x = 2, y = 3;
    int total = demSoNhanVien(head);
    int totalPages = (total + soDongMotTrang - 1) / soDongMotTrang;
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    drawTableHeader(x, y);

    int start = (page - 1) * soDongMotTrang;
    int stt = start + 1;
    int row = y + 2;

    string keyLower = toLowerCase(keyword);

    NhanVien* p = head;
    for (int i = 0; i < start && p; ++i) p = p->next;

    while (p && row < y + 2 + soDongMotTrang) {
        console::setColor(14); // nền trắng mặc định
        console::gotoxy(x - 1, row);
        cout << setw(4) << right << stt++ << " | ";
        console::resetColor();

        auto match = [&](const string& field) {
            return toLowerCase(field).find(keyLower) != string::npos;
        };

        auto printField = [&](const string& field, int width) {
            if (match(field)) console::setColor(14); // vàng nếu khớp
            else console::setColor(15);              // nền trắng
            cout << setw(width) << left << field.substr(0, width);
            console::resetColor(); cout << " | ";
        };

        // PB: gộp giữa maPB và tenPB
        bool match_pb = match(p->tenPhongBan) || match(p->maPhongBan);
        if (match_pb) console::setColor(14);
        else console::setColor(15);
        cout << setw(19) << left << p->tenPhongBan.substr(0, 19);
        console::resetColor(); cout << " | ";

        printField(p->maNhanVien, 10);
        printField(p->hoTen, 25);
        printField(p->chucVu, 14);
        printField(p->ngaySinh, 10);

        string luongStr = to_string((int)p->luong);
        if (luongStr.find(keyword) != string::npos) console::setColor(14);
        else console::setColor(15);
        cout << setw(13) << fixed << setprecision(0) << p->luong;
        console::resetColor();

        ++row;
        p = p->next;
    }

    // Thanh điều hướng
    console::gotoxy(x + 15, MENU_Y + 1 + 12); console::setColor(11); cout << "<= Trang truoc";
    console::gotoxy(x + 35, MENU_Y + 1 + 12); cout << "Trang sau =>";
    console::gotoxy(x + 60, MENU_Y + 1 + 12); console::setColor(12); cout << "ESC/ENTER: Thoat";
    console::resetColor();
}
