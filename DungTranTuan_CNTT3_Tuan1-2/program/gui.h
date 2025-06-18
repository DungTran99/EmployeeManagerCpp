#ifndef GUI_H
#define GUI_H
#include <conio.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
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
const int MENU_X = 0;
const int MENU_Y = 14;
const int SECOND_MENU_X = 24;
const int THIRD_MENU_X = 47;
const int PRINT_OFFSET_Y = 1;
const int SORT_OFFSET_Y = 2;
const int KEY_OFFSET_Y = 2;
const int SEARCH_OFFSET_Y = 3;
const int STATISTIC_OFFSET_Y = 4;

#define KEY_RIGHT 77
#define KEY_LEFT 75
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER '\r'


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
int firstSelect(MenuContext& firstMenu);
int secondSelect(const MenuContext& firstMenu, MenuContext& secondMenu);
int thirdSelect(MenuContext& firstMenu, MenuContext& secondMenu, MenuContext& thirdMenu);
void printProfile(MenuContext& mainMenu);
void newProfile(MenuContext& mainMenu);
void keyProfile(MenuContext& mainMenu, MenuContext& secondMenu, int MENU_Y);
void sortProfile(MenuContext& mainMenu);
void searchProfile(MenuContext& mainMenu);
void statisticProfile(MenuContext& mainMenu);
void menuGUI();
void printMenu(const vector<string>& options, int selected);
void drawBox(int x, int y, int width, int height, const string& title, int color);

// ================= NAMESPACE CONSOLE =================
namespace console {
    void gotoxy(short x, short y);//ham tro toi vi tri
    void setColor(int color);//ham set mau
    void cursorStatus(unsigned size, WINBOOL stt);
    void resetColor();
    void clearScreen();
}

#endif