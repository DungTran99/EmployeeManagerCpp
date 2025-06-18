#ifndef SHOWPROFILE_H
#define SHOWPROFILE_H

#include <iostream>
#include <string>
#include <iomanip>      
#include "gui.h" 
#include "search.h"
//======== KHAI BAO ==========
const int soDongMotTrang = 20;

int demSoNhanVien(NhanVien* head);
void drawDanhSachNhanVien(NhanVien* head, int page = 1);
void drawTableHeader(int x, int y);
void controlPrintProfile(NhanVien* head);
void controlPrintProfileHighlight(NhanVien* head, Key key, const string& searchValue,NhanVien* danhSach = nullptr);
void controlPrintProfileHighlightMulti(NhanVien* head, const DieuKienTim& dk, NhanVien* danhSach = nullptr);
void drawDanhSachNhanVienHighlightMulti(NhanVien* head, int page, const DieuKienTim& dk);

#endif