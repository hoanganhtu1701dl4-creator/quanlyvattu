#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <algorithm> // Cho std::transform
#include <cctype>    // Cho std::toupper
#include <limits>    // Cho numeric_limits

using namespace std;

// <<< SỬA >>> Hàm "Chờ Enter" đa nền tảng
void pressEnterToContinue() {
    cout << "\nNhan Enter de tiep tuc...";
    // Xóa bộ đệm đầu vào cho đến khi gặp \n
    // Đảm bảo nó hoạt động sau cả cin >> và getline
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (cin.peek() == '\n') {
        cin.get();
    }
    cin.get(); // Chờ người dùng nhấn Enter
}


// <<< FIX 1: BỔ SUNG CÁC HÀM NHẬP LIỆU AN TOÀN >>>

// Hàm xóa dấu cách thừa ở đầu và cuối chuỗi
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return ""; // Trả về chuỗi rỗng nếu toàn dấu cách
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Hàm chuyển chuỗi sang chữ HOA
string toUpper(string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::toupper(c); }
    );
    return s;
}

// <<< BỔ SUNG >>> Hàm kiểm tra chuỗi chỉ chứa chữ cái và dấu cách
bool isAlphaWithSpaces(const string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        // Phải ép kiểu (unsigned char) để isalpha hoạt động đúng
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return false; // Tìm thấy ký tự không phải chữ/cách
        }
    }
    return true; // Hợp lệ
}

// <<< BỔ SUNG >>> Hàm kiểm tra chuỗi chỉ chứa SỐ
bool isAllDigits(const string& str) {
    if (str.empty()) return false; // Không được rỗng
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit((unsigned char)str[i])) {
            return false; // Tìm thấy ký tự không phải số
        }
    }
    return true;
}


/*
 * Hàm nhập std::string (cho Tên, Họ, DVT, TENVT)
 * Bắt buộc nhập lại nếu chuỗi rỗng hoặc chỉ có dấu cách
 */
string nhapChuoiBatBuoc(string prompt) {
    string input;
    do {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (input.empty()) {
            cout << "Loi: Khong duoc de trong! Vui long nhap lai.\n";
        }
    } while (input.empty());
    return input;
}

// <<< BỔ SUNG >>> Hàm nhập chuỗi (chỉ chấp nhận chữ cái và dấu cách)
string nhapChuoiChuBatBuoc(string prompt) {
    string input;
    bool hopLe;
    do {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        hopLe = true;

        if (input.empty()) {
            cout << "Loi: Khong duoc de trong! Vui long nhap lai.\n";
            hopLe = false;
        }
        else if (!isAlphaWithSpaces(input)) {
            cout << "Loi: Truong nay chi duoc phep nhap chu cai va dau cach (khong nhap so hoac ky tu dac biet).\n";
            hopLe = false;
        }
    } while (!hopLe);
    return input;
}


/*
 * Hàm nhập mảng char[] (cho SoHD)
 */
void nhapChuoiCharBatBuoc(string prompt, char* dest, int maxLength, bool autoTuUpper = false) {
    string input;
    do {
        cout << prompt;
        getline(cin, input);
        input = trim(input);

        if (autoTuUpper) {
            input = toUpper(input);
        }

        if (input.empty()) {
            cout << "Loi: Khong duoc de trong! Vui long nhap lai.\n";
        }
        else if (input.length() >= (size_t)maxLength) {
            cout << "Loi: Vuot qua do dai toi da (" << maxLength - 1 << " ky tu). Vui long nhap lai.\n";
            input = ""; // Đặt lại để vòng lặp chạy tiếp
        }
    } while (input.empty());

    strncpy(dest, input.c_str(), maxLength - 1); 
    dest[maxLength - 1] = '\0'; // Đảm bảo luôn kết thúc bằng null
}

// -- CẢI TIẾN GIAO DIỆN (UI) --
void clearScreen() {
#ifdef _WIN32
    system("cls"); // Giữ lại cho Windows
#else
    system("clear"); // Dùng cho Linux/macOS
#endif
}

// -- HÀM HỖ TRỢ --
int customMax(int a, int b) {
    return (a > b) ? a : b;
}

// -- KHAI BÁO CẤU TRÚC DỮ LIỆU --
// (Không thay đổi)
// === 1. Cấu trúc Vật Tư (Cây AVL) ===
struct Vattu {
    char MAVT[11];
    string TENVT;
    string DVT;
    int SoLuongTon;
};

struct NodeVattu {
    Vattu info;
    NodeVattu* left;
    NodeVattu* right;
    int height;
};

struct TreeVattu {
    NodeVattu* root;
};

// === 2. Cấu trúc Chi Tiết Hóa Đơn (DSLK Đơn) ===
struct CT_HoaDon {
    char MAVT[11];
    int SoLuong;
    float DonGia;
    float VAT;
};

struct NodeCT_HoaDon {
    CT_HoaDon info;
    NodeCT_HoaDon* next;
};

struct ListCT_HoaDon {
    NodeCT_HoaDon* head;
};

// === 3. Cấu trúc Hóa Đơn (DSLK Đơn) ===
struct Date {
    int ngay, thang, nam;
};

struct HoaDon {
    char SoHD[21];
    Date NgayLap;
    char Loai; // 'N' hoặc 'X'
    ListCT_HoaDon dscthd;
};

struct NodeHoaDon {
    HoaDon info;
    NodeHoaDon* next;
};

struct ListHoaDon {
    NodeHoaDon* head;
};

// === 4. Cấu trúc Nhân Viên (Mảng con trỏ) ===
const int MAX_NHANVIEN = 500;

struct Nhanvien {
    int MANV;
    string HO;
    string TEN;
    string GIOITINH;
    ListHoaDon dshd;
    Nhanvien() {
        MANV = 0;
        dshd.head = NULL;
    }
};

struct ListNhanVien {
    Nhanvien* dsnv[MAX_NHANVIEN];
    int soLuong;
};

// -- KHAI BÁO BIẾN TOÀN CỤC --
TreeVattu dsVatTu;
ListNhanVien dsNhanVien;

// -- CÁC HÀM XỬ LÝ CÂY AVL (VẬT TƯ) --
// (Không thay đổi)
void initTree(TreeVattu& t) {
    t.root = NULL;
}

int getHeight(NodeVattu* node) {
    if (node == NULL) return 0;
    return node->height;
}

int getBalance(NodeVattu* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

NodeVattu* createNodeVattu(Vattu vt) {
    NodeVattu* p = new NodeVattu;
    p->info = vt;
    p->left = NULL;
    p->right = NULL;
    p->height = 1;
    return p;
}

NodeVattu* rightRotate(NodeVattu* y) {
    NodeVattu* x = y->left;
    NodeVattu* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = customMax(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = customMax(getHeight(x->left), getHeight(x->right)) + 1;
    return x;
}

NodeVattu* leftRotate(NodeVattu* x) {
    NodeVattu* y = x->right;
    NodeVattu* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = customMax(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = customMax(getHeight(y->left), getHeight(y->right)) + 1;
    return y;
}

NodeVattu* insertVattu(NodeVattu* node, Vattu vt) {
    if (node == NULL) {
        return createNodeVattu(vt);
    }
    if (strcmp(vt.MAVT, node->info.MAVT) < 0)
        node->left = insertVattu(node->left, vt);
    else if (strcmp(vt.MAVT, node->info.MAVT) > 0)
        node->right = insertVattu(node->right, vt);
    else {
        return node;
    }

    node->height = 1 + customMax(getHeight(node->left), getHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && strcmp(vt.MAVT, node->left->info.MAVT) < 0)
        return rightRotate(node);
    if (balance < -1 && strcmp(vt.MAVT, node->right->info.MAVT) > 0)
        return leftRotate(node);
    if (balance > 1 && strcmp(vt.MAVT, node->left->info.MAVT) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && strcmp(vt.MAVT, node->right->info.MAVT) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

NodeVattu* findVattu(NodeVattu* root, char mavt[]) {
    if (root == NULL || strcmp(root->info.MAVT, mavt) == 0) {
        return root;
    }
    if (strcmp(mavt, root->info.MAVT) < 0) {
        return findVattu(root->left, mavt);
    }
    return findVattu(root->right, mavt);
}

NodeVattu* findMinNode(NodeVattu* node) {
    NodeVattu* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

NodeVattu* deleteVattu(NodeVattu* root, char mavt[]) {
    if (root == NULL) return root;

    if (strcmp(mavt, root->info.MAVT) < 0)
        root->left = deleteVattu(root->left, mavt);
    else if (strcmp(mavt, root->info.MAVT) > 0)
        root->right = deleteVattu(root->right, mavt);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            NodeVattu* temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            }
            else {
                *root = *temp;
            }
            delete temp;
        }
        else {
            NodeVattu* temp = findMinNode(root->right);
            root->info = temp->info;
            root->right = deleteVattu(root->right, temp->info.MAVT);
        }
    }

    if (root == NULL) return root;

    root->height = 1 + customMax(getHeight(root->left), getHeight(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}


// ---------------------------------------------
// -- CHỨC NĂNG A: QUẢN LÝ VẬT TƯ --
// ---------------------------------------------

// <<< SỬA >>> Cập nhật hàm xuLyThemVatTu
void xuLyThemVatTu() {
    Vattu vt;
    cout << "\n-- Them Vat Tu Moi --\n";

    // --- Bắt đầu Validation MAVT ---
    string inputNum;
    string finalMa;
   
    int maxDigits = 11 - 1 - 2; 
    bool hopLe = false;
    
    do {
        cout << "Nhap Ma Vat Tu  " << maxDigits << "  ";
        getline(cin, inputNum);
        inputNum = trim(inputNum);

        if (inputNum.empty()) {
            cout << "Loi: Khong duoc de trong!\n";
        } else if (!isAllDigits(inputNum)) {
            cout << "Loi: Phan so cua ma vat tu chi duoc phep chua SO (0-9).\n";
        } else if (inputNum.length() > (size_t)maxDigits) {
            cout << "Loi: Phan so qua dai (toi da " << maxDigits << " so).\n";
        } else {
            finalMa = "VT" + inputNum;
            finalMa = toUpper(finalMa); 
            
            // Chuyển string sang char[] để kiểm tra
            char tempMa[11];
            strncpy(tempMa, finalMa.c_str(), 10);
            tempMa[10] = '\0';

            if (findVattu(dsVatTu.root, tempMa) != NULL) {
                cout << "Loi: Ma vat tu '" << finalMa << "' da ton tai!\n";
            } else {
                hopLe = true;
                strncpy(vt.MAVT, tempMa, 11); // Lưu mã hợp lệ
            }
        }
    } while (!hopLe);
    // --- Kết thúc Validation MAVT ---


    // --- SỬA --- Validation TENVT (chỉ chữ)
    vt.TENVT = nhapChuoiChuBatBuoc("Nhap Ten Vat Tu: ");
    
    // --- Validation DVT (không rỗng) ---
    // Giữ nguyên, DVT có thể là "cai", "m2", "kg"
    vt.DVT = nhapChuoiBatBuoc("Nhap Don Vi Tinh: "); 


    // --- SỬA --- Validation SoLuongTon (số >= 0)
    vt.SoLuongTon = -1; // Đặt giá trị không hợp lệ ban đầu
    do {
        cout << "Nhap So Luong Ton (so nguyen >= 0): ";
        cin >> vt.SoLuongTon;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Loi: So luong ton phai la mot SO nguyen.\n";
            vt.SoLuongTon = -1; // Đặt lại để lặp
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (vt.SoLuongTon < 0) {
                cout << "Loi: So luong ton phai lon hon hoac bang 0.\n";
            }
        }
    } while (vt.SoLuongTon < 0);
    // --- Kết thúc Validation SoLuongTon ---

    dsVatTu.root = insertVattu(dsVatTu.root, vt);
    cout << "Da them vat tu thanh cong!\n";
}

void xuLyXoaVatTu() {
    char mavt[11];
    cout << "\n-- Xoa Vat Tu --\n";

    // <<< SỬA >>> Yêu cầu nhập theo format mới
    string inputNum;
    string finalMa;
    int maxDigits = 11 - 1 - 2;
    bool hopLe = false;
    
    do {
        cout << "Nhap PHAN SO cua Ma Vat Tu can xoa (vi du: 001): ";
        getline(cin, inputNum);
        inputNum = trim(inputNum);

        if (inputNum.empty()) {
            cout << "Loi: Khong duoc de trong!\n";
        } else if (!isAllDigits(inputNum)) {
            cout << "Loi: Phan so cua ma vat tu chi duoc phep chua SO (0-9).\n";
        } else if (inputNum.length() > (size_t)maxDigits) {
            cout << "Loi: Phan so qua dai (toi da " << maxDigits << " so).\n";
        } else {
            finalMa = "VT" + inputNum;
            finalMa = toUpper(finalMa); 
            strncpy(mavt, finalMa.c_str(), 10);
            mavt[10] = '\0';
            hopLe = true;
        }
    } while (!hopLe);
    // --- Kết thúc Validation MAVT ---

    NodeVattu* node = findVattu(dsVatTu.root, mavt);
    if (node == NULL) {
        cout << "Loi: Khong tim thay vat tu co ma " << mavt << endl;
        return;
    }
    
    dsVatTu.root = deleteVattu(dsVatTu.root, mavt);
    cout << "Da xoa vat tu thanh cong!\n";
}

void xuLyHieuChinhVatTu() {
    char mavt[11];
    cout << "\n-- Hieu Chinh Vat Tu --\n";

    // <<< SỬA >>> Yêu cầu nhập theo format mới
    string inputNum;
    string finalMa;
    int maxDigits = 11 - 1 - 2;
    bool hopLe = false;
    
    do {
        cout << "Nhap PHAN SO cua Ma Vat Tu can hieu chinh (vi du: 001): ";
        getline(cin, inputNum);
        inputNum = trim(inputNum);

        if (inputNum.empty()) {
            cout << "Loi: Khong duoc de trong!\n";
        } else if (!isAllDigits(inputNum)) {
            cout << "Loi: Phan so cua ma vat tu chi duoc phep chua SO (0-9).\n";
        } else if (inputNum.length() > (size_t)maxDigits) {
            cout << "Loi: Phan so qua dai (toi đa " << maxDigits << " so).\n";
        } else {
            finalMa = "VT" + inputNum;
            finalMa = toUpper(finalMa); 
            strncpy(mavt, finalMa.c_str(), 10);
            mavt[10] = '\0';
            hopLe = true;
        }
    } while (!hopLe);
    // --- Kết thúc Validation MAVT ---

    NodeVattu* node = findVattu(dsVatTu.root, mavt);
    if (node == NULL) {
        cout << "Loi: Khong tim thay vat tu co ma " << mavt << endl;
        return;
    }

    cout << "Tim thay vat tu: " << node->info.TENVT << endl;
    
    // <<< SỬA >>> Yêu cầu nhập tên mới cũng phải là chữ
    cout << "Nhap Ten Vat Tu moi (Enter de giu nguyen - [" << node->info.TENVT << "]): ";
    string tenMoi;
    getline(cin, tenMoi);
    tenMoi = trim(tenMoi); // Xóa cách thừa
    if (!tenMoi.empty()) {
        if (isAlphaWithSpaces(tenMoi)) {
             node->info.TENVT = tenMoi;
        } else {
            cout << "Loi: Ten vat tu moi chi duoc chua chu cai. Ten chua duoc cap nhat.\n";
        }
    }

    // DVT có thể chứa số (m2, kg), nên giữ nguyên logic
    cout << "Nhap Don Vi Tinh moi (Enter de giu nguyen - [" << node->info.DVT << "]): ";
    string dvtMoi;
    getline(cin, dvtMoi);
    dvtMoi = trim(dvtMoi); // Xóa cách thừa
    if (!dvtMoi.empty()) {
        node->info.DVT = dvtMoi;
    }

    cout << "Da cap nhat thong tin vat tu!\n";
}

void menuQuanLyVatTu() {
    int choice = -1;

    do {
        clearScreen();
        cout << "\n--- Quan Ly Vat Tu ---\n";
        cout << "1. Them Vat Tu\n";
        cout << "2. Xoa Vat Tu\n";
        cout << "3. Hieu Chinh Vat Tu\n";
        cout << "0. Quay lai Menu Chinh\n";
        cout << "Lua chon cua ban: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1; // Đặt lại choice
        } else {
             cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa \n
        }

        clearScreen();

        switch (choice) {
        case 1:
            xuLyThemVatTu();
            break;
        case 2:
            xuLyXoaVatTu();
            break;
        case 3:
            xuLyHieuChinhVatTu();
            break;
        case 0:
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }

        if (choice != 0) {
            pressEnterToContinue();
        }

    } while (choice != 0);
}


// --------------------------------------------------------
// -- CHỨC NĂNG B: IN DS VẬT TƯ (THEO TÊN TĂNG DẦN) --
// (Không thay đổi)
// --------------------------------------------------------
int demSoVatTu(NodeVattu* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + demSoVatTu(root->left) + demSoVatTu(root->right);
}

void dienVaoMang(NodeVattu* root, NodeVattu** dsTam, int& index) {
    if (root != NULL) {
        dsTam[index++] = root;
        dienVaoMang(root->left, dsTam, index);
        dienVaoMang(root->right, dsTam, index);
    }
}

void swapConTro(NodeVattu** a, NodeVattu** b) {
    NodeVattu* t = *a;
    *a = *b;
    *b = t;
}

int partition(NodeVattu** arr, int low, int high) {
    NodeVattu* pivot = arr[high];
    string tenPivot = pivot->info.TENVT;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->info.TENVT < tenPivot) {
            i++;
            swapConTro(&arr[i], &arr[j]);
        }
    }
    swapConTro(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSortTheoTen(NodeVattu** arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortTheoTen(arr, low, pi - 1);
        quickSortTheoTen(arr, pi + 1, high);
    }
}

void inDanhSachVatTuTonKho_TheoTen() {
    cout << "\n--- DANH SACH VAT TU TON KHO (Theo TEN VT tang dan) ---\n";
    cout << "------------------------------------------------------------------\n";
    cout << left
        << setw(12) << "Ma VT"
        << setw(30) << "Ten vat tu"
        << setw(15) << "Don vi tinh"
        << setw(10) << right << "So luong ton" << endl;
    cout << "------------------------------------------------------------------\n";

    if (dsVatTu.root == NULL) {
        cout << "Danh sach vat tu rong.\n";
        cout << "------------------------------------------------------------------\n";
        return;
    }

    int soLuongVT = demSoVatTu(dsVatTu.root);
    NodeVattu** dsTam = new NodeVattu * [soLuongVT];
    int index = 0;
    dienVaoMang(dsVatTu.root, dsTam, index);
    quickSortTheoTen(dsTam, 0, soLuongVT - 1);

    for (int i = 0; i < soLuongVT; i++) {
        Vattu& vt = dsTam[i]->info;
        cout << left
            << setw(12) << vt.MAVT
            << setw(30) << vt.TENVT
            << setw(15) << vt.DVT
            << setw(10) << right << vt.SoLuongTon << endl;
    }

    cout << "------------------------------------------------------------------\n";
    delete[] dsTam;
}


// ------------------------------------------------
// -- CHỨC NĂNG C & D: QUẢN LÝ NHÂN VIÊN --
// (Không thay đổi từ lần trước)
// ------------------------------------------------
int timNhanVienTheoMa(int manv) {
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        if (dsNhanVien.dsnv[i]->MANV == manv) {
            return i;
        }
    }
    return -1;
}

int timViTriChen(Nhanvien* nvMoi) {
    int i = 0;
    while (i < dsNhanVien.soLuong) {
        Nhanvien* nvHienTai = dsNhanVien.dsnv[i];

        if (nvMoi->TEN < nvHienTai->TEN) {
            return i;
        }
        else if (nvMoi->TEN == nvHienTai->TEN) {
            if (nvMoi->HO < nvHienTai->HO) {
                return i;
            }
        }
        i++;
    }
    return i;
}

void xuLyThemNhanVien() {
    cout << "\n-- Them Nhan Vien Moi --\n";

    if (dsNhanVien.soLuong >= MAX_NHANVIEN) {
        cout << "Loi: Danh sach nhan vien da day! Khong The them.\n";
        return;
    }

    Nhanvien* nvMoi = new Nhanvien;

    // --- Bắt đầu Validation ---
    
    // 1. Validation Mã Nhân Viên (MANV)
    do {
        cout << "Nhap Ma Nhan Vien (so nguyen > 0): ";
        cin >> nvMoi->MANV;

        // Xử lý nếu người dùng nhập chữ
        if (cin.fail()) {
            cin.clear(); // Xóa cờ lỗi
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm
            cout << "Loi: Ma Nhan Vien phai la mot SO nguyen.\n";
            nvMoi->MANV = 0; // Đặt lại giá trị 0 để lặp lại
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa \n nếu nhập đúng
            
            // Kiểm tra > 0
            if (nvMoi->MANV <= 0) {
                cout << "Loi: Ma Nhan Vien phai la so duong (> 0).\n";
            }
            // Kiểm tra trùng lặp
            else if (timNhanVienTheoMa(nvMoi->MANV) != -1) {
                cout << "Loi: Ma Nhan Vien da ton tai!\n";
                delete nvMoi;
                return; // Thoát nếu trùng
            }
        }
    } while (nvMoi->MANV <= 0); // Lặp lại nếu nhập sai hoặc <= 0

    
    // 2. Validation Họ và Tên (Chỉ chứa chữ)
    nvMoi->HO = nhapChuoiChuBatBuoc("Nhap Ho: ");
    nvMoi->TEN = nhapChuoiChuBatBuoc("Nhap Ten: ");

    // 3. Validation Giới Tính (Chỉ NAM hoặc NU)
    do {
        nvMoi->GIOITINH = nhapChuoiChuBatBuoc("Nhap gioi tinh (Nam/Nu): ");
        nvMoi->GIOITINH = toUpper(nvMoi->GIOITINH); // Chuyển sang HOA để so sánh

        if (nvMoi->GIOITINH != "NAM" && nvMoi->GIOITINH != "NU") {
            cout << "Loi: Gioi tinh chi duoc phep la 'Nam' hoac 'Nu'.\n";
        }
    } while (nvMoi->GIOITINH != "NAM" && nvMoi->GIOITINH != "NU");

    // --- Kết thúc Validation ---

    nvMoi->dshd.head = NULL;

    // Yêu cầu "DS tăng dần" của bạn đã được xử lý bằng hàm này
    int viTriChen = timViTriChen(nvMoi);

    for (int j = dsNhanVien.soLuong; j > viTriChen; j--) {
        dsNhanVien.dsnv[j] = dsNhanVien.dsnv[j - 1];
    }

    dsNhanVien.dsnv[viTriChen] = nvMoi;
    dsNhanVien.soLuong++;

    cout << "Da them nhan vien thanh cong!\n";
}

void inDanhSachNhanVien() {
    cout << "\n--- DANH SACH NHAN VIEN (Theo Ten -> Ho) ---\n";
    cout << "------------------------------------------------------------------\n";
    cout << left
        << setw(10) << "MA NV"
        << setw(20) << "HO"
        << setw(15) << "TEN"
        << setw(10) << "GIOI TINH" << endl;
    cout << "------------------------------------------------------------------\n";

    if (dsNhanVien.soLuong == 0) {
        cout << "Danh sach nhan vien rong.\n";
    }
    else {
        for (int i = 0; i < dsNhanVien.soLuong; i++) {
            Nhanvien* nv = dsNhanVien.dsnv[i];
            cout << left
                << setw(10) << nv->MANV
                << setw(20) << nv->HO
                << setw(15) << nv->TEN
                << setw(10) << nv->GIOITINH << endl;
        }
    }
    cout << "------------------------------------------------------------------\n";
}


// --------------------------------------------
// -- CHỨC NĂNG E: LẬP HÓA ĐƠN NHẬP/XUẤT --
// (Không thay đổi)
// --------------------------------------------

bool kiemTraTrungSoHD(char soHD[]) {
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        ListHoaDon& dshd = dsNhanVien.dsnv[i]->dshd;
        NodeHoaDon* p = dshd.head;
        while (p != NULL) {
            if (strcmp(p->info.SoHD, soHD) == 0) {
                return true;
            }
            p = p->next;
        }
    }
    return false;
}

void themCT_HoaDon(ListCT_HoaDon& dscthd, CT_HoaDon cthd) {
    NodeCT_HoaDon* p = new NodeCT_HoaDon;
    p->info = cthd;
    p->next = dscthd.head;
    dscthd.head = p;
}

void inDanhSachCT_TamThoi(ListCT_HoaDon dscthd) {
    cout << "\n    --- Chi Tiet Hoa Don Hien Tai ---\n";
    if (dscthd.head == NULL) {
        cout << "    (Chua co vat tu nao)\n";
        return;
    }
    cout << "    " << left << setw(12) << "MAVT" << setw(10) << "SO LUONG"
        << setw(15) << "DON GIA" << "VAT (%)\n";
    NodeCT_HoaDon* p = dscthd.head;
    while (p != NULL) {
        cout << "    " << left << setw(12) << p->info.MAVT
            << setw(10) << p->info.SoLuong
            << setw(15) << p->info.DonGia
            << p->info.VAT << endl;
        p = p->next;
    }
    cout << "    -------------------------------------\n";
}

bool xoaCT_HoaDon_TamThoi(ListCT_HoaDon& dscthd, char mavtXoa[]) {
    NodeCT_HoaDon* pDel = dscthd.head, * pPrev = NULL;

    while (pDel != NULL && strcmp(pDel->info.MAVT, mavtXoa) != 0) {
        pPrev = pDel;
        pDel = pDel->next;
    }

    if (pDel == NULL) {
        return false;
    }

    if (pPrev == NULL) {
        dscthd.head = pDel->next;
    }
    else {
        pPrev->next = pDel->next;
    }

    delete pDel;
    return true;
}

void giaiPhongDS_CTHD(ListCT_HoaDon& dscthd) {
    NodeCT_HoaDon* p;
    while (dscthd.head != NULL) {
        p = dscthd.head;
        dscthd.head = p->next;
        delete p;
    }
}


void xuLyLapHoaDon() {
    cout << "\n--- LAP HOA DON MOI ---\n";

    int manv;
    cout << "Nhap Ma Nhan Vien lap hoa don: ";
    cin >> manv;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    int indexNV = timNhanVienTheoMa(manv);
    if (indexNV == -1) {
        cout << "Loi: Khong tim thay nhan vien co ma " << manv << endl;
        return;
    }
    Nhanvien* nvLapHD = dsNhanVien.dsnv[indexNV];
    cout << "Nhan vien lap hoa don: " << nvLapHD->HO << " " << nvLapHD->TEN << endl;

    NodeHoaDon* pHD = new NodeHoaDon;
    pHD->info.dscthd.head = NULL;
    pHD->next = NULL;

    nhapChuoiCharBatBuoc("Nhap So Hoa Don (max 20 ky tu): ", pHD->info.SoHD, 21, true);

    if (kiemTraTrungSoHD(pHD->info.SoHD)) {
        cout << "Loi: So hoa don nay da ton tai trong he thong!\n";
        delete pHD;
        return;
    }

    cout << "Nhap Ngay Lap (ngay thang nam): ";
    cin >> pHD->info.NgayLap.ngay >> pHD->info.NgayLap.thang >> pHD->info.NgayLap.nam;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Yêu cầu "Chỉ N hoặc X" của bạn đã được xử lý ở đây
    do {
        cout << "Nhap Loai Hoa Don (N: Nhap, X: Xuat): ";
        cin >> pHD->info.Loai;
        if (pHD->info.Loai == 'n') pHD->info.Loai = 'N';
        if (pHD->info.Loai == 'x') pHD->info.Loai = 'X';
    } while (pHD->info.Loai != 'N' && pHD->info.Loai != 'X');
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa \n

    char loaiHD = pHD->info.Loai;
    cout << (loaiHD == 'N' ? "== LAP PHIEU NHAP ==" : "== LAP PHIEU XUAT ==") << endl;


    int choice = -1;

    do {
        clearScreen();
        cout << (loaiHD == 'N' ? "== LAP PHIEU NHAP ==" : "== LAP PHIEU XUAT ==") << endl;
        cout << "So HD: " << pHD->info.SoHD << " | NV: " << nvLapHD->HO << " " << nvLapHD->TEN << endl;
        
        inDanhSachCT_TamThoi(pHD->info.dscthd);
        cout << "\n-- Menu Lap Chi Tiet --\n";
        cout << "1. Them Vat Tu\n";
        cout << "2. Xoa Vat Tu\n";
        cout << "0. Luu Hoa Don & Thoat\n";
        cout << "9. Huy Hoa Don\n";
        cout << "Lua chon cua ban: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1; // Đặt lại choice
        } else {
             cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }


        if (choice == 1) {
            CT_HoaDon cthd;

            // <<< SỬA >>> Nhập MAVT theo format mới
            string inputNum;
            string finalMa;
            int maxDigits = 11 - 1 - 2;
            bool hopLe = false;
            
            do {
                cout << "Nhap PHAN SO cua Ma Vat Tu (vi du: 001): ";
                getline(cin, inputNum);
                inputNum = trim(inputNum);

                if (inputNum.empty()) {
                    cout << "Loi: Khong duoc de trong!\n";
                } else if (!isAllDigits(inputNum)) {
                    cout << "Loi: Phan so cua ma vat tu chi duoc phep chua SO (0-9).\n";
                } else if (inputNum.length() > (size_t)maxDigits) {
                    cout << "Loi: Phan so qua dai (toi da " << maxDigits << " so).\n";
                } else {
                    finalMa = "VT" + inputNum;
                    finalMa = toUpper(finalMa); 
                    strncpy(cthd.MAVT, finalMa.c_str(), 10);
                    cthd.MAVT[10] = '\0';
                    hopLe = true;
                }
            } while (!hopLe);
            // --- Kết thúc Validation MAVT ---

            NodeVattu* vtNode = findVattu(dsVatTu.root, cthd.MAVT);
            if (vtNode == NULL) {
                cout << "Loi: Khong tim thay vat tu nay!\n";
            }
            else {
                cout << "Vat tu: " << vtNode->info.TENVT << " | Ton kho hien tai: " << vtNode->info.SoLuongTon << endl;

                cout << "Nhap So Luong: ";
                cin >> cthd.SoLuong;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                if (loaiHD == 'X' && cthd.SoLuong > vtNode->info.SoLuongTon) {
                    cout << "Loi: So luong xuat vuot qua so luong ton!\n";
                    cout << "So luong ton hien co trong kho: " << vtNode->info.SoLuongTon << endl;
                }
                else {
                    cout << "Nhap Don Gia: ";
                    cin >> cthd.DonGia;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    cout << "Nhap %VAT: ";
                    cin >> cthd.VAT;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    themCT_HoaDon(pHD->info.dscthd, cthd);
                    cout << "Da them vat tu vao hoa don.\n";
                }
            }

        }
        else if (choice == 2) {
            char mavtXoa[11];
            
            // <<< SỬA >>> Nhập MAVT theo format mới
            string inputNum;
            string finalMa;
            int maxDigits = 11 - 1 - 2;
            bool hopLe = false;
            
            do {
                cout << "Nhap PHAN SO cua Ma Vat Tu can xoa (vi du: 001): ";
                getline(cin, inputNum);
                inputNum = trim(inputNum);

                if (inputNum.empty()) {
                    cout << "Loi: Khong duoc de trong!\n";
                } else if (!isAllDigits(inputNum)) {
                    cout << "Loi: Phan so cua ma vat tu chi duoc phep chua SO (0-9).\n";
                } else if (inputNum.length() > (size_t)maxDigits) {
                    cout << "Loi: Phan so qua dai (toi da " << maxDigits << " so).\n";
                } else {
                    finalMa = "VT" + inputNum;
                    finalMa = toUpper(finalMa); 
                    strncpy(mavtXoa, finalMa.c_str(), 10);
                    mavtXoa[10] = '\0';
                    hopLe = true;
                }
            } while (!hopLe);
            // --- Kết thúc Validation MAVT ---

            if (xoaCT_HoaDon_TamThoi(pHD->info.dscthd, mavtXoa)) {
                cout << "Da xoa vat tu khoi hoa don.\n";
            }
            else {
                cout << "Loi: Khong tim thay vat tu nay trong hoa don.\n";
            }

        }
        else if (choice == 0) {
            if (pHD->info.dscthd.head == NULL) {
                cout << "Hoa don rong, khong the luu. Chuyen sang Huy Hoa Don.\n";
                choice = 9;
            }
            else {
                cout << "Bat dau cap nhat so luong ton kho...\n";
                NodeCT_HoaDon* pCT = pHD->info.dscthd.head;
                while (pCT != NULL) {
                    NodeVattu* vtNode = findVattu(dsVatTu.root, pCT->info.MAVT);
                    if (loaiHD == 'N') {
                        vtNode->info.SoLuongTon += pCT->info.SoLuong;
                    }
                    else { // 'X'
                        vtNode->info.SoLuongTon -= pCT->info.SoLuong;
                    }
                    pCT = pCT->next;
                }
                cout << "Da cap nhat so luong ton kho thanh cong!\n";

                pHD->next = nvLapHD->dshd.head;
                nvLapHD->dshd.head = pHD;

                cout << "==> DA LUU HOA DON THANH CONG! <==\n";
            }

        }

        if (choice == 9) {
            giaiPhongDS_CTHD(pHD->info.dscthd);
            delete pHD;
            cout << "Da huy hoa don. Khong co gi duoc luu.\n";
            choice = 0; // Đặt choice = 0 để thoát vòng lặp
        }

        if (choice != 0) {
            pressEnterToContinue();
        }

    } while (choice != 0);
}


// -----------------------------------
// -- CHỨC NĂNG F: IN HÓA ĐƠN --
// (Không thay đổi)
// -----------------------------------
NodeHoaDon* timHoaDon(char soHD[], Nhanvien*& nvTimThay) {
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        ListHoaDon& dshd = dsNhanVien.dsnv[i]->dshd;
        NodeHoaDon* p = dshd.head;
        while (p != NULL) {
            if (strcmp(p->info.SoHD, soHD) == 0) {
                nvTimThay = dsNhanVien.dsnv[i];
                return p;
            }
            p = p->next;
        }
    }
    nvTimThay = NULL;
    return NULL;
}

void xuLyInHoaDon() {
    char soHD_input[21];
    cout << "\n--- IN HOA DON ---\n";
    
    nhapChuoiCharBatBuoc("Nhap So Hoa Don can in: ", soHD_input, 21, true);

    Nhanvien* nvLapHD = NULL;
    NodeHoaDon* hdTimThay = timHoaDon(soHD_input, nvLapHD);

    if (hdTimThay == NULL) {
        cout << "Loi: Khong tim thay hoa don co so '" << soHD_input << "'." << endl;
        return;
    }
    
    cout << "\n================== HOA DON ==================\n";
    cout << "So Hoa Don: " << hdTimThay->info.SoHD << endl;
    cout << "Ngay Lap  : " << hdTimThay->info.NgayLap.ngay << "/"
        << hdTimThay->info.NgayLap.thang << "/"
        << hdTimThay->info.NgayLap.nam << endl;
    cout << "Loai HD   : " << (hdTimThay->info.Loai == 'N' ? "Phieu Nhap" : "Phieu Xuat") << endl;
    cout << "Nhan Vien  : " << nvLapHD->HO << " " << nvLapHD->TEN << " (Ma: " << nvLapHD->MANV << ")" << endl;
    cout << "-----------------------------------------------\n";
    cout << left << setw(20) << "Ten Vat Tu"
        << right << setw(10) << "So Luong"
        << setw(15) << "Don Gia"
        << setw(15) << "Tri Gia" << endl;
    cout << "-----------------------------------------------------------------\n";

    double tongTriGia = 0.0;
    NodeCT_HoaDon* pCT = hdTimThay->info.dscthd.head;

    while (pCT != NULL) {
        NodeVattu* vtNode = findVattu(dsVatTu.root, pCT->info.MAVT);
        string tenVT = (vtNode != NULL) ? vtNode->info.TENVT : "??? (DA BI XOA)";

        double triGia = (double)pCT->info.SoLuong * pCT->info.DonGia * (1.0 + pCT->info.VAT / 100.0);
        tongTriGia += triGia;

        cout << left << setw(20) << tenVT
            << right << setw(10) << pCT->info.SoLuong
            << setw(15) << fixed << setprecision(2) << pCT->info.DonGia
            << setw(15) << fixed << setprecision(2) << triGia << endl;
        pCT = pCT->next;
    }

    cout << "-----------------------------------------------------------------\n";
    cout << "TONG TRI GIA HOA DON: " << fixed << setprecision(2) << tongTriGia << endl;
    cout << "=================================================================\n";
}

// ----------------------------------------------------------------
// -- CHỨC NĂNG G: THỐNG KÊ HÓA ĐƠN THEO KHOẢNG THỜI GIAN --
// (Không thay đổi)
// ----------------------------------------------------------------
double tinhTongTriGia(ListCT_HoaDon dscthd) {
    double tongTriGia = 0.0;
    NodeCT_HoaDon* pCT = dscthd.head;
    while (pCT != NULL) {
        tongTriGia += (double)pCT->info.SoLuong * pCT->info.DonGia * (1.0 + pCT->info.VAT / 100.0);
        pCT = pCT->next;
    }
    return tongTriGia;
}

int dateToInt(Date d) {
    return d.nam * 10000 + d.thang * 100 + d.ngay;
}

bool isDateInRange(Date check, Date from, Date to) {
    int intCheck = dateToInt(check);
    int intFrom = dateToInt(from);
    int intTo = dateToInt(to);
    return (intCheck >= intFrom && intCheck <= intTo);
}

void xuLyThongKeHoaDon() {
    Date tuNgay, denNgay;
    cout << "\n--- THONG KE HOA DON THEO KHOANG THOI GIAN ---\n";
    cout << "Nhap Thoi Gian Bat Dau (Tu Ngay):\n";
    cout << "Ngay: "; cin >> tuNgay.ngay;
    cout << "Thang: "; cin >> tuNgay.thang;
    cout << "Nam: "; cin >> tuNgay.nam;

    cout << "Nhap Thoi Gian Ket Thuc (Den Ngay):\n";
    cout << "Ngay: "; cin >> denNgay.ngay;
    cout << "Thang: "; cin >> denNgay.thang;
    cout << "Nam: "; cin >> denNgay.nam;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\n\n                      BANG LIET KE CAC HOA DON TRONG KHOANG THOI GIAN\n";
    cout << "                      Tu ngay : "
        << setfill('0') << setw(2) << tuNgay.ngay << "/"
        << setw(2) << tuNgay.thang << "/" << setw(4) << tuNgay.nam;
    cout << "    Den ngay : "
        << setw(2) << denNgay.ngay << "/" << setw(2) << denNgay.thang
        << "/" << setw(4) << denNgay.nam << endl;

    cout << setfill(' ');
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << left << setw(22) << "So HD"
        << setw(15) << "Ngay Lap"
        << setw(10) << "Loai HD"
        << setw(30) << "Ho Ten NV Lap"
        << right << setw(15) << "Tri Gia" << endl;
    cout << "-------------------------------------------------------------------------------------------\n";

    bool timThay = false;

    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        Nhanvien* nv = dsNhanVien.dsnv[i];
        NodeHoaDon* pHD = nv->dshd.head;

        while (pHD != NULL) {
            if (isDateInRange(pHD->info.NgayLap, tuNgay, denNgay)) {
                timThay = true;
                double triGia = tinhTongTriGia(pHD->info.dscthd);

                string ngayLapStr = to_string(pHD->info.NgayLap.ngay) + "/" +
                    to_string(pHD->info.NgayLap.thang) + "/" +
                    to_string(pHD->info.NgayLap.nam);
                string loaiStr = (pHD->info.Loai == 'N') ? "Nhap" : "Xuat";
                string hoTenNV = nv->HO + " " + nv->TEN;

                cout << left << setw(22) << pHD->info.SoHD
                    << setw(15) << ngayLapStr
                    << setw(10) << loaiStr
                    << setw(30) << hoTenNV
                    << right << setw(15) << fixed << setprecision(2) << triGia << endl;
            }
            pHD = pHD->next;
        }
    }

    if (!timThay) {
        cout << "\nKhong tim thay hoa don nao trong khoang thoi gian nay.\n";
    }
    cout << "-------------------------------------------------------------------------------------------\n";
}


// -- HÀM GIẢI PHÓNG BỘ NHỚ --
// (Không thay đổi)
void giaiPhongCayVatTu(NodeVattu*& root) {
    if (root != NULL) {
        giaiPhongCayVatTu(root->left);
        giaiPhongCayVatTu(root->right);
        delete root;
        root = NULL;
    }
}


// ---------------------------------
// -- HÀM MAIN CHÍNH (HOÀN CHỈNH) --
// (Không thay đổi)
// ---------------------------------
int main() {
    initTree(dsVatTu);
    dsNhanVien.soLuong = 0;
    
    int choice = -1;

    do {
        clearScreen();
        cout << "\n============================================\n";
        cout << "    CHUONG TRINH QUAN LY NHAP XUAT VAT TU\n";
        cout << "============================================\n";
        cout << "1. Quan Ly Vat Tu (Chuc nang a)\n";
        cout << "2. In Danh Sach Vat Tu Ton Kho (Chuc nang b)\n";
        cout << "3. Them Nhan Vien (Chuc nang c)\n";
        cout << "4. In Danh Sach Nhan Vien (Chuc nang d)\n";
        cout << "5. Lap Hoa Don Nhap/Xuat (Chuc nang e)\n";
        cout << "6. In Hoa Don (Chuc nang f)\n";
        cout << "7. Thong Ke Hoa Don (Chuc nang g)\n";
        cout << "0. Thoat Chuong Trinh\n";
        cout << "============================================\n";
        cout << "Lua chon cua ban: ";
        
        cin >> choice;
        
        // <<< SỬA >>> Xử lý lỗi nhập sai (không phải số)
        if (cin.fail()) {
            cin.clear(); // Xóa cờ lỗi
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa bộ đệm
            choice = -1; // Đặt lại choice để lặp lại
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa \n nếu nhập đúng
        }

        clearScreen();

        switch (choice) {
        case 1:
            menuQuanLyVatTu();
            break;
        case 2:
            inDanhSachVatTuTonKho_TheoTen();
            break;
        case 3:
            xuLyThemNhanVien();
            break;
        case 4:
            inDanhSachNhanVien();
            break;
        case 5:
            xuLyLapHoaDon();
            break;
        case 6:
            xuLyInHoaDon();
            break;
        case 7:
            xuLyThongKeHoaDon();
            break;
        case 0:
            cout << "Tam biet! Dang don dep bo nho...\n";
            break;
        default:
            cout << "Lua chon khong hop le. Vui long chon lai.\n";
        }

        if (choice != 0) {
            pressEnterToContinue();
        }

    } while (choice != 0);

    // -- GIẢI PHÓNG BỘ NHỚ KHI KẾT THÚC --

    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        NodeHoaDon* pHoaDon = dsNhanVien.dsnv[i]->dshd.head;
        while (pHoaDon != NULL) {
            giaiPhongDS_CTHD(pHoaDon->info.dscthd);
            NodeHoaDon* pDelHD = pHoaDon;
            pHoaDon = pHoaDon->next;
            delete pDelHD;
        }
        delete dsNhanVien.dsnv[i];
    }
    cout << "Da giai phong bo nho Nhan Vien & Hoa Don.\n";

    giaiPhongCayVatTu(dsVatTu.root);
    cout << "Da giai phong bo nho Vat Tu.\n";

    cout << "Don dep hoan tat!\n";
    return 0;
}