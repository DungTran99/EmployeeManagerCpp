#include "gui.h"
#include "getprofile.cpp"

using namespace std;

// ================= LOGO =================
void printLogo() {
    console::setColor(4);
    console::gotoxy(0, 0);
    cout << LOGO << endl;
    console::resetColor();
}
void console::gotoxy(short x, short y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void console::setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void console::cursorStatus(unsigned size, WINBOOL stt) {
    CONSOLE_CURSOR_INFO crs_inf = {size, stt};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &crs_inf);
}

void console::resetColor() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void console::clearScreen() {
    system("cls");
}


// ================= DRAWING =================
void drawingMenu(const vector<string>& options, int selected, int x, int y) {
    for (int i = 0; i < int(options.size()); ++i) {
        console::gotoxy(x, y + i);
        if (i == selected) {
            console::setColor(172);
            cout << "-> " << options[i] << endl;
        } else {
            console::setColor(15);
            cout << "   " << options[i] << endl;
        }
    }
    console::setColor(15);
}

void drawBox(int x, int y, int width, int height, int color = 15) {
    console::setColor(color);

    // Góc trên
    console::gotoxy(x, y); cout << char(201); // ╔
    for (int i = 1; i < width - 1; ++i) cout << char(205); // ═
    cout << char(187); // ╗

    // Thân
    for (int i = 1; i < height - 1; ++i) {
        console::gotoxy(x, y + i); cout << char(186); // ║
        console::gotoxy(x + width - 1, y + i); cout << char(186); // ║
    }

    // Đáy
    console::gotoxy(x, y + height - 1); cout << char(200); // ╚
    for (int i = 1; i < width - 1; ++i) cout << char(205); // ═
    cout << char(188); // ╝

    console::resetColor();
}



// ================= MENU SELECTION =================
int firstSelect(MenuContext& firstMenu) {
    char key;
    while (true) {
        console::clearScreen();
        printLogo();
        drawingMenu(firstMenu.options, firstMenu.selected, firstMenu.x, firstMenu.y);
        drawBox(firstMenu.x, firstMenu.y-1 , 24, firstMenu.options.size() + 2);

        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_UP:
                firstMenu.selected = (firstMenu.selected == 0) ? firstMenu.options.size() - 1 : firstMenu.selected - 1;
                break;
            case KEY_DOWN:
                firstMenu.selected = (firstMenu.selected + 1) % firstMenu.options.size();
                break;
            case KEY_ENTER:
            case KEY_RIGHT:
                return firstMenu.selected;
            case KEY_LEFT:
                return -1;
        }
    }
}

int secondSelect(const MenuContext& firstMenu,  MenuContext& secondMenu) {
    char key;
    while (true) {
        console::clearScreen();
        printLogo();
        drawingMenu(firstMenu.options, firstMenu.selected, firstMenu.x, firstMenu.y);
        drawBox(firstMenu.x, firstMenu.y-1 , 24, firstMenu.options.size() + 2);// ve box
        drawingMenu(secondMenu.options, secondMenu.selected, secondMenu.x, secondMenu.y);
        drawBox(secondMenu.x, secondMenu.y-1 , 24, secondMenu.options.size() + 2);

        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_UP:
                secondMenu.selected = (secondMenu.selected == 0) ? secondMenu.options.size() - 1 : secondMenu.selected - 1;
                break;
            case KEY_DOWN:
                secondMenu.selected = (secondMenu.selected + 1) % secondMenu.options.size();
                break;
            case KEY_ENTER:
            case KEY_RIGHT:
                return secondMenu.selected;
            case KEY_LEFT:
                return -1;
        }
    }
}

int thirdSelect(MenuContext& firstMenu, MenuContext& secondMenu, MenuContext& thirdMenu) {
    char key;
    while (true) {
        console::clearScreen();
        printLogo();
        drawingMenu(firstMenu.options, firstMenu.selected, firstMenu.x, firstMenu.y);
        drawBox(firstMenu.x, firstMenu.y-1 , 24, firstMenu.options.size() + 2);// ve box
        drawingMenu(secondMenu.options, secondMenu.selected, secondMenu.x, secondMenu.y);
        drawBox(secondMenu.x, secondMenu.y-1 , 24, secondMenu.options.size() + 2);
        drawingMenu(thirdMenu.options, thirdMenu.selected, thirdMenu.x, thirdMenu.y);
        drawBox(thirdMenu.x, thirdMenu.y-1 , 24, thirdMenu.options.size() + 2);
        
        key = _getch();
        if (key == -32 || key == 0) key = _getch();

        switch (key) {
            case KEY_UP:
                thirdMenu.selected = (thirdMenu.selected == 0) ? thirdMenu.options.size() - 1 : thirdMenu.selected - 1;
                break;
            case KEY_DOWN:
                thirdMenu.selected = (thirdMenu.selected + 1) % thirdMenu.options.size();
                break;
            case KEY_ENTER:
            case KEY_RIGHT:
                return thirdMenu.selected;
            case KEY_LEFT:
                return -1;
        }
    }
}

// ================= MAIN MENU HANDLER =================
void inDanhSach(const NhanVien* head) {
    console::clearScreen();
    if (!head) {
        cout << "Danh sach rong.\n";
        return;
    }

    cout << left << setw(15) << "Ma PB"
         << setw(12) << "Ma NV"
         << setw(25) << "Ho Ten"
         << setw(15) << "Chuc Vu"
         << setw(12) << "Ngay Sinh"
         << setw(10) << "Luong"
         << "\n";
    cout << string(90, '-') << "\n";

    for (const NhanVien* p = head; p; p = p->next) {
        cout << left << setw(15) << p->maPhongBan
             << setw(12) << p->maNhanVien
             << setw(25) << p->hoTen
             << setw(15) << p->chucVu
             << setw(12) << p->ngaySinh
             << fixed << setprecision(2) << setw(10) << p->luong
             << "\n";
    }
}


void printProfile(MenuContext& mainMenu) {
    const vector<string> printMenu = {
        "In toan bo danh sach"
    };
    MenuContext profile(printMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y +  PRINT_OFFSET_Y);
    
    int choice = secondSelect(mainMenu, profile);
    if (choice == -1) return;
    else{
        inDanhSach(danhSach);
        console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
        getch();
        cout<<"nhan nut quay lai de tiep tuc";
        choice = secondSelect(mainMenu, profile);
        if (choice == -1) return;
    }    
}

//============= PROFILE ========================

void nhapHoSo(NhanVien*& head) {
    while (true) {
        NhanVien* nv = new NhanVien();

        cout << "Nhap ma phong ban: ";
        getline(cin, nv->maPhongBan);

        do {
            cout << "Nhap ma nhan vien (8 chu so): ";
            getline(cin, nv->maNhanVien);
            if (!kiemTraMaNhanVien(nv->maNhanVien))
                cout << "Ma nhan vien khong hop le. Vui long nhap lai.\n";
        } while (!kiemTraMaNhanVien(nv->maNhanVien));

        cout << "Nhap ho ten: ";
        getline(cin, nv->hoTen);
        nv->hoTen = chuanHoaHoTen(nv->hoTen);

        cout << "Nhap chuc vu: ";
        getline(cin, nv->chucVu);

        do {
            cout << "Nhap ngay sinh (dd/mm/yyyy): ";
            getline(cin, nv->ngaySinh);
            if (!kiemTraNgaySinh(nv->ngaySinh))
                cout << "Ngay sinh khong hop le hoac chua du 18 tuoi.\n";
        } while (!kiemTraNgaySinh(nv->ngaySinh));

        do {
            cout << "Nhap luong: ";
            string sLuong;
            getline(cin, sLuong);
            try {
                nv->luong = stod(sLuong);
            } catch (...) {
                nv->luong = -1;
            }
            if (!kiemTraLuong(nv->luong))
                cout << "Luong khong hop le.\n";
        } while (!kiemTraLuong(nv->luong));

        nv->next = nullptr;
        nv->prev = nullptr;

        // Thêm vào danh sách
        if (!head) {
            head = nv;
        } else {
            NhanVien* tail = head;
            while (tail->next) tail = tail->next;
            tail->next = nv;
            nv->prev = tail;
        }

        cout << "Nhap tiep? (y/n): ";
        string tiep;
        getline(cin, tiep);
        if (tiep != "y" && tiep != "Y") break;
    }
}


//===============================================
void newProfile(MenuContext& mainMenu) {
    const vector<string> profileMenu = {
        "1. Nhap tu file",
        "2. Nhap tu ban phim",
        "3. Xoa theo ma",
        "4. Sua theo ma"
    };
    MenuContext profile(profileMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y);

    while (true) {
        int choice = secondSelect(mainMenu, profile);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                
                break;
            case 1:
                console::clearScreen();
                nhapHoSo(danhSach);
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}
void keyProfile(MenuContext& mainMenu, MenuContext& secondMenu, int MENU_Y){
    const vector<string> sortKeysMenu = {
        "1. Ma phong ban",
        "2. Ma nhan vien",
        "3. Ho va ten",
        "4. Ngay sinh",
        "5. Luong"
    };
    MenuContext sortKey(sortKeysMenu, 0, secondMenu.x + SECOND_MENU_X, secondMenu.y + MENU_Y);

    while (true) {
        int choice = thirdSelect(mainMenu, secondMenu, sortKey);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                break;
            case 1:
                // TODO: handle keyboard input
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien...????"; getch();
                break;
        }
    }
}

void sortProfile(MenuContext& mainMenu){
    const vector<string> sortAlgosMenu = {
        "1. SelectionSort",
        "2. InsertionS",
        "3. QuickSort",
        "4. MergeSort"
    };
    
    MenuContext sortAlgos(sortAlgosMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y + SORT_OFFSET_Y);
    
    while (true) {
        int choice = secondSelect(mainMenu,sortAlgos);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                keyProfile(mainMenu, sortAlgos, 0);
                break;
            case 1:
                keyProfile(mainMenu, sortAlgos, 1);// hien thi cac cua so moi ngang voi o^ dang chon
                break;
            case 2:
                keyProfile(mainMenu, sortAlgos, 2);
                break;
            case 3:
                keyProfile(mainMenu, sortAlgos, 3);
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}
// search 
void searchProfile(MenuContext& mainMenu) {
    const vector<string> searchAlgosMenu = {
        "1. Tuan tu",
        "2. Nhi phan"
    };

    MenuContext searchAlgos(searchAlgosMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y + SEARCH_OFFSET_Y);

    while (true) {
        int choice = secondSelect(mainMenu,searchAlgos);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                keyProfile(mainMenu, searchAlgos, 0);
                break;
            case 1:
                keyProfile(mainMenu, searchAlgos, 1);
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}

void statisticProfile(MenuContext& mainMenu) {
    const vector<string> stats = {
        "1. So luong nhan vien theo phong ban",
        "2. Ty le phan loai theo muc luong"
    };

    MenuContext statsticMenu(stats, 0,mainMenu.x + SECOND_MENU_X, mainMenu.y + STATISTIC_OFFSET_Y);
    while (true) {
        int choice = secondSelect(mainMenu, statsticMenu) ;
        if (choice == -1) return;

        // TODO: gọi hàm thống kê tương ứng
        cout << "Thong ke theo tuy chon #" << choice + 1;
        getch();
        break;
    }
}
//menu chinh
void menuGUI() {
    console::cursorStatus(25, FALSE);
    const vector<string> mainMenuList = {
        "1. Them Moi Ho So",
        "2. In Danh Sach",
        "3. Sap Xep",
        "4. Tim Kiem",
        "5. Thong Ke",
        "6. EXIT"
    };

    MenuContext mainMenu(mainMenuList, 0, MENU_X, MENU_Y);

    while (true) {
        int choice = firstSelect(mainMenu);
        if (choice == -1) continue;

        switch (choice) {
        case 0:
            newProfile(mainMenu);
            break;
        case 1:
            printProfile(mainMenu);
            break;
        case 2:
            sortProfile(mainMenu);
            break;
        case 3:
            searchProfile(mainMenu);
            break;
        case 4:
            statisticProfile(mainMenu);
            break;
        case 5:
            exit(0);
        default:
            console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
            cout << "Chuc nang dang phat trien..."; getch();
            break;
        }
    }
}


