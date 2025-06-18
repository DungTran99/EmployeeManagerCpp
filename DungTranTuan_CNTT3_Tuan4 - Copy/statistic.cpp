#include "statistic.h"
#include "gui.h"
#include "showProfile.h"

void drawFrameThongKePhongBan(int x, int y, int tongNV){
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "THONG KE SO LUONG NHAN VIEN THEO PHONG BAN";  
    console::gotoxy(x, 2);
    cout << "DANH SACH NHAN VIEN: "<<tenFile;
    cout << "TONG SO NHAN VIEN: "<<tongNV;
    console::resetColor();
    console::gotoxy(x, y+1);
    cout << "STT | Phong ban                  | So nhan vien | Ty le (Nhan vien / Phong ban)                            ";
    console::gotoxy(x, y + 2);
    cout << "----|----------------------------|--------------|----------------------------------------------------------";
}

void thongKePhongBan(NhanVien *head, bool showChart){
    console::clearScreen();

    if(!head){
        cout<<"Danh sach rong";
    }

    int x = 2, y = 3;
    int row = y + 3;
    // Đếm tổng số
    int total = demSoNhanVien(head);
    //ve khung dau dong
    drawFrameThongKePhongBan( x, y, total);
    NhanVien* p = head;
    
    int MAX = 100; //giả sử có tối đa 100 phòng ban
    int stt = 0;
    string phongBanList[MAX];
    int countList[MAX] = {0};
    int n = 0; // số lượng phòng ban khác nhau
    int i;
    bool found;

    while(p){
        found = false;
        for(i = 0; i < n; i++){
            if(phongBanList[i] == p->tenPhongBan){
                countList[i]++;
                found = true;
                break;
            }
        }

        if(!found){
            phongBanList[n] = p->tenPhongBan;
            countList[n] = 1;
            n++;
        }

        p = p->next;
    }

    int length = 0;

    for(i = 0; i< n; i++){
        
        console::gotoxy(x-1, row);
        stt = i + 1;
        cout << setw(4) << right << stt 
             << " | " << setw(26) << left << phongBanList[i]
             << " | " << setw(12) << left << countList[i]
             << " | ";  
        for (int j = 0; j < 46; j++) {
            console::gotoxy(x-1 + 51 + j, row);
            cout << (char)176; // layer duoi
        }
        length = round((double)countList[i] / total * 46);
        for(int j = 0; j < length; j++ ){
            console::gotoxy(x-1 + 51 + j, row);
            console::setColor(10);
            cout << (char)219;
        }
        console::resetColor();
        console::gotoxy(x - 1 + 51 + 46 + 1, row); // hoặc kiểm tra khoảng trắng
        cout << " " << fixed << setprecision(1) << ((double)countList[i] / total )* 100 << "%";
        row++;
    }

    // Vẽ biểu đồ nếu được yêu cầu
    if (showChart) {
        drawBarChart(x, y-2, phongBanList, countList, n, total);
    }
    
    // Hiển thị gợi ý
    console::gotoxy(x + 70, 2);
    cout << "Nhan TAB de " << (showChart ? "tat" : "bat") << " bieu do";
    cout << " | ESC/ENTER de thoat";
}

void drawBarChart(int x, int y, const string labels[], const int values[], int n, int total) {
    console::clearScreen();
    console::gotoxy(x, y);
    console::setColor(4);
    cout << "BIEU DO THONG KE PHAN BO NHAN VIEN";
    console::resetColor();

    // Vẽ trục Y với các mức 50% max và 5% intervals
    for (int i = 0; i <= 15; i++) {
        console::gotoxy(x + 5, y + 2 + i);
        double percent = 50 - i * 3.333; 
        cout << setw(4) << right << percent << "% -";
    }

    // Vẽ trục X
    int yAxis = y + 12+5;//+5 từ trục y
    console::gotoxy(x + 10, yAxis);
    cout << string(100, '-');

    int colSpace = 5;
    
    // Vẽ các cột với hỗ trợ giá trị nhỏ tốt hơn
    for (int i = 0; i < n; i++) {
        int xPos = x + 14 + (i * colSpace);
        double percent = (double)values[i] / total * 100;
        double height = round(percent / 3.333); // Đảm bảo các giá trị nhỏ vẫn hiện
        
        // Vẽ cột với ■■ (char 223)
        for (int j = 0; j < height; j++) {
            console::gotoxy(xPos, yAxis - j);
            console::setColor(10);
            cout << (char)223 << (char)223;
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
        cout << (char)('A' + i) << ": " << labels[i];
    }
    
    // Hiển thị biểu tượng cho biểu đồ
    console::gotoxy(x, yAxis + 4);
    console::setColor(10);
    cout << (char)223 << (char)223;
    console::resetColor();
    cout << " So nhan vien";
}

void controlThongKePhongBan(NhanVien* head){
    bool showChart = false;
    char key;
    
    while (true) {
        console::clearScreen();
        thongKePhongBan(head, showChart);
        
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

//thong ke theo luong
void drawFrameThongKeLuong(int x, int y, int tongNV) {
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "THONG KE PHAN LOAI THEO MUC LUONG";
    console::gotoxy(x, 2);
    cout << "TONG SO NHAN VIEN: " << tongNV;
    console::resetColor();
    
    console::gotoxy(x, y+1);
    cout << "STT | Muc luong                  | So luong | Ty le   | Bieu do                                                      ";
    console::gotoxy(x, y+2);
    cout << "----|----------------------------|----------|---------|--------------------------------------------------------------";
}
void drawPieChart(int x, int y, const vector<pair<string, int>>& data, int total) {
    const string pieChars[] = {
    "      ****      ",
    "    ********    ",
    "   **********   ",
    "  ************  ",
    " ************** ",
    " ************** ",
    " ************** ",
    " ************** ",
    " ************** ",
    "  ************  ",
    "   **********   ",
    "    ********    ",
    "      ****      "
};

    int rows = 13;
    int cols = pieChars[0].length();

    console::clearScreen();
    console::gotoxy(x, y);
    console::setColor(4);
    cout << "BIEU DO TY LE PHAN PHOI LUONG";
    console::resetColor();

    // Vẽ từng dòng của hình tròn
    for (int i = 0; i < rows; i++) {
        string line = pieChars[i];
        int countStars = count(line.begin(), line.end(), '*');
        int drawn = 0;
        int printed = 0;

        console::gotoxy(x, y + 2 + i);

        for (size_t j = 0; j < data.size(); j++) {
            int num = round((double)data[j].second / total * countStars);
            if (j == data.size() - 1)
                num = countStars - drawn; // đảm bảo đủ số ký tự

            int drawnForThis = 0;
            for (size_t k = 0; k < line.length() && drawnForThis < num; k++) {
                if (line[k] == '*') {
                    console::setColor(j + 9);
                    cout << (char)219;
                    drawn++;
                    drawnForThis++;
                } else {
                    if (drawn == 0) cout << line[k]; // In khoảng trắng đầu dòng một lần
                }
            }
        }

        console::resetColor();
    }

    // Chú thích
    for (size_t i = 0; i < data.size(); i++) {
        console::gotoxy(x, y + 20 + i);
        console::setColor(i + 9);
        cout << "\xDB\xDB ";
        console::resetColor();
        cout << data[i].first << ": "
             << fixed << setprecision(1)
             << ((double)data[i].second / total * 100) << "%";
    }

    console::gotoxy(x, y + 11 + data.size());
}

void thongKeMucLuong(NhanVien* head, bool showChart) {
    console::clearScreen();
    if(!head) {
        cout << "Danh sach rong";
        return;
    }

    int x = 2, y = 3;
    int row = y + 3;
    int total = demSoNhanVien(head);
    drawFrameThongKeLuong(x, y, total);

    // Định nghĩa các mức lương
    struct MucLuong {
        double tu;
        double den;
        string ten;
        int soLuong;
    };

    vector<MucLuong> mucLuong = {
        {0, 3000000, "Duoi 3 trieu", 0},
        {3000000, 5000000, "3 - 5 trieu", 0},
        {5000000, 7000000, "5 - 7 trieu", 0},
        {7000000, 10000000, "7 - 10 trieu", 0},
        {10000000, 999999999, "Tren 10 trieu", 0}
    };

    // Thống kê số lượng theo mức lương
    for(NhanVien* p = head; p != nullptr; p = p->next) {
        for(auto& muc : mucLuong) {
            if(p->luong >= muc.tu && p->luong < muc.den) {
                muc.soLuong++;
                break;
            }
        }
    }

    // Hiển thị thống kê
    int stt = 1;
    for(const auto& muc : mucLuong) {
        if(muc.soLuong > 0) { // Chỉ hiện những mức có nhân viên
            row++;
            console::gotoxy(x-1, row);
            cout << setw(4) << right << stt++ 
                 << " | " << setw(26) << left << muc.ten
                 << " | " << setw(8) << right << muc.soLuong
                 << " | " << setw(6) << fixed << setprecision(1) 
                 << ((double)muc.soLuong/total*100) << "% |";

            // Vẽ biểu đồ thanh
            for(int j = 0; j < 52; j++) {
                console::gotoxy(x-1 + 61 + j, row);
                cout << char(176); // Nền biểu đồ
            }

            // Vẽ thanh thể hiện tỷ lệ
            int length = round((double)muc.soLuong/total * 52);
            for(int j = 0; j < length; j++) {
                console::gotoxy(x-1 + 61 + j, row);
                console::setColor(10);
                cout << char(219); // Thanh biểu đồ
            }
            console::resetColor();
        }
    }

    // Hiển thị thông tin thống kê thêm
    row += 2;
    console::gotoxy(x-1, row);
    double tongLuong = 0;
    double luongMax = 0;
    double luongMin = head->luong;
    
    for(NhanVien* p = head; p != nullptr; p = p->next) {
        tongLuong += p->luong;
        luongMax = max(luongMax, p->luong);
        luongMin = min(luongMin, p->luong);
    }

    console::setColor(4);
    cout << "Thong ke them:";
    console::resetColor();
    row++;
    console::gotoxy(x-1, row);
    cout << "- Luong trung binh: " << fixed << setprecision(0) << tongLuong/total;
    row++;
    console::gotoxy(x-1, row);
    cout << "- Luong cao nhat: " << luongMax;
    row++;
    console::gotoxy(x-1, row);
    cout << "- Luong thap nhat: " << luongMin;

    if (showChart) {
        vector<pair<string, int>> chartData;
        for (const auto& muc : mucLuong) {
            if (muc.soLuong > 0) {
                chartData.push_back({muc.ten, muc.soLuong});
            }
        }
        drawPieChart(x, y, chartData, total);
    }
    
    // Hiển thị gợi ý
    console::gotoxy(x + 70, 2);
    cout << "Nhan TAB de " << (showChart ? "tat" : "bat") << " bieu do";
    cout << " | ESC/ENTER de thoat";
}

void controlThongKeMucLuong(NhanVien* head) {
    bool showChart = false;
    char key;
    
    while (true) {
        console::clearScreen();
        thongKeMucLuong(head, showChart);
        
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

void drawFrameThongKeChucVu(int x, int y, int tongNV) {
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "THONG KE SO LUONG NHAN VIEN THEO CHUC VU";
    console::gotoxy(x, 2);
    cout << "DANH SACH NHAN VIEN: " << tenFile;
    cout << "TONG SO NHAN VIEN: " << tongNV;
    console::resetColor();
    console::gotoxy(x, y + 1);
    cout << "STT | Chuc vu                    | So nhan vien | Ty le                                                      ";
    console::gotoxy(x, y + 2);
    cout << "----|----------------------------|--------------|------------------------------------------------------------";
}

void thongKeChucVu(NhanVien* head, bool showChart) {
    console::clearScreen();
    if (!head) {
        cout << "Danh sach rong";
        return;
    }

    int x = 2, y = 3;
    int row = y + 3;
    int total = demSoNhanVien(head);
    drawFrameThongKeChucVu(x, y, total);

    NhanVien* p = head;
    int MAX = 100;
    string chucVuList[MAX];
    int countList[MAX] = {0};
    int n = 0;
    bool found;

    // Thống kê theo chức vụ
    while (p) {
        found = false;
        for (int i = 0; i < n; i++) {
            if (chucVuList[i] == p->chucVu) {
                countList[i]++;
                found = true;
                break;
            }
        }

        if (!found) {
            chucVuList[n] = p->chucVu;
            countList[n] = 1;
            n++;
        }

        p = p->next;
    }

    // Hiển thị thống kê
    for (int i = 0; i < n; i++) {
        console::gotoxy(x-1, row);
        cout << setw(4) << right << (i + 1)
             << " | " << setw(26) << left << chucVuList[i]
             << " | " << setw(12) << left << countList[i]
             << " | ";

        // Vẽ nền biểu đồ
        for (int j = 0; j < 46; j++) {
            console::gotoxy(x-1 + 51 + j, row);
            cout << (char)176;
        }

        // Vẽ thanh biểu đồ
        int length = round((double)countList[i] / total * 46);
        for (int j = 0; j < length; j++) {
            console::gotoxy(x-1 + 51 + j, row);
            console::setColor(10);
            cout << (char)219;
        }
        console::resetColor();

        // In phần trăm
        console::gotoxy(x - 1 + 51 + 46 + 1, row);
        cout << " " << fixed << setprecision(1) << ((double)countList[i] / total) * 100 << "%";
        row++;
    }

    // Vẽ biểu đồ nếu được yêu cầu
    if (showChart) {
        //su dung lai bieu do chung
        drawBarChart(x, y-2, chucVuList, countList, n, total);
    }

    // Hiển thị gợi ý
    console::gotoxy(x + 70, 2);
    cout << "Nhan TAB de " << (showChart ? "tat" : "bat") << " bieu do";
    cout << " | ESC/ENTER de thoat";
}

void controlThongKeChucVu(NhanVien* head) {
    bool showChart = false;
    char key;
    
    while (true) {
        console::clearScreen();
        thongKeChucVu(head, showChart);
        
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

void drawFrameThongKeLuongTheoPB(int x, int y) {
    console::gotoxy(x, 1);
    console::setColor(4);
    cout << "THONG KE PHAN LOAI NHAN VIEN THEO LUONG VA PHONG BAN";
    console::resetColor();
    
    // Header của bảng
    console::gotoxy(x, y);
    cout << "Ma PB | > 10tr     | 5tr - 10tr | 3tr - 5tr  | 1tr - 3tr  | < 1tr      |  Tong";
    console::gotoxy(x, y + 1);
    cout << "      | SL    %    | SL    %    | SL    %    | SL    %    | SL    %    |";
    console::gotoxy(x, y + 2);
    cout << "------|------------|------------|------------|------------|------------|--------";
}

void thongKeLuongTheoPB(NhanVien* head) {
    if (!head) {
        cout << "Danh sach rong";
        return;
    }

    console::clearScreen();
    struct ThongKePB {
        string maPB;
        int luong[5] = {0}; // 5 mức lương
        int total = 0;
    };

    vector<ThongKePB> thongKe;
    int tongChung[5] = {0}; // Tổng cho mỗi mức lương
    int tongTatCa = 0;

    // Thống kê số lượng theo phòng ban và mức lương
    for (NhanVien* p = head; p != nullptr; p = p->next) {
        // Tìm hoặc thêm phòng ban mới
        auto it = find_if(thongKe.begin(), thongKe.end(),
            [p](const ThongKePB& tk) { return tk.maPB == p->maPhongBan; });
        
        if (it == thongKe.end()) {
            ThongKePB newPB;
            newPB.maPB = p->maPhongBan;
            thongKe.push_back(newPB);
            it = thongKe.end() - 1;
        }

        // Phân loại lương
        int mucLuong;
        if (p->luong > 10000000) mucLuong = 0;
        else if (p->luong > 5000000) mucLuong = 1;
        else if (p->luong > 3000000) mucLuong = 2;
        else if (p->luong > 1000000) mucLuong = 3;
        else mucLuong = 4;

        it->luong[mucLuong]++;
        it->total++;
        tongChung[mucLuong]++;
        tongTatCa++;
    }

    // Hiển thị bảng thống kê
    int x = 2, y = 3;
    drawFrameThongKeLuongTheoPB(x, y);

    int row = y + 3;
    // In thống kê cho từng phòng ban
    for (const auto& pb : thongKe) {
        console::gotoxy(x, row);
        cout << setw(6) << left << pb.maPB << "|";
        
        // In thống kê cho từng mức lương
        for (int i = 0; i < 5; i++) {
            double percent = pb.total > 0 ? (pb.luong[i] * 100.0 / pb.total) : 0;
            cout << " " << setw(3) << right << pb.luong[i] 
                 << " " << setw(5) << fixed << setprecision(1) << percent << "% |";
        }
        
        // In tổng của phòng ban
        cout << " " << setw(6) << pb.total;
        row++;
    }

    // In dòng tổng
    console::gotoxy(x, row + 1);
    cout << "------|------------|------------|------------|------------|------------|--------";
    console::gotoxy(x, row + 2);
    cout << "Tong  |";
    
    // In tổng cho từng mức lương
    for (int i = 0; i < 5; i++) {
        double percent = (tongChung[i] * 100.0 / tongTatCa);
        cout << " " << setw(3) << right << tongChung[i] 
             << " " << setw(5) << fixed << setprecision(1) << percent << "% |";
    }
    cout << " " << setw(6) << tongTatCa;

    // Hiển thị gợi ý
    console::gotoxy(x, row + 4);
    cout << "Nhan ESC/ENTER de thoat";
}

void controlThongKeLuongTheoPB(NhanVien* head) {
    char key;
    while (true) {
        console::clearScreen();
        thongKeLuongTheoPB(head);
        
        key = _getch();
        if (key == -32 || key == 0) key = _getch();
        
        if (key == KEY_ESC || key == KEY_ENTER) {
            return;
        }
    }
}