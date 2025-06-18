#include "gui.h" 
#include "showProfile.h"
#include "getProfile.h"
#include "search.h"

using namespace std;

//ham dem so nhan vien
int demSoNhanVien(NhanVien* head) {
    int count = 0;
    for (NhanVien* p = head; p != nullptr; p = p->next)
        count++;
    return count;
}

void drawTableHeader(int x, int y) {
    console::gotoxy(x, y);
    cout << "STT | Phong ban           | Ma NV      | Ho ten                    | Chuc vu        | Ngay sinh  | Luong";
    console::gotoxy(x, y + 1);
    cout << "----|---------------------|------------|---------------------------|----------------|------------|---------------";
}

void drawDanhSachNhanVien(NhanVien* head, int page) {
    console::clearScreen();
    int x = 2, y = 3;
    int total = 0; // tong so nhan vien
    NhanVien* p = head;

    // Đếm tổng số
    total = demSoNhanVien(head);

    int totalPages = (total + soDongMotTrang - 1) / soDongMotTrang; //tong so trang
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;
    //page = trang hien tai
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "DANH SACH NHAN VIEN: "<<tenFile<<" - Tong: " << total << " - Trang " << page << "/" << totalPages;
    console::resetColor();
    //ve khung dau dong
    drawTableHeader(x, y);

    int start = (page - 1) * soDongMotTrang;
    int stt = start + 1;
    int row = y + 2;
    int count = 0;

    p = head;
    for (int i = 0; i < start && p; ++i)
        p = p->next;

    while (p && count < soDongMotTrang) {
        console::gotoxy(x-1, row++);
        cout << setw(4) << right << stt++ << " | "
             << setw(19) << left << p->tenPhongBan.substr(0, 22) << right << " | "
             << setw(10) << p->maNhanVien << " | "
             << setw(25) << left << p->hoTen.substr(0, 20) << right << " | "
             << setw(14) << left << p->chucVu.substr(0, 11) << right << " | "
             << setw(10) << p->ngaySinh << " | "
             << setw(13) << fixed << setprecision(0) << p->luong;

        p = p->next;
        count++;
    }
    // Các setw và left/right dùng để canh chỉnh cột
    // substr() cắt bớt chuỗi dài quá
    // setprecision(0) + fixed để in lương không có số thập phân

    // Thanh điều hướng
    console::gotoxy(x + 15,  MENU_Y+1 + 12);
    console::setColor(11);
    cout << "<= Trang truoc";
    console::gotoxy(x + 35,  MENU_Y+1 + 12);
    cout << "Trang sau =>";
    console::gotoxy(x + 60,  MENU_Y+1 + 12);
    console::setColor(12);
    cout << "Thoat(ESC-ENTER)";
    console::resetColor();
}

//dieu khien trang
void controlPrintProfile(NhanVien* head){
    int page = 1;
    int tongSoNhanVien = demSoNhanVien(head);
    //tinh so dong can hien thi trong 1 trong
    int totalPages = (tongSoNhanVien + soDongMotTrang - 1) / soDongMotTrang;

    char key;
    while (true) {
        drawDanhSachNhanVien(head, page); // hien thi trang hien tai

        // Thao tac phim
        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_RIGHT:// right arrow
                page++;
                if(page > totalPages) page = 1;
                break;
            case KEY_LEFT:// left arrow
                page--;
                if(page <= 0) page = totalPages;
                break;
            case KEY_ENTER:
            case KEY_ESC:
                return;
        }
    }
}


void drawDanhSachNhanVienHighlight(NhanVien* head, int page, Key key, const string& searchValue) {
    console::clearScreen();
    int x = 2, y = 3;
    int total = demSoNhanVien(head);
    int totalPages = (total + soDongMotTrang - 1) / soDongMotTrang;

    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "KET QUA TIM KIEM (highlight): " << total << " ket qua - Trang " << page << "/" << totalPages;
    console::resetColor();
    drawTableHeader(x, y);

    int start = (page - 1) * soDongMotTrang;
    int stt = start + 1;
    int row = y + 2;
    int count = 0;

    NhanVien* p = head;
    for (int i = 0; i < start && p; ++i)
        p = p->next;

    while (p && count < soDongMotTrang) {
        // === Kiểm tra khớp ===
        bool isMatchedRow = false;
        if (key == MAPB) isMatchedRow = (p->maPhongBan == searchValue);
        else if (key == TENPB) isMatchedRow = (p->tenPhongBan == searchValue);
        else if (key == MANV) isMatchedRow = (p->maNhanVien == searchValue);
        else if (key == HOVATEN) isMatchedRow = (p->hoTen == searchValue);
        else if (key == CHUCVU) isMatchedRow = (p->chucVu == searchValue);
        else if (key == NGAYSINH) isMatchedRow = (p->ngaySinh == searchValue);
        else if (key == LUONG) isMatchedRow = (stod(searchValue) == p->luong);

        if (isMatchedRow) console::setColor(10); // nền xanh

        console::gotoxy(x-1, row);
        cout << setw(4) << right << stt++ << " | ";

        // === In Tên phòng ban (ngay cả khi tìm theo mã) ===
        string tenPB = p->tenPhongBan.substr(0, 20);
        if (key == MAPB && p->maPhongBan == searchValue) {
            console::setColor(14); cout << setw(19) << left << tenPB; 
            if (isMatchedRow) console::setColor(10);
        } else {
            cout << setw(19) << left << tenPB;
        }
        cout << " | ";

        // === Hàm tô màu phần còn lại ===
        auto highlight = [&](const string& field, Key k, int width) {
            if (k == key && field == searchValue) {
                console::setColor(14);
                cout << setw(width) << left << field.substr(0, width);
                if (isMatchedRow) console::setColor(10);
            } else {
                cout << setw(width) << left << field.substr(0, width);
            }
            cout << " | ";
        };

        highlight(p->maNhanVien, MANV, 10);
        highlight(p->hoTen, HOVATEN, 25);
        highlight(p->chucVu, CHUCVU, 14);
        highlight(p->ngaySinh, NGAYSINH, 10);

        // Lương
        if (key == LUONG && stod(searchValue) == p->luong) {
            console::setColor(14); 
            cout << setw(13) << fixed << setprecision(0) << p->luong;
            if (isMatchedRow) console::setColor(10);
        } else {
            cout << setw(13) << fixed << setprecision(0) << p->luong;
        }

        console::resetColor();
        row++;
        count++;
        p = p->next;
    }


    // hint
    console::gotoxy(x + 15, MENU_Y + 1 + 12);
    console::setColor(11);
    cout << "<= Trang truoc";
    console::gotoxy(x + 35, MENU_Y + 1 + 12);
    cout << "Trang sau =>";
    console::gotoxy(x + 55, MENU_Y + 1 + 12);
    cout << "Nhan TAB de hien toan bo";
    console::gotoxy(x + 90, MENU_Y + 1 + 12);
    console::setColor(12);
    cout << "Thoat(ESC-ENTER)";
    console::resetColor();
}

void controlPrintProfileHighlight(NhanVien* head, Key key, const string& searchValue, NhanVien* danhSach) {
    if(head == nullptr) return;
    int page = 1;
    int total = demSoNhanVien(head);
    int totalPages = (total + soDongMotTrang - 1) / soDongMotTrang;
    char keyPress;
    while (true) {
        drawDanhSachNhanVienHighlight(head, page, key, searchValue);
        keyPress = _getch();
        if (keyPress == -32 || keyPress == 0) keyPress = _getch();
        switch (keyPress) {
            case KEY_RIGHT: page = (page % totalPages) + 1; break;
            case KEY_LEFT: page = (page == 1) ? totalPages : page - 1; break;
            case KEY_TAB: controlPrintProfileHighlight(danhSach,key,searchValue); break;
            case KEY_ENTER: 
            case KEY_ESC: return;
        }
    }
}
void drawDanhSachNhanVienHighlightMulti(NhanVien* head, int page, const DieuKienTim& dk) {
    int x = 2, y = 3;
    int total = demSoNhanVien(head);
    int totalPages = (total + soDongMotTrang - 1) / soDongMotTrang;
    if (page < 1) page = 1;
    if (page > totalPages) page = totalPages;

    console::clearScreen();
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "KET QUA TIM KIEM: " << total << " ket qua - Trang " << page << "/" << totalPages;
    console::resetColor();
    drawTableHeader(x, y);

    int start = (page - 1) * soDongMotTrang;
    int stt = start + 1;
    int row = y + 2;

    NhanVien* p = head;
    for (int i = 0; i < start && p; ++i) p = p->next;

    while (p && row < y + 2 + soDongMotTrang) {
        // === Kiểm tra field khớp ===
        bool match_maPB    = !dk.maPB.empty()    && toLowerCase(p->maPhongBan).find(toLowerCase(dk.maPB)) != string::npos;
        bool match_maNV    = !dk.maNV.empty()    && toLowerCase(p->maNhanVien).find(toLowerCase(dk.maNV)) != string::npos;
        bool match_hoTen   = !dk.hoTen.empty()   && toLowerCase(p->hoTen).find(toLowerCase(dk.hoTen)) != string::npos;
        bool match_chucVu  = !dk.chucVu.empty()  && toLowerCase(p->chucVu).find(toLowerCase(dk.chucVu)) != string::npos;
        bool match_ngaySinh = 
            (!dk.ngaySinhTu.empty() && p->ngaySinh >= dk.ngaySinhTu) &&
            (dk.ngaySinhDen.empty() || p->ngaySinh <= dk.ngaySinhDen);
        bool match_luong = 
            (!dk.luongTu.empty() && p->luong >= stod(dk.luongTu)) &&
            (dk.luongDen.empty() || p->luong <= stod(dk.luongDen));

        bool hasAnyMatch = match_maPB || match_maNV || match_hoTen || match_chucVu || match_ngaySinh || match_luong;

        // === Vẽ dòng
        if (hasAnyMatch) console::setColor(10); // nền xanh lá
        console::gotoxy(x - 1, row);
        cout << setw(4) << right << stt++ << " | ";
        console::resetColor();

        // Hàm hỗ trợ in field có tô vàng nếu khớp
        auto printField = [&](const string& value, bool matched, int width) {
            if (hasAnyMatch) console::setColor(10); // giữ nền xanh lá
            if (matched) console::setColor(14);     // đè màu vàng nếu khớp
            cout << setw(width) << left << value.substr(0, width);
            console::resetColor(); cout << " | ";
        };

        // === Các field cụ thể ===
        printField(p->tenPhongBan, match_maPB, 19);
        printField(p->maNhanVien, match_maNV, 10);
        printField(p->hoTen, match_hoTen, 25);
        printField(p->chucVu, match_chucVu, 14);

        if (hasAnyMatch) console::setColor(10);
        if (match_ngaySinh) console::setColor(14);
        cout << setw(10) << p->ngaySinh; console::resetColor(); cout << " | ";

        if (hasAnyMatch) console::setColor(10);
        if (match_luong) console::setColor(14);
        cout << setw(13) << fixed << setprecision(0) << p->luong;
        console::resetColor();

        ++row;
        p = p->next;
    }

    // hint
    console::gotoxy(x + 15, MENU_Y + 1 + 12);
    console::setColor(11);
    cout << "<= Trang truoc";
    console::gotoxy(x + 35, MENU_Y + 1 + 12);
    cout << "Trang sau =>";
    console::gotoxy(x + 55, MENU_Y + 1 + 12);
    cout << "Nhan TAB de hien toan bo";
    console::gotoxy(x + 90, MENU_Y + 1 + 12);
    console::setColor(12);
    cout << "Thoat(ESC-ENTER)";
    console::resetColor();
}



void controlPrintProfileHighlightMulti(NhanVien* head, const DieuKienTim& dk, NhanVien* danhSach) {
    if(head == nullptr) return;
    int page = 1;
    int total = demSoNhanVien(head);
    int totalPages = (total + soDongMotTrang - 1) / soDongMotTrang;
    char key;

    while (true) {
        drawDanhSachNhanVienHighlightMulti(head, page, dk);

        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_RIGHT: page = (page % totalPages) + 1; break;
            case KEY_LEFT:  page = (page == 1) ? totalPages : page - 1; break;
            case KEY_TAB:   // => Hiển thị toàn bộ danh sách
                controlPrintProfileHighlightMulti(danhSach,dk);
                break;
            case KEY_ENTER:
            case KEY_ESC:
                return;
        }
    }
}
