// algrithorm.h
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <vector>
#include <iostream>


using namespace std;

// ================= BUTTON =======================

bool removeKey(vector<Key>& keyMenu, Key keyToRemove);
bool addKey(vector<Key>& keyMenu, Key newKey);
void accecptButton(vector<Key> keyMenu);
void selectionSort(nv*& head, const vector<Key>& keys);
int compareNhanVien(nv* a, nv* b, const vector<Key>& keys);
void insertionSort(nv*& head, const vector<Key>& keys);
void mergeSort(nv*& headRef, const vector<Key>& keys);
nv* sortedMerge(nv* a, nv* b, const vector<Key>& keys);
void splitList(nv* head, nv** frontRef, nv** backRef);
nv* quickSortRecur(nv* head, nv* end, const vector<Key>& keys);
nv* partition(nv* head, nv* end, nv** newHead, nv** newEnd, const vector<Key>& keys);
void quickSort(nv*& head, const vector<Key>& keys);
#endif