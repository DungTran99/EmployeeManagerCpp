#ifndef SHOWPROFILE_H
#define SHOWPROFILE_H

#include <iostream>
#include <string>

//======== KHAI BAO ==========
const int soDongMotTrang = 20;

int demSoNhanVien(NhanVien* head);
void drawDanhSachNhanVien(NhanVien* head, int page = 1);
void drawTableHeader(int x, int y);
void controlPrintProfile(NhanVien* head);

#endif