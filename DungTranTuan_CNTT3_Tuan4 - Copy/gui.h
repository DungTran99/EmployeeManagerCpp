#ifndef GUI_H
#define GUI_H
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "getProfile.h"



using namespace std;


// ================= CONSTANTS =================
const string LOGO = R"(

  /$$$$$$  /$$       /$$   /$$ /$$    /$$
 /$$__  $$| $$      | $$$ | $$| $$   | $$
| $$  \ $$| $$      | $$$$| $$| $$   | $$
| $$  | $$| $$      | $$ $$ $$|  $$ / $$/
| $$  | $$| $$      | $$  $$$$ \  $$ $$/ 
| $$/$$ $$| $$      | $$\  $$$  \  $$$/  
|  $$$$$$/| $$$$$$$$| $$ \  $$   \  $/   
 \____ $$$|________/|__/  \__/    \_/    
      \__/                               
                                         
                                         
)";
string tenFile = "Dulieu.dat";//file mặc định
const int CONSOLE_SIZE_X = 300;
const int CONSOLE_SIZE_Y = 100;
const int MENU_X = 0;
const int MENU_Y = 14;
const int SECOND_MENU_X = 24;
const int THIRD_MENU_X = 47 + 3;
const int PRINT_OFFSET_Y = 1;
const int SORT_OFFSET_Y = 2;
const int KEY_OFFSET_Y = 2;
const int SEARCH_OFFSET_Y = 3;
const int STATISTIC_OFFSET_Y = 4;
enum Key { MAPB, TENPB, MANV, HOVATEN, CHUCVU, NGAYSINH, LUONG };

#define KEY_TAB '\t'
#define KEY_BACKSPACE 8
#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER '\r'
#define KEY_ESC 27


// ================ STRUCT ==================
struct MenuContext {
    const vector<string>& options;
    int selected;
    int x, y;

    MenuContext(const vector<string>& opts, int sel, int xpos, int ypos)
    : options(opts), selected(sel), x(xpos), y(ypos) {} // constructor có tham số 
};


// ================= FUNCTION PROTOTYPES =================
void printLogo();
void drawingMenu(const vector<string>& options, int selected, int x, int y);
int firstSelect(MenuContext& firstMenu, int  width = 0, int height = 0);
int secondSelect(const MenuContext& firstMenu, MenuContext& secondMenu, int  width = 0, int height = 0);
int thirdSelect(MenuContext& firstMenu, MenuContext& secondMenu, MenuContext& thirdMenu, vector<Key> keyMenu, int  width = 0, int height = 0);
void printProfile(MenuContext& mainMenu, NhanVien *& danhSach);
void newProfile(MenuContext& mainMenu, NhanVien *& danhSach);
void arrangeMenu(MenuContext& mainMenu, NhanVien *& danhSach);
void searchProfile(MenuContext& mainMenu, NhanVien *& danhSach);
void statisticProfile(MenuContext& mainMenu, NhanVien *& danhSach);
void menuGUI();
void printMenu(const vector<string>& options, int selected);
void drawBox(int x, int y, int width, int height, int color);
void loading(const string& message = "Loading", int delay = 50, int length = 30);
void resizeConsole_(int x, int y);
void showSuggest();
void mergeSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);
void quickSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);
void insertionSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);
void selectionSortKeyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);
void keyMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);
void drawFrameKey();
void button(vector<Key> &keyMenu, Key newKeym);
void showKey(vector<Key> &keyMenu);
string keyToString(Key k);
void timKiemChinhXacMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);
void timKiemMotPhanMenu(MenuContext& mainMenu, NhanVien *& danhSach, MenuContext& secondMenu, int MENU_Y);

// ================= NAMESPACE CONSOLE =================
namespace console {
    void gotoxy(short x, short y);//ham tro toi vi tri
    void setColor(int color);//ham set mau
    void cursorStatus(unsigned size, WINBOOL stt);
    void resetColor();
    void clearScreen();
    void clearLine(int x, int y, int width);
}


#endif