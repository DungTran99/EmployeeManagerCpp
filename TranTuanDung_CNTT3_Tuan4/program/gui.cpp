#include "gui.h"
#include "getProfile.cpp"
#include "showProfile.cpp"
#include "algorithm.cpp"
#include "search.cpp"


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

void console::clearLine(int x, int y, int width) {
    console::gotoxy(x, y);
    for (int i = 0; i < width; ++i) cout << " ";
}
//co dinh kich thuoc terminal
void resizeConsole_(int x, int y){
    _COORD coord;
    coord.X =x;
    coord.Y =y;
    _SMALL_RECT rect;
    rect.Top = 0;
    rect.Left = 0 ;
    rect.Bottom = y - 1;
    rect.Right = x - 1;
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE) ;
    SetConsoleScreenBufferSize(handle,coord);
    SetConsoleWindowInfo(handle, false, &rect);
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
//khung hien key da chon
void drawFrameKey(){
    int x = 73, y = 15, width = 45;
    console::gotoxy(x+1, y-1);
    console::setColor(4);
    cout<<"Key da chon:";
    console::resetColor();
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
//huong dan o duoi
void showSuggest(){
    console::gotoxy(MENU_X+2, MENU_Y+1 + 12);
    cout<<"Nhan ENTER hoac RIGHT ARROW de chon!";
    console::gotoxy(MENU_X+2, MENU_Y+2 + 12);
    cout<<"Nhan ESC hoac LEFT ARROW quay lai";
    drawBox(MENU_X, MENU_Y + 12, 40, 4, 4);
}

//loading
void loading(const string& message, int delay, int length) {
    resizeConsole_(CONSOLE_SIZE_X,CONSOLE_SIZE_Y);
    console::clearScreen();
    console::cursorStatus(25, FALSE);
    cout<<LOGO;
    console::gotoxy(MENU_X+50,MENU_Y);
    cout << message;
    for (int i = 0; i < 3; ++i) { 
        cout << ".";
        Sleep(300);
    }
    console::gotoxy(MENU_X+38,MENU_Y+2);
    cout << "[";
    for (int i = 0; i <= length; ++i) {
        cout << (char)177; // layer duoi
    }
    for (int i = 0; i <= length; ++i) {
        int percent = i * 100 / length;
        console::gotoxy(MENU_X+35,MENU_Y+2);
        cout << percent << "%"<< "  ";
        console::gotoxy(MENU_X+38 + i,MENU_Y+2);
        console::setColor(10);
        cout << (char)219; // layer tren
        Sleep(delay);
    }
    cout << " Done!";
    console::gotoxy(MENU_X+41,MENU_Y+4);
    console::resetColor();
    cout<<"!Press any key to continue...";
    if(_getch()){
        menuGUI();
    }
}
// ================= MENU SELECTION =================
int firstSelect(MenuContext& firstMenu, int width, int height ) {
    char key;
    while (true) {
        console::clearScreen();
        printLogo();
        drawingMenu(firstMenu.options, firstMenu.selected, firstMenu.x, firstMenu.y);
        drawBox(firstMenu.x, firstMenu.y-1 ,width + 24, height + firstMenu.options.size() + 2);
        // Gợi ý
        drawBox(MENU_X, MENU_Y + 12, 40, 4, 4);
        showSuggest();

        // Thao tac phim
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
            case KEY_ESC:
                return -1;
        }
    }
}

int secondSelect(const MenuContext& firstMenu,  MenuContext& secondMenu, int  width, int height) {
    char key;
    while (true) {
        console::clearScreen();
        printLogo();
        drawingMenu(firstMenu.options, firstMenu.selected, firstMenu.x, firstMenu.y);
        drawBox(firstMenu.x, firstMenu.y-1 , 24, firstMenu.options.size() + 2);// ve box
        drawingMenu(secondMenu.options, secondMenu.selected, secondMenu.x, secondMenu.y);
        drawBox(secondMenu.x, secondMenu.y-1 ,width + 24  + 3,height + secondMenu.options.size() + 2);
        // Gợi ý
        drawBox(MENU_X, MENU_Y + 12, 40, 4, 4);
        showSuggest();


        // Thao tac phim
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
            case KEY_ESC:
                return -1;
        }
    }
}

int thirdSelect(MenuContext& firstMenu, MenuContext& secondMenu, MenuContext& thirdMenu, vector<Key> keyMenu, int  width, int height) {
    char key;
    while (true) {
        console::clearScreen();
        printLogo();
        drawingMenu(firstMenu.options, firstMenu.selected, firstMenu.x, firstMenu.y);
        drawBox(firstMenu.x, firstMenu.y-1 ,24, firstMenu.options.size() + 2);// ve box
        drawingMenu(secondMenu.options, secondMenu.selected, secondMenu.x, secondMenu.y);
        drawBox(secondMenu.x, secondMenu.y-1 , 24 + 3, secondMenu.options.size() + 2);
        drawingMenu(thirdMenu.options, thirdMenu.selected, thirdMenu.x + 3, thirdMenu.y);
        drawBox(thirdMenu.x + 3, thirdMenu.y-1 ,width +  24 - 3,height + thirdMenu.options.size() + 2);
        // Gợi ý
        drawBox(MENU_X, MENU_Y + 12, 40, 4, 4);
        showSuggest();
        //hiển thị các key đã chọn
        drawFrameKey();
        showKey(keyMenu);
        // Thao tac phim
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
            case KEY_ESC:
                return -1;
        }
    }
}

// ================= MAIN MENU CHOICE =================

void profileMenu(MenuContext& mainMenu, NhanVien *& danhSach) {
    const vector<string> profileMenu = {
        "A. Nhap tu ban phim",
        "B. Nhap tu file",
        "C. Xuat ra file",
        "D. Sua theo ma",
        "E. Xoa theo ma",
        "F. Xoa toan bo"
    };
    MenuContext profile(profileMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y);

    while (true) {
        int choice = secondSelect(mainMenu, profile, -3);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                nhapProfile(danhSach);
                break;
            case 1:
                importByFile(danhSach);
                break;
            case 2:
                outputByFile(danhSach);
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}

void printMenu(MenuContext& mainMenu, NhanVien *& danhSach) {
    const vector<string> printMenu = {
        "In toan bo danh sach"
    };
    MenuContext profile(printMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y +  PRINT_OFFSET_Y);
    
    int choice = secondSelect(mainMenu, profile);
    if (choice == -1) return;
    else{
        controlPrintProfile(danhSach);
        console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
        cout<<"nhan nut quay lai de tiep tuc";
        choice = secondSelect(mainMenu, profile);
        if (choice == -1) return;
    }    
}
//============== KHUNG SAP XEP ============================
void selectionSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y){
    const vector<string> sortKeysMenu = {
        "A. Ma phong ban",
        "B. Ten phong ban",
        "C. Ma nhan vien",
        "D. Ho va ten",
        "E. Chuc vu",
        "F. Ngay sinh",
        "G. Luong",
        "(Xac nhan!)"
    };
    MenuContext sortKey(sortKeysMenu, 0, secondMenu.x + SECOND_MENU_X, secondMenu.y + MENU_Y);
    //khoi tao key 
    vector<Key> keyMenu;
    while (true) {
        int choice = thirdSelect(mainMenu, secondMenu, sortKey, keyMenu);
        if (choice == -1) return;
        
        showKey(keyMenu);

        switch (choice) {
            case 0:
                button(keyMenu, MAPB);
                break;
            case 1:
                button(keyMenu, TENPB);
                break;
            case 2:
                button(keyMenu, MANV);
                break;
            case 3:
                button(keyMenu, HOVATEN);
                break;
            case 4:
                button(keyMenu, CHUCVU);
                break;
            case 5:
                button(keyMenu, NGAYSINH);
                break;
            case 6:
                button(keyMenu, LUONG);
                break;
            case 7:
                accecptButton(keyMenu, danhSach, "SelectionSort");
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien...????"; getch();
                break;
        }
    }
}
void insertionSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y){
    const vector<string> sortKeysMenu = {
        "A. Ma phong ban",
        "B. Ten phong ban",
        "C. Ma nhan vien",
        "D. Ho va ten",
        "E. Chuc vu",
        "F. Ngay sinh",
        "G. Luong",
        "(Xac nhan!)"
    };
    MenuContext sortKey(sortKeysMenu, 0, secondMenu.x + SECOND_MENU_X, secondMenu.y + MENU_Y);
    //khoi tao key 
    vector<Key> keyMenu;
    while (true) {
        
        int choice = thirdSelect(mainMenu, secondMenu, sortKey, keyMenu);
        if (choice == -1) return;
        
        switch (choice) {
            case 0:
                button(keyMenu, MAPB);
                break;
            case 1:
                button(keyMenu, TENPB);
                break;
            case 2:
                button(keyMenu, MANV);
                break;
            case 3:
                button(keyMenu, HOVATEN);
                break;
            case 4:
                button(keyMenu, CHUCVU);
                break;
            case 5:
                button(keyMenu, NGAYSINH);
                break;
            case 6:
                button(keyMenu, LUONG);
                break;
            case 7:
                accecptButton(keyMenu, danhSach, "InsertionSort");
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien...????"; getch();
                break;
        }
    }
}
void mergeSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y){
    const vector<string> sortKeysMenu = {
        "A. Ma phong ban",
        "B. Ten phong ban",
        "C. Ma nhan vien",
        "D. Ho va ten",
        "E. Chuc vu",
        "F. Ngay sinh",
        "G. Luong",
        "(Xac nhan!)"
    };
    MenuContext sortKey(sortKeysMenu, 0, secondMenu.x + SECOND_MENU_X, secondMenu.y + MENU_Y);
    //khoi tao key 
    vector<Key> keyMenu;
    while (true) {
        
        int choice = thirdSelect(mainMenu, secondMenu, sortKey, keyMenu);
        if (choice == -1) return;
        
        switch (choice) {
            case 0:
                button(keyMenu, MAPB);
                break;
            case 1:
                button(keyMenu, TENPB);
                break;
            case 2:
                button(keyMenu, MANV);
                break;
            case 3:
                button(keyMenu, HOVATEN);
                break;
            case 4:
                button(keyMenu, CHUCVU);
                break;
            case 5:
                button(keyMenu, NGAYSINH);
                break;
            case 6:
                button(keyMenu, LUONG);
                break;
            case 7:
                accecptButton(keyMenu, danhSach, "MergeSort");
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien...????"; getch();
                break;
        }
    }
}
void quickSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y){
    const vector<string> sortKeysMenu = {
        "A. Ma phong ban",
        "B. Ten phong ban",
        "C. Ma nhan vien",
        "D. Ho va ten",
        "E. Chuc vu",
        "F. Ngay sinh",
        "G. Luong",
        "(Xac nhan!)"
    };
    MenuContext sortKey(sortKeysMenu, 0, secondMenu.x + SECOND_MENU_X, secondMenu.y + MENU_Y);
    //khoi tao key 
    vector<Key> keyMenu;
    while (true) {
        
        int choice = thirdSelect(mainMenu, secondMenu, sortKey, keyMenu);
        if (choice == -1) return;
        
        switch (choice) {
            case 0:
                button(keyMenu, MAPB);
                break;
            case 1:
                button(keyMenu, TENPB);
                break;
            case 2:
                button(keyMenu, MANV);
                break;
            case 3:
                button(keyMenu, HOVATEN);
                break;
            case 4:
                button(keyMenu, CHUCVU);
                break;
            case 5:
                button(keyMenu, NGAYSINH);
                break;
            case 6:
                button(keyMenu, LUONG);
                break;
            case 7:
                accecptButton(keyMenu, danhSach, "QuickSort");
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien...????"; getch();
                break;
        }
    }
}
//ket hop voi key menu 
void arrangeMenu(MenuContext& mainMenu, NhanVien *& danhSach){
    const vector<string> sortAlgosMenu = {
        "A. SelectionSort",
        "B. InsertionSort",
        "C. QuickSort",
        "D. MergeSort"
    };
    
    MenuContext sortAlgos(sortAlgosMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y + SORT_OFFSET_Y);
    
    while (true) {
        int choice = secondSelect(mainMenu,sortAlgos);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                selectionSortKeyMenu(mainMenu, danhSach, sortAlgos, 0);
                break;
            case 1:
                insertionSortKeyMenu(mainMenu, danhSach, sortAlgos, 1);
                break;
            case 2:
                mergeSortKeyMenu(mainMenu, danhSach, sortAlgos, 2);
                break;
            case 3:
                quickSortKeyMenu(mainMenu, danhSach, sortAlgos, 3);
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}
//====================== KHUNG TIM KIEM ========================
void timKiemChinhXacMenu(MenuContext& mainMenu, NhanVien *&danhSach, MenuContext& secondMenu, int MENU_Y){
    const vector<string> sortKeysMenu = {
        "A. Ma phong ban",
        "B. Ma nhan vien",
        "C. Ho va ten",
        "D. Ngay sinh",
        "E. Luong",
        "F. Chuc vu"
    };
    MenuContext sortKey(sortKeysMenu, 0, secondMenu.x + SECOND_MENU_X, secondMenu.y + MENU_Y);
    //khoi tao menu key
    vector<Key> keyMenu;
    while (true) {
        int choice = thirdSelect(mainMenu, secondMenu, sortKey, keyMenu);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                chinhXac(danhSach, MAPB);
                break;
            case 1:
                chinhXac(danhSach, MANV);
                break;
            case 2:
                chinhXac(danhSach, HOVATEN);
                break;
            case 3:
                chinhXac(danhSach, NGAYSINH);
                break;
            case 4:
                chinhXac(danhSach, LUONG);
                break;
            case 5:
                chinhXac(danhSach, CHUCVU);
                break;
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien...????"; getch();
                break;
        }
    }
}

// search 
void searchMenu(MenuContext& mainMenu, NhanVien *& danhSach) {
    const vector<string> searchAlgosMenu = {
        "A. Chinh xac",
        "B. Theo nhieu tieu chi",
        "C. Ngay lap tuc"
    };

    MenuContext searchAlgos(searchAlgosMenu, 0, mainMenu.x + SECOND_MENU_X, mainMenu.y + SEARCH_OFFSET_Y);

    while (true) {
        int choice = secondSelect(mainMenu,searchAlgos);
        if (choice == -1) return;

        switch (choice) {
            case 0:
                timKiemChinhXacMenu(mainMenu, danhSach, searchAlgos, 0);
                break;  
            case 1:
                timKiemTheoTieuChiMenu(danhSach);
                break;
            case 2:
                timKiemNhanhToanCuc(danhSach);
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}
//thong ke
void statisticMenu(MenuContext& mainMenu, NhanVien *& danhSach) {
    const vector<string> stats = {
        "A. So luong nhan vien theo phong ban",
        "B. Ty le phan loai theo muc luong"
    };

    MenuContext statsticMenu(stats, 0,mainMenu.x + SECOND_MENU_X, mainMenu.y + STATISTIC_OFFSET_Y);
    while (true) {
        int choice = secondSelect(mainMenu, statsticMenu, 16, 0) ;// width va height duoc cong them
        if (choice == -1) return;

        switch (choice) {
            case 0:
                //gan tam
                timKiemChinhXacMenu(mainMenu, danhSach, statsticMenu, 0);
                break;  
            default:
                console::gotoxy(MENU_X, MENU_Y + STATISTIC_OFFSET_Y + 22);
                cout << "Chuc nang dang phat trien..."; getch();
                break;
        }
    }
}
//menu chinh
void menuGUI() {
    //khởi tạo đối tượng
    NhanVien* danhSach = nullptr;
    //khi chương trình chạy thì nhập sẵn data
    docFile(tenFile, danhSach);
    

    console::cursorStatus(25, FALSE);
    const vector<string> mainMenuList = {
        "A. Them Moi Ho So",
        "B. In Danh Sach",
        "C. Sap Xep",
        "D. Tim Kiem",
        "E. Thong Ke",
        "F. EXIT"
    };

    MenuContext mainMenu(mainMenuList, 0, MENU_X, MENU_Y);

    while (true) {
        int choice = firstSelect(mainMenu);
        if (choice == -1) continue;

        switch (choice) {
        case 0:
            profileMenu(mainMenu, danhSach);
            break;
        case 1:
            printMenu(mainMenu, danhSach);
            break;
        case 2:
            arrangeMenu(mainMenu, danhSach);
            break;
        case 3:
            searchMenu(mainMenu, danhSach);
            break;
        case 4:
            statisticMenu(mainMenu, danhSach);
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

