#include "showProfile.h"
#include "getProfile.h"
#include "gui.h"

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
    cout << "STT | Phong ban              | Ma NV      | Ho ten                    | Chuc vu     | Ngay sinh  | Luong";
    console::gotoxy(x, y + 1);
    cout << "----|------------------------|------------|---------------------------|-------------|------------|---------------";
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
        cout << setw(4) << stt++ << " | "
             << setw(22) << left << p->tenPhongBan.substr(0, 22) << right << " | "
             << setw(10) << p->maNhanVien << " | "
             << setw(25) << left << p->hoTen.substr(0, 20) << right << " | "
             << setw(11) << left << p->chucVu.substr(0, 11) << right << " | "
             << setw(9) << p->ngaySinh << " | "
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


