#include "cuoiky.h"
#include "gui.h"
#include "algorithm.h"
#include "showProfile.h"
#include "getProfile.h"
#include "statistic.h"
using namespace std;

//mergesort

void menuSapXep(NhanVien *head){
    vector<Key> keyMenu;
    keyMenu.push_back(LUONG);
    mergeSortNew(head, keyMenu);
    controlPrintProfile(head);
}

int compareLuong(nv* a, nv* b, const vector<Key>& keys) {
    for (Key k : keys) {
        if (k == LUONG) {
            if (a->luong != b->luong)
                return a->luong > b->luong ? -1 : 1;
        }
    }
    return 0; // bằng nhau
}
void splitListNew(nv* head, nv** frontRef, nv** backRef) {
    nv* slow = head;
    nv* fast = head->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = head;
    *backRef = slow->next;
    if (*backRef) (*backRef)->prev = nullptr;
    slow->next = nullptr;
}
nv* sortedMergeNew(nv* a, nv* b, const vector<Key>& keys) {
    if (!a) return b;
    if (!b) return a;

    nv* result = nullptr;

    if (compareLuong(a, b, keys) <= 0) {
        result = a;
        result->next = sortedMergeNew(a->next, b, keys);
        if (result->next) result->next->prev = result;
        result->prev = nullptr;
    } else {
        result = b;
        result->next = sortedMergeNew(a, b->next, keys);
        if (result->next) result->next->prev = result;
        result->prev = nullptr;
    }

    return result;
}

void mergeSortNew(nv*& headRef, const vector<Key>& keys) {
    if (!headRef || !headRef->next) return;

    nv* a = nullptr;
    nv* b = nullptr;

    splitListNew(headRef, &a, &b);

    mergeSortNew(a, keys);
    mergeSortNew(b, keys);

    headRef = sortedMergeNew(a, b, keys);
}

//tim khoang tuoi

void menuKhoangTuoi(NhanVien *danhSach) {
    console::clearScreen();
    if (!danhSach) {
        console::gotoxy(2, 6);
        console::setColor(12);
        cout << "Danh sach nhan vien rong.";
        console::resetColor();
        _getch();
        return;
    }
    console::clearScreen();

    int tuoiTu, tuoiDen;

    // Nhập khoảng tuổi
    
    console::gotoxy(2, 1);
    console::setColor(4);
    cout << "Nhap khoang tuoi:";
    console::resetColor();

    console::gotoxy(5,4);
    cout<<"Tu tuoi:";
    drawBox(5,5,20,3,7);
    console::gotoxy(7,6);
    cin >> tuoiTu;

    console::gotoxy(5,9);
    cout<<"Den tuoi:";
    drawBox(5,10,20,3,7);
    console::gotoxy(7,11);
    cin >> tuoiDen;

    // Kiểm tra khoảng tuổi hợp lệ
    if (tuoiTu > tuoiDen || tuoiTu < 0 || tuoiDen < 0) {
        console::gotoxy(2, 6);
        console::setColor(12);
        cout << "Khoang tuoi khong hop le. Vui long thu lai.";
        console::resetColor();
        _getch();
        return;
    }

    // Lọc danh sách nhân viên
    time_t t = time(0);
    tm* now = localtime(&t);
    int namHienTai = now->tm_year + 1900;

    NhanVien* ketQua = nullptr;
    NhanVien* tail = nullptr;

    for (NhanVien* p = danhSach; p != nullptr; p = p->next) {
        int ngay, thang, nam;
        parseNgaySinh(p->ngaySinh, ngay, thang, nam);
        int tuoi = namHienTai - nam;
        if (tuoi >= tuoiTu && tuoi <= tuoiDen) {
            NhanVien* node = new NhanVien(*p);
            node->next = nullptr;
            node->prev = tail;
            if (!ketQua) ketQua = node;
            else tail->next = node;
            tail = node;
        }
    }

    // Hiển thị kết quả
    if (!ketQua) {
        console::clearScreen();
        console::gotoxy(2, 6);
        console::setColor(12);
        cout << "Khong tim thay nhan vien nao trong khoang tuoi.";
        console::resetColor();
        _getch();
        return;
    }

    controlPrintProfile(ketQua);

    // Giải phóng bộ nhớ danh sách tạm
    while (ketQua) {
        NhanVien* temp = ketQua;
        ketQua = ketQua->next;
        delete temp;
    }
}

//bieu do thong ke

void drawBarChartNew(int x, int y, const string labels[], const int values[], int n, int total) {
    console::clearScreen();
    console::gotoxy(x, y);
    console::setColor(4);
    cout << "BIEU DO THONG KE PHAN BO NHAN VIEN";
    console::resetColor();

    // Ve truc y
    for (int i = 0; i <= 10; i++) {
        console::gotoxy(x + 5, y + 2 + i);
        double percent = 100 - i * 10; 
        cout << setw(4) << right << percent << "% -";
    }

    // ve truc x
    int yAxis = y + 12;
    console::gotoxy(x + 10, yAxis);
    cout << string(60, '-'); 

    int colSpace = 15;
    
    //ve cot
    for (int i = 0; i < n; i++) {
        int xPos = x + 15 + (i * colSpace);
        double percent = (double)values[i] / total * 100;
        int height = ceil(percent / 10); 
        
        // ve cot 
        for (int j = 0; j < height && j < 10; j++) {
            console::gotoxy(xPos, yAxis - j);
            console::setColor(10);
            if(j == 0){
                cout << (char)223 << (char)223;
            }else{
                cout << (char)219 << (char)219;
            }
        }
        
        // In ký tự đại diện
        console::gotoxy(xPos, yAxis + 1);
        console::resetColor();
        cout << (char)('A' + i);
        
        // In phần trăm với độ chính xác cao hơn cho giá trị nhỏ
        console::gotoxy(xPos - 1, yAxis + 2);
        if (percent < 1) {
            cout << fixed << setprecision(1) << percent << "%";
        } else {
            cout << fixed << setprecision(0) << percent << "%";
        }
    }
    
    console::resetColor();
    
    // Hiển thị chú thích
    int legendX = x;
    int legendY = y + 20;
    console::gotoxy(legendX, legendY);
    cout << "CHU THICH:";

    for (int i = 0; i < n; i++) {
        if(i % 6 == 0) {
            legendX = x + 8*i;
            legendY = y + 22; 
        }
        console::gotoxy(legendX, legendY++); 
        cout << (char)('A' + i) << ": " << labels[i]<<" (" <<values[i]<<")";
    }
    
    // Hiển thị biểu tượng cho biểu đồ
    console::gotoxy(x, yAxis + 7);
    console::setColor(10);
    cout << (char)223 << (char)223;
    console::resetColor();
    cout << " So nhan vien";
}

void drawFrameThongKeTuoi(int x, int y, int tongNV){
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "THONG KE SO LUONG NHAN VIEN THEO PHONG BAN";  
    console::gotoxy(x, 2);
    cout << "DANH SACH NHAN VIEN: "<<tenFile;
    cout << "TONG SO NHAN VIEN: "<<tongNV;
    console::resetColor();
    console::gotoxy(x, y+1);
    cout << "STT | Nam sinh                   | So luong     ";
    console::gotoxy(x, y + 2);
    cout << "----|----------------------------|--------------";
}

void thongKeTheoTuoi(NhanVien *head, bool showChart){
    console::clearScreen();

    if(!head){
        cout << "Danh sach rong";
        return;
    }

    int x = 2, y = 3;
    int row = y + 3;
    // Đếm tổng số nhân viên
    int total = demSoNhanVien(head);
    // Vẽ khung đầu dòng
    drawFrameThongKeTuoi(x, y, total);

    // Khởi tạo các nhóm tuổi
    int countBefore1990 = 0;
    int count1990To2000 = 0;
    int countAfter2000 = 0;

    time_t t = time(0);
    tm* now = localtime(&t);
    int currentYear = now->tm_year + 1900;

    // Phân loại nhân viên theo nhóm tuổi
    for (NhanVien* p = head; p != nullptr; p = p->next) {
        int ngay, thang, nam;
        parseNgaySinh(p->ngaySinh, ngay, thang, nam);

        if (nam < 1990) {
            countBefore1990++;
        } else if (nam >= 1990 && nam <= 2000) {
            count1990To2000++;
        } else {
            countAfter2000++;
        }
    }

    // Hiển thị kết quả thống kê
    string ageGroups[] = {"Truoc 1990", "1990 - 2000", "Sau 2000"};
    int counts[] = {countBefore1990, count1990To2000, countAfter2000};

    for (int i = 0; i < 3; i++) {
        console::gotoxy(x - 1, row);
        int stt = i + 1;
        cout << setw(4) << right << stt
             << " | " << setw(26) << left << ageGroups[i]
             << " | " << setw(12) << left << counts[i];
        row++;
    }

    // Vẽ biểu đồ nếu được yêu cầu
    if (showChart) {
        drawBarChartNew(x, y - 2, ageGroups, counts, 3, total);
    }

    // Hiển thị gợi ý
    console::gotoxy(x + 70, 2);
    cout << "Nhan TAB de " << (showChart ? "tat" : "bat") << " bieu do";
    cout << " | ESC/ENTER de thoat";
}

void menuThongKe(NhanVien* head){
    bool showChart = false;
    char key;

    while (true) {
        console::clearScreen();
        thongKeTheoTuoi(head, showChart);

        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_TAB:
                showChart = !showChart;
                break;
            case KEY_ESC:
            case KEY_ENTER:
                return;
        }
    }
}