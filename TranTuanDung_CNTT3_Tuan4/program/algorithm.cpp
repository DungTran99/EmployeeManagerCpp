#include "algorithm.h"
#include "gui.h"

using namespace std;

//nut bat tat key
void button(vector<Key> &keyMenu, Key newKey){
    if(addKey(keyMenu, newKey)){
        //
    }else if(removeKey(keyMenu, newKey)){
        //
    }
}
//hien thi key da chon
void showKey(vector<Key> &keyMenu){
    int x = 74, y = 16; // cung vi tri voi drawFrameKey()
    console::gotoxy(x,y);
    for(unsigned int i = 0; i< keyMenu.size(); i++){
        cout<<keyToString(keyMenu[i]);
        if( i != keyMenu.size() - 1){
            cout<<"=>";
        }
    }
}

string keyToString(Key k) {
    switch (k) {
        case MAPB: return "maPB";
        case TENPB: return "tenPB";
        case MANV: return "maNV";
        case HOVATEN: return "hoTen";
        case CHUCVU: return "chucVu";
        case NGAYSINH: return "NS";
        case LUONG: return "Luong";
        default: return "???";
    }
}

//thêm key vào va kiểm tra đã tồn tại chưa
bool addKey(vector<Key>& keyMenu, Key newKey) {
    for (unsigned int i = 0; i < keyMenu.size(); ++i) {
        if (keyMenu[i] == newKey) {
            return false;
        }
    }
    keyMenu.push_back(newKey);
    return true;
}
//xóa key theo truyền vào
bool removeKey(vector<Key>& keyMenu, Key keyToRemove) {
    for (unsigned int i = 0; i < keyMenu.size(); ++i) {
        if (keyMenu[i] == keyToRemove) {
            for (unsigned int j = i; j < keyMenu.size() - 1; ++j) {
                keyMenu[j] = keyMenu[j + 1];
            }
            keyMenu.pop_back(); 
            return true;
        }
    }
    return false;
}

void accecptButton(vector<Key> keyMenu, NhanVien *& head, string arrange){
    if(arrange == "SelectionSort"){
        selectionSort(head, keyMenu);
    }else if(arrange =="InsertionSort"){
        insertionSort(head, keyMenu);
    }else if(arrange =="QuickSort"){
        quickSort(head, keyMenu);
    }else if(arrange == "MergeSort"){
        mergeSort(head, keyMenu);
    }
    controlPrintProfile(head);
}
//ham tach ngay thang nam
void parseNgaySinh(const string& ns, int& d, int& m, int& y) {
    d = stoi(ns.substr(0, 2));
    m = stoi(ns.substr(3, 2));
    y = stoi(ns.substr(6, 4));
}


int compareNhanVien(nv* a, nv* b, const vector<Key>& keys) {
    for (Key k : keys) {
        if (k == MAPB) {
            if (a->maPhongBan != b->maPhongBan)
                return a->maPhongBan < b->maPhongBan ? -1 : 1;
        } else if (k == TENPB) {
            if (a->tenPhongBan != b->tenPhongBan)
                return a->tenPhongBan < b->tenPhongBan ? -1 : 1;
        } else if (k == MANV) {
            if (a->maNhanVien != b->maNhanVien)
                return a->maNhanVien < b->maNhanVien ? -1 : 1;
        } else if (k == HOVATEN) {
            string tenA = a->hoTen.substr(a->hoTen.find_last_of(' ') + 1);
            string tenB = b->hoTen.substr(b->hoTen.find_last_of(' ') + 1);
            if (tenA != tenB)
                return tenA < tenB ? -1 : 1;
            if (a->hoTen != b->hoTen)
                return a->hoTen < b->hoTen ? -1 : 1;
        } else if (k == CHUCVU) {
            if (a->chucVu != b->chucVu)
                return a->chucVu < b->chucVu ? -1 : 1;
        } else if (k == NGAYSINH) {
            int da, ma, ya, db, mb, yb;
            parseNgaySinh(a->ngaySinh, da, ma, ya);
            parseNgaySinh(b->ngaySinh, db, mb, yb);
            if (ya != yb) return ya < yb ? -1 : 1;
            if (ma != mb) return ma < mb ? -1 : 1;
            if (da != db) return da < db ? -1 : 1;
        } else if (k == LUONG) {
            if (a->luong != b->luong)
                return a->luong < b->luong ? -1 : 1;
        }
    }
    return 0; // bằng nhau
}
void selectionSort(nv*& head, const vector<Key>& keys) {
    for (nv* i = head; i != nullptr; i = i->next) {
        nv* minNode = i;
        for (nv* j = i->next; j != nullptr; j = j->next) {
            if (compareNhanVien(j, minNode, keys) < 0) {
                minNode = j;
            }
        }
        if (minNode != i) {
            // Hoán đổi toàn bộ nội dung của 2 node
            swap(i->tenPhongBan, minNode->tenPhongBan);
            swap(i->maPhongBan, minNode->maPhongBan);
            swap(i->maNhanVien, minNode->maNhanVien);
            swap(i->hoTen, minNode->hoTen);
            swap(i->chucVu, minNode->chucVu);
            swap(i->ngaySinh, minNode->ngaySinh);
            swap(i->luong, minNode->luong);
        }
    }
}

void insertionSort(nv*& head, const vector<Key>& keys) {
    if (!head || !head->next) return;

    nv* sorted = nullptr;

    nv* current = head;
    while (current != nullptr) {
        nv* next = current->next;

        if (!sorted || compareNhanVien(current, sorted, keys) < 0) {
            current->next = sorted;
            if (sorted) sorted->prev = current;
            sorted = current;
            sorted->prev = nullptr;
        } else {
            nv* temp = sorted;
            while (temp->next && compareNhanVien(current, temp->next, keys) >= 0)
                temp = temp->next;

            current->next = temp->next;
            if (temp->next) temp->next->prev = current;
            temp->next = current;
            current->prev = temp;
        }
        current = next;
    }

    head = sorted;
}
//mergesort
void splitList(nv* head, nv** frontRef, nv** backRef) {
    nv* slow = head;
    nv* fast = head->next;

    while (fast != nullptr) {
        fast = fast->next;
        if (fast != nullptr) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *frontRef = head;
    *backRef = slow->next;
    if (*backRef) (*backRef)->prev = nullptr;
    slow->next = nullptr;
}
nv* sortedMerge(nv* a, nv* b, const vector<Key>& keys) {
    if (!a) return b;
    if (!b) return a;

    nv* result = nullptr;

    if (compareNhanVien(a, b, keys) <= 0) {
        result = a;
        result->next = sortedMerge(a->next, b, keys);
        if (result->next) result->next->prev = result;
        result->prev = nullptr;
    } else {
        result = b;
        result->next = sortedMerge(a, b->next, keys);
        if (result->next) result->next->prev = result;
        result->prev = nullptr;
    }

    return result;
}

void mergeSort(nv*& headRef, const vector<Key>& keys) {
    if (!headRef || !headRef->next) return;

    nv* a = nullptr;
    nv* b = nullptr;

    splitList(headRef, &a, &b);

    mergeSort(a, keys);
    mergeSort(b, keys);

    headRef = sortedMerge(a, b, keys);
}

//quicksort
//tim node cuoi
nv* getTail(nv* cur) {
    while (cur && cur->next) cur = cur->next;
    return cur;
}
nv* partition(nv* head, nv* end, nv** newHead, nv** newEnd, const vector<Key>& keys) {
    nv* pivot = end;
    nv* prev = nullptr, *cur = head, *tail = pivot;

    while (cur != pivot) {
        if (compareNhanVien(cur, pivot, keys) < 0) {
            if (!*newHead) *newHead = cur;
            prev = cur;
            cur = cur->next;
        } else {
            if (prev) prev->next = cur->next;
            nv* tmp = cur->next;
            cur->next = nullptr;
            tail->next = cur;
            tail = cur;
            cur = tmp;
        }
    }

    if (!*newHead) *newHead = pivot;
    *newEnd = tail;
    return pivot;
}
nv* quickSortRecur(nv* head, nv* end, const vector<Key>& keys) {
    if (!head || head == end) return head;

    nv *newHead = nullptr, *newEnd = nullptr;

    nv* pivot = partition(head, end, &newHead, &newEnd, keys);

    if (newHead != pivot) {
        nv* temp = newHead;
        while (temp->next != pivot) temp = temp->next;
        temp->next = nullptr;

        newHead = quickSortRecur(newHead, temp, keys);

        temp = getTail(newHead);
        temp->next = pivot;
    }

    pivot->next = quickSortRecur(pivot->next, newEnd, keys);

    return newHead;
}

void quickSort(nv*& head, const vector<Key>& keys) {
    head = quickSortRecur(head, getTail(head), keys);
}
