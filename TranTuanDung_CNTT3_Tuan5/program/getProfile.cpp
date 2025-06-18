#include "getProfile.h"
#include "gui.h"
#include "showProfile.h"

using namespace std;

bool kiemTraMaNhanVien(const string& ma) {
    if (ma.length() < 8) return false;
    if (ma.length() > 10) return false;
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

//nhap ten file
bool setNameFile(){
    int x = MENU_X + 50, y = MENU_Y, width = 60;
    textBoxInput(x,y,width);
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
        console::clearLine(x, y + i, width);
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
        showFileErrorPut();
    }
}

void outputByFile(NhanVien*& head){
    if(!setNameFile()){
        return;
    }
    if(ghiFile(tenFile, head)){
        showAcceptPut();
    }else{
        showFileErrorPut();
    }
}

void showFileErrorPut(){
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
    _getch();
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
    _getch();
}
// ==================== xay dung khung nhap ===================

//khung nhap file
void textBoxInput(int x, int y, int width,int color){
    console::setColor(color);
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
//khung
void setProfileFrame(){
    console::clearScreen();
    int x = 0, y = 0;
    //int width = 60;
    
    // Tiêu đề
    console::gotoxy(x+2, y+1);
    console::setColor(4);
    cout << "THEM NHAN VIEN MOI";
    console::resetColor();

    // Thông tin file
    console::gotoxy(x + 20, y+1);
    cout << " - FILE: "<<tenFile;   
    // Các nút điều hướng (tương tự như trong drawDanhSachNhanVien)
    console::gotoxy(x + 30, MENU_Y+1 + 12);
    console::setColor(11);
    cout << "Luu(Enter)";
    console::gotoxy(x + 15, MENU_Y+1 + 12);
    console::setColor(12);
    cout << "Thoat(ESC)";
    console::resetColor();
}


void nhapProfile(NhanVien*& head){
    const vector<string> KeysMenu = {
        "Ma phong ban:",
        "Ten phong ban:",
        "Ma nhan vien (8 chu so):",
        "Ho va ten:",
        "Chuc vu:",
        "Ngay sinh (dd/mm/yy):",
        "Luong:",
        
    };
    //bool isErr;
    vector<string> input(KeysMenu.size());
    int width = 60;
    int xinchu = 8, yinchu = 5;
    int x = 0;
    int selected = 0;
    char key;
    while (true) {
        console::cursorStatus(25,false);
        console::clearScreen();
        setProfileFrame();
        // Gợi ý
        for(int i = 0; i < KeysMenu.size(); i++){
            if(i == selected){
                //cach nhau y = 3
                textBoxInput( xinchu - 2, yinchu - 1 + i*3, width, 4);
                console::setColor(4);
                console::gotoxy(xinchu - 1, yinchu + i*3);
                cout<<KeysMenu[i];
                console::setColor(7);
            }else{
                //cach nhau y = 3
                textBoxInput( xinchu - 2, yinchu - 1 + i*3, width);
                console::gotoxy( xinchu, yinchu + i*3);
                cout<<KeysMenu[i];
            }
            //hien thi lai chuoi da nhap
            //xoa truoc khi hien thi
            //console::clearLine(xinchu + KeysMenu[i].length(), yinchu + i*3, width);
            console::gotoxy(xinchu + KeysMenu[i].length() + 1 , yinchu + i*3);
            cout<<input[i];
        }
        //vi tri nhap voi x la do dai chuoi dang nhap va y la vi tri dang nhap * 3
        console::gotoxy(KeysMenu[selected].length() + xinchu + input[selected].length() + 1, yinchu + selected * 3);
        console::cursorStatus(25,true);
        // Thao tac phim
        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_UP:
                selected = (selected == 0) ? KeysMenu.size() - 1 : selected - 1;
                break;
            case KEY_TAB:
            case KEY_DOWN:
                selected = (selected + 1) % KeysMenu.size();
                break;
            case KEY_BACKSPACE:
                if(!input[selected].empty()){
                    input[selected].pop_back();
                }
                break;
            case KEY_ENTER:
            {
                string errorMessage;
                if (checkError(input, errorMessage, head)) {
                    textBoxInput(x + 55, MENU_Y + 12, 60);
                    console::gotoxy(x + 57, MENU_Y + 13);
                    cout<<errorMessage;
                    _getch();
                }else{
                    if(setProfile(head, input)){
                        return;
                    }
                }
                break;
            }
            case KEY_RIGHT:
            case KEY_LEFT:
                break;
            case KEY_ESC:
                return;
                break;
            default:
                if(isprint(key)){
                    if (input[selected].length() < width - 4 - KeysMenu[selected].length())
                    {
                    input[selected]+= key;
                    }
                }
                break;
        }
    }
    return;
}

bool checkError(const vector<string>& input, string& errorMessage, NhanVien* head) {
    int xinchu = 8, yinchu = 5;
    double luong;
    int count = 0;
    
    // 1. Kiểm tra rỗng
    for (int i = 0; i < int(input.size()); i++) {
        if (input[i].empty()) {
            textBoxInput(xinchu + 60, yinchu - 1 + i * 3, 32);
            console::gotoxy(xinchu + 60 + 1, yinchu + i * 3);
            cout << "  Khong duoc de trong";
        }
    }
    if(count > 0){
        errorMessage = "Truong so trong: " + to_string(count) ;
        return true;
    }

    // 2. Kiểm tra tên
    if (!isNameClean(input[3], errorMessage)) {
        textBoxInput(xinchu + 60, yinchu - 1 + 3 * 3, 32);
        console::gotoxy(xinchu + 60 + 1, yinchu + 3 * 3);
        cout << errorMessage;
        _getch();
        return true;
    }

    // 3. Kiểm tra ngày sinh
    if (!kiemTraNgaySinh(input[5])) {
        errorMessage = "Ngay sinh khong hop le hoac chua du 18 tuoi.";
        //showErrorPut();
        return true;
    }

    // 4. Kiểm tra lương
    if (!isNumberhuh(input[6], luong, errorMessage) || !kiemTraLuong(luong)) {
        errorMessage = "Luong khong hop le (>= 0)";
        textBoxInput(xinchu + 60, yinchu - 1 + 6 * 3, 32);
        console::gotoxy(xinchu + 60 + 1, yinchu + 6 * 3);
        cout << errorMessage;
        _getch();
        return true;
    }

    // 5. Kiểm tra mã nhân viên (8 chữ số)
    if (!kiemTraMaNhanVien(input[2])) {
        errorMessage = "Ma nhan vien phai gom 8 chu so.";
        //showErrorPut();
        return true;
    }

    // 6. Kiểm tra trùng mã nhân viên
    for (NhanVien* p = head; p != nullptr; p = p->next) {
        if (p->maNhanVien == input[2]) {
            errorMessage = "Ma nhan vien da ton tai.";
            //showErrorPut();
            return true;
        }
    }

    return false; // Không có lỗi
}


bool setProfile(NhanVien*& head, const vector<string>& input) {
    double luong = stod(input[6]); // Sửa lỗi chưa khai báo

    // Chuẩn hóa họ tên
    string hoTen = chuanHoaHoTen(input[3]);

    // Tạo đối tượng mới
    NhanVien* nv = new NhanVien();
    nv->maPhongBan = input[0];
    nv->tenPhongBan = input[1];
    nv->maNhanVien = input[2];
    nv->hoTen = hoTen;
    nv->chucVu = input[4];
    nv->ngaySinh = input[5];
    nv->luong = luong;
    nv->next = nullptr;
    nv->prev = nullptr;

    // Thêm vào danh sách
    if (!head) head = nv;
    else {
        NhanVien* tail = head;
        while (tail->next) tail = tail->next;
        tail->next = nv;
        nv->prev = tail;
    }

    // Thông báo thành công
    int x = MENU_X + 40, y = MENU_Y + 3;
    for (int i = 0; i < 4; ++i) {
        console::clearLine(x, y + i, 45);
    }
    drawBox(x, y, 45, 4, 10);
    console::gotoxy(x + 2, y + 1);
    console::setColor(10);
    cout << "Them nhan vien thanh cong!";
    console::resetColor();
    console::gotoxy(x + 2, y + 2);
    console::setColor(11);
    cout << "Nhan phim bat ky de tiep tuc...";
    console::resetColor();
    _getch();
    return true;
}


bool isNameClean(const string &ten, string &errorMessage)
{
	for(char c:ten)
	{
		if(!isalpha(c) && c != ' ')
		{
			errorMessage = "Ho va ten chi duoc su dung chu cai va khoang trang";
			return false;
		}
	}
	return true;
}

bool isNumberhuh(const string& s, double& so, string& errorMessage) {
    if (!isDouble(s, so)) {
        errorMessage = "Luong khong hop le (phai la so dương, có thể có thập phân)";
        return false;
    }
    if (so < 0) {
        errorMessage = "Luong phai >= 0";
        return false;
    }
    return true;
}
bool isDouble(const string& s, double& out) {
    if (s.empty()) return false;
    int dotCount = 0;
    for (char c : s) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        } else if (!isdigit(c)) {
            return false;
        }
    }
    stringstream ss(s);
    ss >> out;
    return !(ss.fail() || !ss.eof());
}


bool isInteger(const string& s, int& out) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    stringstream ss(s);
    ss >> out;
    return !(ss.fail() || !ss.eof());
}
