#ifndef CUOIKY_H
#define CUOIKY_H
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
using namespace std;

void menuSapXep(NhanVien *head);
int compareLuong(NhanVien* a, NhanVien* b, const vector<Key>& keys);
void splitListNew(NhanVien* head, NhanVien** frontRef, NhanVien** backRef);
NhanVien* sortedMergeNew(NhanVien* a, NhanVien* b, const vector<Key>& keys);
void mergeSortNew(NhanVien*& headRef, const vector<Key>& keys);
void parseNgaySinh(const string& ns, int& d, int& m, int& y);

#endif