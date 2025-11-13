// ====================================================================
// KHỐI 1: KHAI BÁO THƯ VIỆN
// Bao gồm các thư viện chuẩn C++ và thư viện conio.h cho Windows
// ====================================================================
#include <iostream>  // Cho cin, cout
#include <string>    // Cho std::string
#include <cstring>   // Cho strcmp, strncpy
#include <iomanip>   // Cho setw, setprecision (định dạng in)
#include <cstdlib>   // Cho system("cls"), system("clear")
#include <fstream>   // Cho File I/O (ofstream, ifstream)
#include <sstream>   // Cho String Stream (đọc file)
#include <algorithm> // Cho std::transform (chuyển chữ hoa)
#include <cctype>    // Cho std::toupper, isalpha, isdigit (kiểm tra ký tự)
#include <limits>    // Cho numeric_limits (xử lý lỗi cin)

// Thư viện 'conio.h' chỉ dành cho Windows để bắt phím mũi tên
// với hàm _getch()
#include <conio.h> 

using namespace std;

// --- Định nghĩa mã phím (chỉ cho Windows) ---
#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_ENTER 13


// ====================================================================
// KHỐI 2: CÁC HÀM TIỆN ÍCH (UI & CHUNG)
// Các hàm hỗ trợ cho Giao diện người dùng và các phép toán nhỏ
// ====================================================================

// --- Hàm xóa màn hình console (tương thích Windows/Linux) ---
void clearScreen() {
#ifdef _WIN32
    system("cls"); 
#else
    system("clear"); 
#endif
}

// --- Hàm chờ người dùng nhấn Enter ---
void pressEnterToContinue() {
    cout << "\nNhan Enter de tiep tuc...";
    // Xóa bộ đệm \n từ các hàm cin >> ... trước đó (nếu có)
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    // Chờ người dùng nhấn Enter
    cin.get(); 
}

// --- Hàm toán học tìm số lớn nhất ---
int customMax(int a, int b) {
    return (a > b) ? a : b;
}

// ====================================================================
// KHỐI 3: CÁC HÀM NHẬP LIỆU AN TOÀN (VALIDATION)
// Nhóm các hàm dùng để nhận đầu vào từ người dùng một cách an toàn,
// chống crash, chống nhập sai dữ liệu.
// ====================================================================

// --- Hàm xóa dấu cách thừa ở đầu và cuối chuỗi ---
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first) {
        return ""; // Trả về chuỗi rỗng nếu toàn dấu cách
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// --- Hàm chuyển chuỗi sang chữ HOA ---
string toUpper(string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return std::toupper(c); }
    );
    return s;
}

// --- Hàm kiểm tra chuỗi chỉ chứa chữ cái và dấu cách ---
bool isAlphaWithSpaces(const string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        // Phải ép kiểu (unsigned char) để isalpha hoạt động đúng
        if (!isalpha((unsigned char)str[i]) && !isspace((unsigned char)str[i])) {
            return false; // Tìm thấy ký tự không phải chữ/cách
        }
    }
    return true; // Hợp lệ
}

// --- Hàm kiểm tra chuỗi chỉ chứa SỐ ---
bool isAllDigits(const string& str) {
    if (str.empty()) return false; // Không được rỗng
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit((unsigned char)str[i])) {
            return false; // Tìm thấy ký tự không phải số
        }
    }
    return true;
}

// --- Hàm nhập std::string (bắt buộc nhập, không được rỗng) ---
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

// --- Hàm nhập chuỗi (chỉ chấp nhận chữ cái và dấu cách) ---
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
            cout << "Loi: Truong nay chi duoc phep nhap chu cai va dau cach.\n";
            hopLe = false;
        }
    } while (!hopLe);
    return input;
}

// --- Hàm nhập số nguyên (dùng cho nhập Năm, kiểm tra độ dài) ---
int nhapSoNguyen(string prompt, int minLength, int maxLength) {
    string input;
    bool hopLe;
    do {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        hopLe = true;

        if (input.empty()) {
            cout << "Loi: Khong duoc de trong!\n";
            hopLe = false;
        }
        else if (!isAllDigits(input)) {
            cout << "Loi: Chi duoc phep nhap SO (0-9).\n";
            hopLe = false;
        }
        else if (input.length() < (size_t)minLength || input.length() > (size_t)maxLength) {
             cout << "Loi: Do dai khong hop le (phai co tu " << minLength << " den " << maxLength << " chu so).\n";
             hopLe = false;
        }
    } while (!hopLe);
    
    // Chuyển chuỗi an toàn sang số
    return stoi(input);
}

// --- Hàm nhập mảng char[] (cho SoHD, MAVT...) ---
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
            cout << "Loi: Vuot qua do dai toi đa (" << maxLength - 1 << " ky tu). Vui long nhap lai.\n";
            input = ""; // Đặt lại để vòng lặp chạy tiếp
        }
    } while (input.empty());

    strncpy(dest, input.c_str(), maxLength - 1); 
    dest[maxLength - 1] = '\0'; // Đảm bảo luôn kết thúc bằng null
}

// --- Hàm nhập số nguyên dương (dùng cin >>, có kiểm tra cin.fail()) ---
int nhapSoNguyenDuong(string prompt) {
    int number = -1;
    do {
        cout << prompt;
        cin >> number;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Loi: Phai nhap la mot SO nguyen.\n";
            number = -1; // Đặt lại để lặp
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (number <= 0) {
                cout << "Loi: So phai lon hon 0.\n";
            }
        }
    } while (number <= 0);
    return number;
}

// --- Hàm nhập số float không âm (dùng cin >>, có kiểm tra cin.fail()) ---
float nhapSoFloatKhongAm(string prompt) {
    float number = -1.0;
    do {
        cout << prompt;
        cin >> number;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Loi: Phai nhap la mot SO (vi du: 10.5).\n";
            number = -1.0; // Đặt lại để lặp
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (number < 0) {
                cout << "Loi: So phai lon hon hoac bang 0.\n";
            }
        }
    } while (number < 0);
    return number;
}

// ====================================================================
// KHỐI 4: HÀM ĐIỀU KHIỂN MENU MŨI TÊN
// Hàm dùng _getch() để vẽ menu và nhận phím mũi tên/Enter
// ====================================================================
/*
 * Hàm hiển thị menu động
 * Cho phép chọn bằng phím LÊN/XUỐNG và ENTER
 * Dùng MẢNG C-STYLE (const string options[], int numOptions)
 * Trả về CHỈ SỐ (index) 0-based của lựa chọn
 */
int hienThiMenuMuiTen(const string& title, const string options[], int numOptions) {
    int selectedOption = 0; // Lựa chọn hiện tại

    while (true) {
        clearScreen(); 

        cout << title << "\n";
        cout << "============================================\n";
        cout << "(Dung phim Mui Ten LEN/XUONG de di chuyen, Enter de chon)\n\n";

        for (int i = 0; i < numOptions; ++i) {
            if (i == selectedOption) {
                // In ra lựa chọn đang được highlight
                cout << "  >> " << options[i] << " << \n";
            } else {
                cout << "     " << options[i] << "\n";
            }
        }
        cout << "\n============================================\n";

        // Chờ nhận phím
        int key = _getch(); // Đọc 1 phím

        if (key == 224) { // 224 là mã đặc biệt cho phím mũi tên
            key = _getch(); // Đọc thêm 1 lần nữa để lấy mã phím thực sự
            switch (key) {
                case KEY_UP:
                    // Di chuyển lên, có vòng lặp
                    selectedOption = (selectedOption - 1 + numOptions) % numOptions;
                    break;
                case KEY_DOWN:
                    // Di chuyển xuống, có vòng lặp
                    selectedOption = (selectedOption + 1) % numOptions;
                    break;
            }
        } else if (key == KEY_ENTER) {
            // Nhấn Enter
            return selectedOption; // Trả về chỉ số 0-based
        }
    }
}


// ====================================================================
// KHỐI 5: ĐỊNH NGHĨA CẤU TRÚC DỮ LIỆU & BIẾN TOÀN CỤC
// Định nghĩa các struct Vattu, HoaDon, NhanVien và các biến
// toàn cục (dsVatTu, dsNhanVien)
// ====================================================================

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
    Nhanvien() { // Constructor
        MANV = 0;
        dshd.head = NULL;
    }
};

struct ListNhanVien {
    Nhanvien* dsnv[MAX_NHANVIEN];
    int soLuong;
};

// --- Khai báo Biến Toàn Cục ---
TreeVattu dsVatTu;
ListNhanVien dsNhanVien;
const string FILE_VATTU = "danhsachvattu.txt";
const string FILE_NHANVIEN = "danhsachnhanvien.txt";


// ====================================================================
// KHỐI 6: CÁC HÀM XỬ LÝ CÂY AVL (VẬT TƯ)
// Nhóm các hàm cơ bản để thao tác với Cây nhị phân tìm kiếm
// cân bằng (AVL Tree)
// ====================================================================

// --- Khởi tạo cây ---
void initTree(TreeVattu& t) {
    t.root = NULL;
}

// --- Lấy chiều cao của 1 node ---
int getHeight(NodeVattu* node) {
    if (node == NULL) return 0;
    return node->height;
}

// --- Lấy hệ số cân bằng (Balance Factor) ---
int getBalance(NodeVattu* node) {
    if (node == NULL) return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// --- Tạo một node vật tư mới ---
NodeVattu* createNodeVattu(Vattu vt) {
    NodeVattu* p = new NodeVattu;
    p->info = vt;
    p->left = NULL;
    p->right = NULL;
    p->height = 1; // Node lá có chiều cao 1
    return p;
}

// --- Phép xoay Phải (Right Rotate) ---
NodeVattu* rightRotate(NodeVattu* y) {
    NodeVattu* x = y->left;
    NodeVattu* T2 = x->right;
    // Thực hiện xoay
    x->right = y;
    y->left = T2;
    // Cập nhật chiều cao
    y->height = customMax(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = customMax(getHeight(x->left), getHeight(x->right)) + 1;
    return x; // Trả về root mới
}

// --- Phép xoay Trái (Left Rotate) ---
NodeVattu* leftRotate(NodeVattu* x) {
    NodeVattu* y = x->right;
    NodeVattu* T2 = y->left;
    // Thực hiện xoay
    y->left = x;
    x->right = T2;
    // Cập nhật chiều cao
    x->height = customMax(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = customMax(getHeight(y->left), getHeight(y->right)) + 1;
    return y; // Trả về root mới
}

// --- Hàm Thêm (Insert) một vật tư vào cây AVL ---
NodeVattu* insertVattu(NodeVattu* node, Vattu vt) {
    // 1. Thêm như cây BST thông thường
    if (node == NULL) {
        return createNodeVattu(vt);
    }
    if (strcmp(vt.MAVT, node->info.MAVT) < 0)
        node->left = insertVattu(node->left, vt);
    else if (strcmp(vt.MAVT, node->info.MAVT) > 0)
        node->right = insertVattu(node->right, vt);
    else {
        // Trùng mã (xảy ra khi tải file lỗi)
        cout << "Canh bao: Ma vat tu " << vt.MAVT << " da ton tai. Khong them vao cay.\n";
        return node;
    }

    // 2. Cập nhật chiều cao
    node->height = 1 + customMax(getHeight(node->left), getHeight(node->right));

    // 3. Lấy hệ số cân bằng
    int balance = getBalance(node);

    // 4. Cân bằng lại cây (4 trường hợp)
    // Left Left (Lệch trái-trái)
    if (balance > 1 && strcmp(vt.MAVT, node->left->info.MAVT) < 0)
        return rightRotate(node);

    // Right Right (Lệch phải-phải)
    if (balance < -1 && strcmp(vt.MAVT, node->right->info.MAVT) > 0)
        return leftRotate(node);

    // Left Right (Lệch trái-phải)
    if (balance > 1 && strcmp(vt.MAVT, node->left->info.MAVT) > 0) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left (Lệch phải-trái)
    if (balance < -1 && strcmp(vt.MAVT, node->right->info.MAVT) < 0) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    
    return node; // Trả về node (đã cân bằng)
}

// --- Hàm Tìm kiếm (Find) một vật tư trong cây ---
NodeVattu* findVattu(NodeVattu* root, char mavt[]) {
    if (root == NULL || strcmp(root->info.MAVT, mavt) == 0) {
        return root;
    }
    if (strcmp(mavt, root->info.MAVT) < 0) {
        return findVattu(root->left, mavt);
    }
    return findVattu(root->right, mavt);
}

// --- Hàm tìm node nhỏ nhất (trái nhất) ---
NodeVattu* findMinNode(NodeVattu* node) {
    NodeVattu* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// --- Hàm Xóa (Delete) một vật tư khỏi cây AVL ---
NodeVattu* deleteVattu(NodeVattu* root, char mavt[]) {
    // 1. Xóa như cây BST thông thường
    if (root == NULL) return root;

    if (strcmp(mavt, root->info.MAVT) < 0)
        root->left = deleteVattu(root->left, mavt);
    else if (strcmp(mavt, root->info.MAVT) > 0)
        root->right = deleteVattu(root->right, mavt);
    else {
        // Node có 0 hoặc 1 con
        if ((root->left == NULL) || (root->right == NULL)) {
            NodeVattu* temp = root->left ? root->left : root->right;
            if (temp == NULL) { // 0 con
                temp = root;
                root = NULL;
            }
            else { // 1 con
                *root = *temp; // Copy nội dung
            }
            delete temp;
        }
        else { // Node có 2 con
            // Tìm node nhỏ nhất bên cây con phải
            NodeVattu* temp = findMinNode(root->right);
            // Copy info
            root->info = temp->info;
            // Xóa node đó
            root->right = deleteVattu(root->right, temp->info.MAVT);
        }
    }

    if (root == NULL) return root;

    // 2. Cập nhật chiều cao
    root->height = 1 + customMax(getHeight(root->left), getHeight(root->right));

    // 3. Lấy hệ số cân bằng
    int balance = getBalance(root);

    // 4. Cân bằng lại cây (4 trường hợp)
    // Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

// ====================================================================
// KHỐI 7: CHỨC NĂNG QUẢN LÝ VẬT TƯ
// Nhóm các hàm xử lý cho menu "Quan Ly Vat Tu": Thêm, Xóa, Sửa
// ====================================================================

// --- Hàm xử lý logic Thêm Vật Tư Mới ---
void xuLyThemVatTu() {
    Vattu vt;
    cout << "\n-- Them Vat Tu Moi --\n";

    // Validation MAVT (Format: VT + số)
    string inputNum;
    string finalMa;
    
    int maxDigits = 11 - 1 - 2; // "VT" + "so" + \0
    bool hopLe = false;
    
    do {
        cout << "Nhap PHAN SO cua Ma Vat Tu (VD: 001, toi da " << maxDigits << " so): ";
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

    // Nhập các trường còn lại
    vt.TENVT = nhapChuoiChuBatBuoc("Nhap Ten Vat Tu: ");
    vt.DVT = nhapChuoiBatBuoc("Nhap Don Vi Tinh: "); 
    vt.SoLuongTon = (int)nhapSoFloatKhongAm("Nhap So Luong Ton (so nguyen >= 0): ");

    // Thêm vào cây
    dsVatTu.root = insertVattu(dsVatTu.root, vt);
    cout << "Da them vat tu thanh cong!\n";
}

// --- Hàm xử lý logic Xóa Vật Tư ---
void xuLyXoaVatTu() {
    char mavt[11];
    cout << "\n-- Xoa Vat Tu --\n";

    // Validation MAVT (Format: VT + số)
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

    // Kiểm tra xem vật tư có tồn tại không
    NodeVattu* node = findVattu(dsVatTu.root, mavt);
    if (node == NULL) {
        cout << "Loi: Khong tim thay vat tu co ma " << mavt << endl;
        return;
    }
    
    // Kiểm tra xem VT đã được dùng trong hóa đơn chưa
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        NodeHoaDon* pHD = dsNhanVien.dsnv[i]->dshd.head;
        while (pHD != NULL) {
            NodeCT_HoaDon* pCT = pHD->info.dscthd.head;
            while (pCT != NULL) {
                if (strcmp(pCT->info.MAVT, mavt) == 0) {
                    cout << "Loi: Vat tu nay da duoc su dung trong Hoa Don '" << pHD->info.SoHD << "'.\n";
                    cout << "Khong the xoa vat tu de dam bao toan ven du lieu.\n";
                    return; // Không cho xóa
                }
                pCT = pCT->next;
            }
            pHD = pHD->next;
        }
    }

    // Nếu không vướng gì, tiến hành xóa
    dsVatTu.root = deleteVattu(dsVatTu.root, mavt);
    cout << "Da xoa vat tu thanh cong!\n";
}

// --- Hàm xử lý logic Hiệu Chỉnh Vật Tư ---
void xuLyHieuChinhVatTu() {
    char mavt[11];
    cout << "\n-- Hieu Chinh Vat Tu --\n";

    // Validation MAVT (Format: VT + số)
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
            cout << "Loi: Phan so qua dai (toi da " << maxDigits << " so).\n";
        } else {
            finalMa = "VT" + inputNum;
            finalMa = toUpper(finalMa); 
            strncpy(mavt, finalMa.c_str(), 10);
            mavt[10] = '\0';
            hopLe = true;
        }
    } while (!hopLe);

    // Tìm vật tư
    NodeVattu* node = findVattu(dsVatTu.root, mavt);
    if (node == NULL) {
        cout << "Loi: Khong tim thay vat tu co ma " << mavt << endl;
        return;
    }

    cout << "Tim thay vat tu: " << node->info.TENVT << endl;
    
    // Nhập Tên mới (Enter để giữ nguyên)
    cout << "Nhap Ten Vat Tu moi (Enter de giu nguyen - [" << node->info.TENVT << "]): ";
    string tenMoi;
    getline(cin, tenMoi);
    tenMoi = trim(tenMoi); 
    if (!tenMoi.empty()) {
        if (isAlphaWithSpaces(tenMoi)) {
             node->info.TENVT = tenMoi;
        } else {
            cout << "Loi: Ten vat tu moi chi duoc chua chu cai. Ten chua duoc cap nhat.\n";
        }
    }

    // Nhập DVT mới (Enter để giữ nguyên)
    cout << "Nhap Don Vi Tinh moi (Enter de giu nguyen - [" << node->info.DVT << "]): ";
    string dvtMoi;
    getline(cin, dvtMoi);
    dvtMoi = trim(dvtMoi);
    if (!dvtMoi.empty()) {
        node->info.DVT = dvtMoi;
    }

    cout << "Da cap nhat thong tin vat tu!\n";
}

// --- Hàm Menu con cho Quản lý Vật tư (Dùng phím mũi tên) ---
void menuQuanLyVatTu() {
    int choice = -1;

    // Định nghĩa các lựa chọn cho menu (ĐÃ XÓA SỐ)
    const string menuTitle = "\n--- Quan Ly Vat Tu ---";
    const string options[] = {
        "Them Vat Tu",
        "Xoa Vat Tu",
        "Hieu Chinh Vat Tu",
        "Quay lai Menu Chinh"
    };
    const int numOptions = 4;

    do {
        // Gọi hàm menu mũi tên mới
        choice = hienThiMenuMuiTen(menuTitle, options, numOptions);
        
        // choice là chỉ số (index): 0, 1, 2, 3
        
        clearScreen();

        switch (choice) {
            case 0: // Tương ứng "Them Vat Tu"
                xuLyThemVatTu();
                break;
            case 1: // Tương ứng "Xoa Vat Tu"
                xuLyXoaVatTu();
                break;
            case 2: // Tương ứng "Hieu Chinh Vat Tu"
                xuLyHieuChinhVatTu();
                break;
            case 3: // Tương ứng "Quay lai"
                // Không làm gì cả, vòng lặp sẽ thoát
                break;
        }

        if (choice != 3) { // Nếu không phải là "Quay lai"
            cout << "\nNhan Enter de tiep tuc...";
            cin.get(); // Chờ Enter
        }

    } while (choice != 3); // Lặp lại cho đến khi chọn "Quay lai"
}

// ====================================================================
// KHỐI 8: CHỨC NĂNG IN DANH SÁCH VẬT TƯ (Theo Tên)
// Nhóm các hàm để lấy dữ liệu từ Cây AVL, chuyển sang mảng,
// dùng QuickSort để sắp xếp theo Tên, và In ra màn hình.
// ====================================================================

// --- Đệ quy đếm số vật tư trong cây ---
int demSoVatTu(NodeVattu* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + demSoVatTu(root->left) + demSoVatTu(root->right);
}

// --- Đệ quy chuyển con trỏ Node từ Cây sang Mảng ---
void dienVaoMang(NodeVattu* root, NodeVattu** dsTam, int& index) {
    if (root != NULL) {
        dsTam[index++] = root;
        dienVaoMang(root->left, dsTam, index);
        dienVaoMang(root->right, dsTam, index);
    }
}

// --- Hàm swap 2 con trỏ (dùng cho QuickSort) ---
void swapConTro(NodeVattu** a, NodeVattu** b) {
    NodeVattu* t = *a;
    *a = *b;
    *b = t;
}

// --- Hàm Partition (phân hoạch) của QuickSort (sắp xếp theo Tên) ---
int partition(NodeVattu** arr, int low, int high) {
    NodeVattu* pivot = arr[high];
    string tenPivot = pivot->info.TENVT;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        // So sánh Tên
        if (arr[j]->info.TENVT < tenPivot) {
            i++;
            swapConTro(&arr[i], &arr[j]);
        }
    }
    swapConTro(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// --- Hàm QuickSort (sắp xếp theo Tên) ---
void quickSortTheoTen(NodeVattu** arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortTheoTen(arr, low, pi - 1);
        quickSortTheoTen(arr, pi + 1, high);
    }
}

// --- Hàm chính: In Danh Sách Vật Tư Tồn Kho (sắp xếp theo Tên) ---
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

    // 1. Đếm số lượng
    int soLuongVT = demSoVatTu(dsVatTu.root);
    // 2. Tạo mảng tạm
    NodeVattu** dsTam = new NodeVattu * [soLuongVT];
    int index = 0;
    // 3. Chuyển cây sang mảng
    dienVaoMang(dsVatTu.root, dsTam, index);
    // 4. Sắp xếp mảng
    quickSortTheoTen(dsTam, 0, soLuongVT - 1);

    // 5. In mảng đã sắp xếp
    for (int i = 0; i < soLuongVT; i++) {
        Vattu& vt = dsTam[i]->info;
        cout << left
             << setw(12) << vt.MAVT
             << setw(30) << vt.TENVT
             << setw(15) << vt.DVT
             << setw(10) << right << vt.SoLuongTon << endl;
    }

    cout << "------------------------------------------------------------------\n";
    // 6. Xóa mảng tạm
    delete[] dsTam;
}

// ====================================================================
// KHỐI 9: CHỨC NĂNG QUẢN LÝ NHÂN VIÊN
// Nhóm các hàm xử lý cho "Them Nhan Vien" và "In Danh Sach Nhan Vien"
// ====================================================================

// --- Tìm Nv theo Mã (trả về index, -1 nếu không tìm thấy) ---
int timNhanVienTheoMa(int manv) {
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        if (dsNhanVien.dsnv[i]->MANV == manv) {
            return i;
        }
    }
    return -1;
}

// --- Tìm vị trí cần chèn (để giữ mảng sắp xếp theo Tên->Họ) ---
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
    return i; // Chèn vào cuối
}

// --- Hàm xử lý logic Thêm Nhân Viên ---
void xuLyThemNhanVien() {
    cout << "\n-- Them Nhan Vien Moi --\n";

    if (dsNhanVien.soLuong >= MAX_NHANVIEN) {
        cout << "Loi: Danh sach nhan vien da day! Khong The them.\n";
        return;
    }

    Nhanvien* nvMoi = new Nhanvien;

    // 1. Validation Mã Nhân Viên (MANV)
    do {
        cout << "Nhap Ma Nhan Vien (so nguyen > 0): ";
        cin >> nvMoi->MANV;

        if (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            cout << "Loi: Ma Nhan Vien phai la mot SO nguyen.\n";
            nvMoi->MANV = 0; // Đặt lại giá trị 0 để lặp lại
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            
            if (nvMoi->MANV <= 0) {
                cout << "Loi: Ma Nhan Vien phai la so duong (> 0).\n";
            }
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

    nvMoi->dshd.head = NULL;

    // 4. Chèn vào mảng theo thứ tự (Insertion Sort)
    int viTriChen = timViTriChen(nvMoi);

    // Dịch chuyển các phần tử phía sau
    for (int j = dsNhanVien.soLuong; j > viTriChen; j--) {
        dsNhanVien.dsnv[j] = dsNhanVien.dsnv[j - 1];
    }

    dsNhanVien.dsnv[viTriChen] = nvMoi;
    dsNhanVien.soLuong++;

    cout << "Da them nhan vien thanh cong!\n";
}

// --- Hàm In Danh Sách Nhân Viên (đã sắp xếp) ---
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

// ====================================================================
// KHỐI 10: CHỨC NĂNG LẬP HÓA ĐƠN
// Nhóm các hàm xử lý logic cho việc tạo Hóa Đơn và
// Chi Tiết Hóa Đơn. Đây là chức năng phức tạp nhất.
// ====================================================================

// --- Kiểm tra trùng Số Hóa Đơn (duyệt qua tất cả NV) ---
bool kiemTraTrungSoHD(char soHD[]) {
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        ListHoaDon& dshd = dsNhanVien.dsnv[i]->dshd;
        NodeHoaDon* p = dshd.head;
        while (p != NULL) {
            if (strcmp(p->info.SoHD, soHD) == 0) {
                return true; // Trùng
            }
            p = p->next;
        }
    }
    return false; // Không trùng
}

// --- Thêm một Chi Tiết HĐ vào đầu DSLK ---
void themCT_HoaDon(ListCT_HoaDon& dscthd, CT_HoaDon cthd) {
    NodeCT_HoaDon* p = new NodeCT_HoaDon;
    p->info = cthd;
    p->next = dscthd.head;
    dscthd.head = p;
}

// --- In danh sách CT_HĐ tạm thời (khi đang lập) ---
void inDanhSachCT_TamThoi(ListCT_HoaDon dscthd) {
    cout << "\n     --- Chi Tiet Hoa Don Hien Tai ---\n";
    if (dscthd.head == NULL) {
        cout << "     (Chua co vat tu nao)\n";
        return;
    }
    cout << "     " << left << setw(12) << "MAVT" << setw(10) << "SO LUONG"
         << setw(15) << "DON GIA" << "VAT (%)\n";
    NodeCT_HoaDon* p = dscthd.head;
    while (p != NULL) {
        cout << "     " << left << setw(12) << p->info.MAVT
             << setw(10) << p->info.SoLuong
             << setw(15) << p->info.DonGia
             << p->info.VAT << endl;
        p = p->next;
    }
    cout << "     -------------------------------------\n";
}

// --- Xóa một CT_HĐ khỏi danh sách tạm thời (khi đang lập) ---
bool xoaCT_HoaDon_TamThoi(ListCT_HoaDon& dscthd, char mavtXoa[]) {
    NodeCT_HoaDon* pDel = dscthd.head, * pPrev = NULL;

    // Tìm node cần xóa
    while (pDel != NULL && strcmp(pDel->info.MAVT, mavtXoa) != 0) {
        pPrev = pDel;
        pDel = pDel->next;
    }

    if (pDel == NULL) {
        return false; // Không tìm thấy
    }

    // Xóa
    if (pPrev == NULL) { // Nếu là node đầu
        dscthd.head = pDel->next;
    }
    else {
        pPrev->next = pDel->next;
    }

    delete pDel;
    return true;
}

// --- Giải phóng bộ nhớ của 1 danh sách CT_HĐ ---
void giaiPhongDS_CTHD(ListCT_HoaDon& dscthd) {
    NodeCT_HoaDon* p;
    while (dscthd.head != NULL) {
        p = dscthd.head;
        dscthd.head = p->next;
        delete p;
    }
}


// --- Hàm xử lý logic chính: Lập Hóa Đơn (Dùng menu mũi tên) ---
void xuLyLapHoaDon() {
    cout << "\n--- LAP HOA DON MOI ---\n";

    // 1. Chọn Nhân viên
    int manv = nhapSoNguyenDuong("Nhap Ma Nhan Vien lap hoa don: ");
    int indexNV = timNhanVienTheoMa(manv);
    if (indexNV == -1) {
        cout << "Loi: Khong tim thay nhan vien co ma " << manv << endl;
        return;
    }
    Nhanvien* nvLapHD = dsNhanVien.dsnv[indexNV];
    cout << "Nhan vien lap hoa don: " << nvLapHD->HO << " " << nvLapHD->TEN << endl;

    // 2. Nhập thông tin Hóa Đơn (Header)
    NodeHoaDon* pHD = new NodeHoaDon;
    pHD->info.dscthd.head = NULL;
    pHD->next = NULL;

    // Nhập Số HĐ và kiểm tra trùng
    nhapChuoiCharBatBuoc("Nhap So Hoa Don (max 20 ky tu): ", pHD->info.SoHD, 21, true);
    if (kiemTraTrungSoHD(pHD->info.SoHD)) {
        cout << "Loi: So hoa don nay da ton tai trong he thong!\n";
        delete pHD;
        return;
    }

    // Nhập Ngày (Tạm bỏ qua validation)
    cout << "Nhap Ngay Lap (ngay thang nam): ";
    cin >> pHD->info.NgayLap.ngay >> pHD->info.NgayLap.thang >> pHD->info.NgayLap.nam;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Nhập Loại HĐ
    do {
        cout << "Nhap Loai Hoa Don (N: Nhap, X: Xuat): ";
        cin >> pHD->info.Loai;
        if (pHD->info.Loai == 'n') pHD->info.Loai = 'N';
        if (pHD->info.Loai == 'x') pHD->info.Loai = 'X';
    } while (pHD->info.Loai != 'N' && pHD->info.Loai != 'X');
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xóa \n

    char loaiHD = pHD->info.Loai;
    string loaiHDStr = (loaiHD == 'N' ? "== LAP PHIEU NHAP ==" : "== LAP PHIEU XUAT ==");
    
    // 3. Bắt đầu vòng lặp menu Chi Tiết Hóa Đơn
    const string options[] = {
        "Them Vat Tu",
        "Xoa Vat Tu",
        "Luu Hoa Don & Thoat",
        "Huy Hoa Don"
    };
    const int numOptions = 4;
    
    int menuChoice = -1;

    do {
        clearScreen();
        cout << loaiHDStr << endl;
        cout << "So HD: " << pHD->info.SoHD << " | NV: " << nvLapHD->HO << " " << nvLapHD->TEN << endl;
        
        inDanhSachCT_TamThoi(pHD->info.dscthd);

        // Tạo title động cho menu
        string menuTitle = "\n-- Menu Lap Chi Tiet --";
        menuChoice = hienThiMenuMuiTen(menuTitle, options, numOptions);
        
        // menuChoice: 0=Thêm, 1=Xóa, 2=Lưu, 3=Hủy

        // --- Xử lý Thêm Vật Tư (case 0) ---
        if (menuChoice == 0) { 
            CT_HoaDon cthd;

            // Nhập MAVT theo format mới
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

            // Kiểm tra tồn kho
            NodeVattu* vtNode = findVattu(dsVatTu.root, cthd.MAVT);
            if (vtNode == NULL) {
                cout << "Loi: Khong tim thay vat tu nay!\n";
            }
            else {
                cout << "Vat tu: " << vtNode->info.TENVT << " | Ton kho hien tai: " << vtNode->info.SoLuongTon << endl;

                cthd.SoLuong = nhapSoNguyenDuong("Nhap So Luong: ");

                // Kiểm tra số lượng tồn khi Xuất
                if (loaiHD == 'X' && cthd.SoLuong > vtNode->info.SoLuongTon) {
                    cout << "Loi: So luong xuat vuot qua so luong ton!\n";
                    cout << "So luong ton hien co trong kho: " << vtNode->info.SoLuongTon << endl;
                }
                else {
                    cthd.DonGia = nhapSoFloatKhongAm("Nhap Don Gia: ");
                    cthd.VAT = nhapSoFloatKhongAm("Nhap %VAT: ");

                    themCT_HoaDon(pHD->info.dscthd, cthd);
                    cout << "Da them vat tu vao hoa don.\n";
                }
            }
            cout << "\nNhan Enter de tiep tuc..."; cin.get(); // Dừng

        } 
        // --- Xử lý Xóa Vật Tư (case 1) ---
        else if (menuChoice == 1) { 
            char mavtXoa[11];
            
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

            if (xoaCT_HoaDon_TamThoi(pHD->info.dscthd, mavtXoa)) {
                cout << "Da xoa vat tu khoi hoa don.\n";
            }
            else {
                cout << "Loi: Khong tim thay vat tu nay trong hoa don.\n";
            }
            cout << "\nNhan Enter de tiep tuc..."; cin.get(); // Dừng

        } 
        // --- Xử lý Lưu Hóa Đơn (case 2) ---
        else if (menuChoice == 2) { 
            if (pHD->info.dscthd.head == NULL) {
                cout << "Hoa don rong, khong the luu. Chuyen sang Huy Hoa Don.\n";
                cout << "\nNhan Enter de tiep tuc..."; cin.get(); // Dừng
                menuChoice = 3; // Ép thành lựa chọn "Hủy"
            }
            else {
                // Cập nhật số lượng tồn kho
                cout << "Bat dau cap nhat so luong ton kho...\n";
                NodeCT_HoaDon* pCT = pHD->info.dscthd.head;
                while (pCT != NULL) {
                    NodeVattu* vtNode = findVattu(dsVatTu.root, pCT->info.MAVT);
                    if (vtNode != NULL) { // Kiểm tra nếu VT còn tồn tại
                        if (loaiHD == 'N') {
                            vtNode->info.SoLuongTon += pCT->info.SoLuong;
                        }
                        else { // 'X'
                            vtNode->info.SoLuongTon -= pCT->info.SoLuong;
                        }
                    }
                    pCT = pCT->next;
                }
                cout << "Da cap nhat so luong ton kho thanh cong!\n";

                // Thêm Hóa đơn vào đầu DSLK của Nhân Viên
                pHD->next = nvLapHD->dshd.head;
                nvLapHD->dshd.head = pHD;

                cout << "==> DA LUU HOA DON THANH CONG! <==\n";
            }
        }

        // --- Xử lý Hủy Hóa Đơn (case 3) ---
        if (menuChoice == 3) { 
            giaiPhongDS_CTHD(pHD->info.dscthd); // Giải phóng list CT_HĐ
            delete pHD; // Xóa HĐ
            cout << "Da huy hoa don. Khong co gi duoc luu.\n";
        }

    } while (menuChoice != 2 && menuChoice != 3); // Thoát khi Lưu (2) hoặc Hủy (3)
}

// ====================================================================
// KHỐI 11: CHỨC NĂNG IN HÓA ĐƠN (THEO SỐ HĐ)
// Nhóm các hàm để tìm và in chi tiết 1 hóa đơn cụ thể
// ====================================================================

// --- Tìm 1 Hóa Đơn (bất kỳ) theo Số HĐ ---
NodeHoaDon* timHoaDon(char soHD[], Nhanvien*& nvTimThay) {
    // Duyệt qua tất cả nhân viên
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        ListHoaDon& dshd = dsNhanVien.dsnv[i]->dshd;
        NodeHoaDon* p = dshd.head;
        // Duyệt qua tất cả HĐ của NV đó
        while (p != NULL) {
            if (strcmp(p->info.SoHD, soHD) == 0) {
                nvTimThay = dsNhanVien.dsnv[i]; // Lưu lại NV đã lập HĐ
                return p; // Trả về HĐ
            }
            p = p->next;
        }
    }
    nvTimThay = NULL;
    return NULL;
}

// --- Hàm xử lý logic In Hóa Đơn ---
void xuLyInHoaDon() {
    char soHD_input[21];
    cout << "\n--- IN HOA DON ---\n";
    
    // Nhập Số HĐ cần in
    nhapChuoiCharBatBuoc("Nhap So Hoa Don can in: ", soHD_input, 21, true);

    Nhanvien* nvLapHD = NULL;
    NodeHoaDon* hdTimThay = timHoaDon(soHD_input, nvLapHD);

    if (hdTimThay == NULL) {
        cout << "Loi: Khong tim thay hoa don co so '" << soHD_input << "'." << endl;
        return;
    }
    
    // Bắt đầu in
    cout << "\n================== HOA DON ==================\n";
    cout << "So Hoa Don: " << hdTimThay->info.SoHD << endl;
    cout << "Ngay Lap  : " << hdTimThay->info.NgayLap.ngay << "/"
         << hdTimThay->info.NgayLap.thang << "/"
         << hdTimThay->info.NgayLap.nam << endl;
    cout << "Loai HD   : " << (hdTimThay->info.Loai == 'N' ? "Phieu Nhap" : "Phieu Xuat") << endl;
    cout << "Nhan Vien  : " << nvLapHD->HO << " " << nvLapHD->TEN << " (Ma: " << nvLapHD->MANV << ")" << endl;
    cout << "-----------------------------------------------\n";
    cout << left << setw(20) << "Ten Vat Tu"
         << right << setw(10) << "So Luong"
         << setw(15) << "Don Gia"
         << setw(15) << "Tri Gia" << endl;
    cout << "-----------------------------------------------------------------\n";

    double tongTriGia = 0.0;
    NodeCT_HoaDon* pCT = hdTimThay->info.dscthd.head;

    cout << fixed << setprecision(2); // Cài đặt định dạng số float

    while (pCT != NULL) {
        // Tìm tên vật tư
        NodeVattu* vtNode = findVattu(dsVatTu.root, pCT->info.MAVT);
        string tenVT = (vtNode != NULL) ? vtNode->info.TENVT : "??? (DA BI XOA)";

        // Tính Trị Giá = SL * ĐG * (1 + VAT/100)
        double triGia = (double)pCT->info.SoLuong * pCT->info.DonGia * (1.0 + pCT->info.VAT / 100.0);
        tongTriGia += triGia;

        cout << left << setw(20) << tenVT
             << right << setw(10) << pCT->info.SoLuong
             << setw(15) << pCT->info.DonGia
             << setw(15) << triGia << endl;
        pCT = pCT->next;
    }

    cout << "-----------------------------------------------------------------\n";
    cout << "TONG TRI GIA HOA DON: " << tongTriGia << endl;
    cout << "=================================================================\n";
    
    cout.unsetf(ios_base::floatfield); // Hủy cài đặt
}

// ====================================================================
// KHỐI 12: CHỨC NĂNG THỐNG KÊ (THEO NGÀY)
// Nhóm các hàm để liệt kê hóa đơn trong 1 khoảng thời gian
// ====================================================================

// --- Tính tổng trị giá của 1 Hóa Đơn ---
double tinhTongTriGia(ListCT_HoaDon dscthd) {
    double tongTriGia = 0.0;
    NodeCT_HoaDon* pCT = dscthd.head;
    while (pCT != NULL) {
        tongTriGia += (double)pCT->info.SoLuong * pCT->info.DonGia * (1.0 + pCT->info.VAT / 100.0);
        pCT = pCT->next;
    }
    return tongTriGia;
}

// --- Chuyển Date sang int (để so sánh) vd: 20241030 ---
int dateToInt(Date d) {
    return d.nam * 10000 + d.thang * 100 + d.ngay;
}

// --- Kiểm tra 1 ngày có nằm trong khoảng [from, to] hay không ---
bool isDateInRange(Date check, Date from, Date to) {
    int intCheck = dateToInt(check);
    int intFrom = dateToInt(from);
    int intTo = dateToInt(to);
    return (intCheck >= intFrom && intCheck <= intTo);
}

// --- Hàm xử lý logic Thống Kê Hóa Đơn ---
void xuLyThongKeHoaDon() {
    Date tuNgay, denNgay;
    cout << "\n--- THONG KE HOA DON THEO KHOANG THOI GIAN ---\n";
    
    // Nhập ngày (Tạm bỏ qua validation)
    cout << "Nhap Thoi Gian Bat Dau (Tu Ngay):\n";
    cout << "Ngay: "; cin >> tuNgay.ngay;
    cout << "Thang: "; cin >> tuNgay.thang;
    cout << "Nam: "; cin >> tuNgay.nam;

    cout << "Nhap Thoi Gian Ket Thuc (Den Ngay):\n";
    cout << "Ngay: "; cin >> denNgay.ngay;
    cout << "Thang: "; cin >> denNgay.thang;
    cout << "Nam: "; cin >> denNgay.nam;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // In tiêu đề bảng
    cout << "\n\n                       BANG LIET KE CAC HOA DON TRONG KHOANG THOI GIAN\n";
    cout << "                     Tu ngay : "
         << setfill('0') << setw(2) << tuNgay.ngay << "/"
         << setw(2) << tuNgay.thang << "/" << setw(4) << tuNgay.nam;
    cout << "       Den ngay : "
         << setw(2) << denNgay.ngay << "/" << setw(2) << denNgay.thang
         << "/" << setw(4) << denNgay.nam << endl;

    cout << setfill(' '); // Trả lại ký tự fill
    cout << "-------------------------------------------------------------------------------------------\n";
    cout << left << setw(22) << "So HD"
         << setw(15) << "Ngay Lap"
         << setw(10) << "Loai HD"
         << setw(30) << "Ho Ten NV Lap"
         << right << setw(15) << "Tri Gia" << endl;
    cout << "-------------------------------------------------------------------------------------------\n";

    bool timThay = false;
    cout << fixed << setprecision(2); // Cài đặt float

    // Duyệt qua tất cả NV
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        Nhanvien* nv = dsNhanVien.dsnv[i];
        NodeHoaDon* pHD = nv->dshd.head;
        // Duyệt qua tất cả HĐ của NV
        while (pHD != NULL) {
            // Kiểm tra ngày
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
                     << right << setw(15) << triGia << endl;
            }
            pHD = pHD->next;
        }
    }

    if (!timThay) {
        cout << "\nKhong tim thay hoa don nao trong khoang thoi gian nay.\n";
    }
    cout << "-------------------------------------------------------------------------------------------\n";
    cout.unsetf(ios_base::floatfield); // Hủy cài đặt
}

// ====================================================================
// KHỐI 13: CHỨC NĂNG THỐNG KÊ DOANH THU (THEO NĂM)
// Liệt kê doanh thu (chỉ HĐ Xuất) của 12 tháng trong năm
// ====================================================================
void xuLyThongKeDoanhThuTheoNam() {
    cout << "\n--- THONG KE DOANH THU (CHI TINH HOA DON XUAT) ---\n";
    
    // Yêu cầu: "chỉ được nhập 4 số ko được nhập chữ"
    int namThongKe = nhapSoNguyen("Nhap Nam can thong ke (VD: 2024): ", 4, 4);

    // Khởi tạo mảng 12 tháng, doanh thu = 0
    double doanhThuThang[12] = { 0.0 };

    // Bắt đầu quét
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        Nhanvien* nv = dsNhanVien.dsnv[i];
        NodeHoaDon* pHD = nv->dshd.head;

        while (pHD != NULL) {
            // Chỉ tính hóa đơn XUẤT (X) và đúng NĂM
            if (pHD->info.Loai == 'X' && pHD->info.NgayLap.nam == namThongKe) {
                int thang = pHD->info.NgayLap.thang; // Lấy tháng (1-12)
                if (thang >= 1 && thang <= 12) {
                    double triGia = tinhTongTriGia(pHD->info.dscthd);
                    doanhThuThang[thang - 1] += triGia; // -1 vì mảng bắt đầu từ 0
                }
            }
            pHD = pHD->next;
        }
    }

    // In bảng
    cout << "\n\n           BANG THONG KE DOANH THU NAM " << namThongKe << endl;
    cout << "---------------------------------------------\n";
    cout << left << setw(20) << "THANG" << right << setw(20) << "DOANH THU" << endl;
    cout << "---------------------------------------------\n";

    cout << fixed << setprecision(2); // Cài đặt
    double tongDoanhThu = 0.0;

    for (int i = 0; i < 12; i++) {
        cout << left << setw(20) << ("Thang " + to_string(i + 1))
             << right << setw(20) << doanhThuThang[i] << endl;
        tongDoanhThu += doanhThuThang[i];
    }
    
    cout << "---------------------------------------------\n";
    cout << left << setw(20) << "TONG CONG" 
         << right << setw(20) << tongDoanhThu << endl;
    cout << "---------------------------------------------\n";
    cout.unsetf(ios_base::floatfield); // Hủy cài đặt
}

// ====================================================================
// KHỐI 14: CHỨC NĂNG LƯU VÀ TẢI FILE
// Nhóm các hàm để đọc/ghi toàn bộ dữ liệu ra file text
// ====================================================================

// --- Hàm đệ quy LƯU Cây Vật Tư ---
void luuNodeVatTu(ofstream& file, NodeVattu* root) {
    if (root != NULL) {
        // Ghi thông tin của node hiện tại (Dùng | làm dấu phân cách)
        file << root->info.MAVT << "|" 
             << root->info.TENVT << "|" 
             << root->info.DVT << "|" 
             << root->info.SoLuongTon << "\n";
        
        // Đệ quy sang con trái và con phải
        luuNodeVatTu(file, root->left);
        luuNodeVatTu(file, root->right);
    }
}

// --- Hàm LƯU Toàn Bộ Dữ Liệu ---
void luuDuLieu() {
    cout << "Bat dau luu du lieu...\n";
    
    // --- 1. Lưu Vật Tư ---
    ofstream fileVT(FILE_VATTU);
    if (!fileVT.is_open()) {
        cout << "Loi: Khong a the mo file " << FILE_VATTU << " de ghi!\n";
    } else {
        luuNodeVatTu(fileVT, dsVatTu.root);
        fileVT.close();
        cout << "Da luu danh sach Vat Tu vao file " << FILE_VATTU << "\n";
    }

    // --- 2. Lưu Nhân Viên (và Hóa Đơn) ---
    ofstream fileNV(FILE_NHANVIEN);
    if (!fileNV.is_open()) {
        cout << "Loi: Khong a the mo file " << FILE_NHANVIEN << " de ghi!\n";
    } else {
        // Dòng đầu tiên: lưu số lượng nhân viên
        fileNV << dsNhanVien.soLuong << "\n";

        for (int i = 0; i < dsNhanVien.soLuong; i++) {
            Nhanvien* nv = dsNhanVien.dsnv[i];
            
            // Lưu thông tin cơ bản của NV
            fileNV << nv->MANV << "|" << nv->HO << "|" << nv->TEN << "|" << nv->GIOITINH << "\n";

            // Đếm số Hóa Đơn của NV này
            int soHD = 0;
            NodeHoaDon* pHD = nv->dshd.head;
            while (pHD != NULL) {
                soHD++;
                pHD = pHD->next;
            }
            fileNV << soHD << "\n"; // Lưu số lượng HD

            // Lặp lại để lưu từng HD
            pHD = nv->dshd.head;
            while (pHD != NULL) {
                // Lưu thông tin HD
                fileNV << pHD->info.SoHD << "|" 
                       << pHD->info.NgayLap.ngay << "|" << pHD->info.NgayLap.thang << "|" << pHD->info.NgayLap.nam << "|" 
                       << pHD->info.Loai << "\n";
                
                // Đếm số CT_HD
                int soCTHD = 0;
                NodeCT_HoaDon* pCT = pHD->info.dscthd.head;
                while (pCT != NULL) {
                    soCTHD++;
                    pCT = pCT->next;
                }
                fileNV << soCTHD << "\n"; // Lưu số lượng CT_HD
                
                // Lặp lại để lưu từng CT_HD
                pCT = pHD->info.dscthd.head;
                while (pCT != NULL) {
                    fileNV << pCT->info.MAVT << "|" 
                           << pCT->info.SoLuong << "|" << pCT->info.DonGia << "|" << pCT->info.VAT << "\n";
                    pCT = pCT->next;
                }
                pHD = pHD->next;
            }
        }
        fileNV.close();
        cout << "Da luu danh sach Nhan Vien & Hoa Don vao file " << FILE_NHANVIEN << "\n";
    }
    cout << "Luu du lieu hoan tat!\n";
}

// --- Hàm TẢI Toàn Bộ Dữ Liệu ---
void taiDuLieu() {
    string line, field;
    cout << "Bat dau tai du lieu...\n";

    // --- 1. Tải Vật Tư ---
    ifstream fileVT(FILE_VATTU);
    if (!fileVT.is_open()) {
        cout << "Thong bao: Khong tim thay file " << FILE_VATTU << ". Bat dau voi danh sach vat tu rong.\n";
    } else {
        int countVT = 0;
        while (getline(fileVT, line)) {
            if(line.empty()) continue; // Bỏ qua dòng trống

            Vattu vt;
            stringstream ss(line);

            // Đọc MAVT (char[])
            getline(ss, field, '|');
            strncpy(vt.MAVT, field.c_str(), 10);
            vt.MAVT[10] = '\0';

            // Đọc TENVT (string)
            getline(ss, field, '|');
            vt.TENVT = field;

            // Đọc DVT (string)
            getline(ss, field, '|');
            vt.DVT = field;

            // Đọc SoLuongTon (int)
            getline(ss, field, '|');
            vt.SoLuongTon = stoi(field);

            // Thêm vào cây
            dsVatTu.root = insertVattu(dsVatTu.root, vt);
            countVT++;
        }
        fileVT.close();
        cout << "Da tai " << countVT << " vat tu tu file " << FILE_VATTU << "\n";
    }

    // --- 2. Tải Nhân Viên (và Hóa Đơn) ---
    ifstream fileNV(FILE_NHANVIEN);
    if (!fileNV.is_open()) {
        cout << "Thong bao: Khong tim thay file " << FILE_NHANVIEN << ". Bat dau voi danh sach nhan vien rong.\n";
    } else {
        // Đọc dòng đầu tiên: số lượng nhân viên
        getline(fileNV, line);
        if(line.empty()) {
             cout << "File " << FILE_NHANVIEN << " rong. Bat dau voi danh sach nhan vien rong.\n";
             fileNV.close();
             return;
        }
        
        dsNhanVien.soLuong = stoi(line);

        for (int i = 0; i < dsNhanVien.soLuong; i++) {
            Nhanvien* nvMoi = new Nhanvien;

            // Đọc thông tin NV
            getline(fileNV, line);
            stringstream ssNV(line);
            getline(ssNV, field, '|'); nvMoi->MANV = stoi(field);
            getline(ssNV, field, '|'); nvMoi->HO = field;
            getline(ssNV, field, '|'); nvMoi->TEN = field;
            getline(ssNV, field, '|'); nvMoi->GIOITINH = field;
            nvMoi->dshd.head = NULL; // Khởi tạo

            // Đọc số lượng HD
            getline(fileNV, line);
            int soHD = stoi(line);
            
            NodeHoaDon* tailHD = NULL; // Dùng tail để giữ đúng thứ tự

            for(int j=0; j < soHD; j++) {
                NodeHoaDon* pHD = new NodeHoaDon;
                pHD->next = NULL;

                // Đọc thông tin HD
                getline(fileNV, line);
                stringstream ssHD(line);
                getline(ssHD, field, '|'); strncpy(pHD->info.SoHD, field.c_str(), 20); pHD->info.SoHD[20] = '\0';
                getline(ssHD, field, '|'); pHD->info.NgayLap.ngay = stoi(field);
                getline(ssHD, field, '|'); pHD->info.NgayLap.thang = stoi(field);
                getline(ssHD, field, '|'); pHD->info.NgayLap.nam = stoi(field);
                getline(ssHD, field, '|'); pHD->info.Loai = field[0];
                pHD->info.dscthd.head = NULL; // Khởi tạo

                // Đọc số lượng CTHD
                getline(fileNV, line);
                int soCTHD = stoi(line);
                
                NodeCT_HoaDon* tailCT = NULL; // Dùng tail

                for (int k = 0; k < soCTHD; k++) {
                    NodeCT_HoaDon* pCT = new NodeCT_HoaDon;
                    pCT->next = NULL;

                    // Đọc thông tin CTHD
                    getline(fileNV, line);
                    stringstream ssCT(line);
                    getline(ssCT, field, '|'); strncpy(pCT->info.MAVT, field.c_str(), 10); pCT->info.MAVT[10] = '\0';
                    getline(ssCT, field, '|'); pCT->info.SoLuong = stoi(field);
                    getline(ssCT, field, '|'); pCT->info.DonGia = stof(field);
                    getline(ssCT, field, '|'); pCT->info.VAT = stof(field);

                    // Thêm vào cuối CTHD
                    if(tailCT == NULL) {
                        pHD->info.dscthd.head = pCT;
                    } else {
                        tailCT->next = pCT;
                    }
                    tailCT = pCT;
                }
                
                // Thêm vào cuối DSHD
                 if(tailHD == NULL) {
                    nvMoi->dshd.head = pHD;
                } else {
                    tailHD->next = pHD;
                }
                tailHD = pHD;
            }
            // Thêm NV vào mảng (đã được sắp xếp khi lưu)
            dsNhanVien.dsnv[i] = nvMoi;
        }
        fileNV.close();
        cout << "Da tai " << dsNhanVien.soLuong << " nhan vien tu file " << FILE_NHANVIEN << "\n";
    }
    
    cout << "Tai du lieu hoan tat!\n";
    // Dừng lại để xem kết quả tải
    cout << "\nNhan Enter de bat dau...";
    cin.get();
}

// ====================================================================
// KHỐI 15: HÀM GIẢI PHÓNG BỘ NHỚ
// Các hàm dùng để dọn dẹp bộ nhớ (delete) khi thoát
// ====================================================================

// --- Giải phóng Cây Vật Tư (Đệ quy) ---
void giaiPhongCayVatTu(NodeVattu*& root) {
    if (root != NULL) {
        giaiPhongCayVatTu(root->left);
        giaiPhongCayVatTu(root->right);
        delete root;
        root = NULL;
    }
}

// (Hàm giải phóng Hóa Đơn và CT_HĐ được đặt trong main)

// ====================================================================
// KHỐI 16: HÀM MAIN (HÀM CHÍNH)
// Điểm bắt đầu của chương trình, chứa vòng lặp menu chính
// ====================================================================
int main() {
    // 1. Khởi tạo
    initTree(dsVatTu);
    dsNhanVien.soLuong = 0;
    
    // 2. Tải dữ liệu
    clearScreen();
    taiDuLieu(); // Đã bao gồm "Nhan Enter de bat dau..."
    
    // 3. Định nghĩa Menu Chính (ĐÃ XÓA SỐ)
    const string menuTitle = "\n============================================\n"
                             "     CHUONG TRINH QUAN LY NHAP XUAT VAT TU\n";
    
    const string mainOptions[] = {
        "Quan Ly Vat Tu",
        "In Danh Sach Vat Tu Ton Kho",
        "Them Nhan Vien",
        "In Danh Sach Nhan Vien",
        "Lap Hoa Don Nhap/Xuat",
        "In Hoa Don",
        "Thong Ke Hoa Don (Theo Ngay)",
        "Thong Ke Doanh Thu (Theo Nam)",
        "Luu Du Lieu Vao File",
        "Luu & Thoat Chuong Trinh"
    };
    const int numMainOptions = 10; // Có 10 lựa chọn
    
    int choice = -1; // Lưu chỉ số (index) của lựa chọn

    // 4. Vòng lặp Menu Chính
    do {
        // Gọi hàm menu mũi tên
        choice = hienThiMenuMuiTen(menuTitle, mainOptions, numMainOptions);
        
        // choice là chỉ số (index) 0-based
        // 0 = "Quan Ly Vat Tu"
        // 1 = "In Danh Sach..."
        // ...
        // 9 = "Luu & Thoat"

        clearScreen(); // Xóa màn hình menu

        // Xử lý lựa chọn
        switch (choice) {
            case 0: // Tương ứng "Quan Ly Vat Tu"
                menuQuanLyVatTu();
                break;
            case 1: // Tương ứng "In Danh Sach Vat Tu"
                inDanhSachVatTuTonKho_TheoTen();
                break;
            case 2: // Tương ứng "Them Nhan Vien"
                xuLyThemNhanVien();
                break;
            case 3: // Tương ứng "In Danh Sach Nhan Vien"
                inDanhSachNhanVien();
                break;
            case 4: // Tương ứng "Lap Hoa Don"
                xuLyLapHoaDon();
                break;
            case 5: // Tương ứng "In Hoa Don"
                xuLyInHoaDon();
                break;
            case 6: // Tương ứng "Thong Ke Hoa Don"
                xuLyThongKeHoaDon();
                break;
            case 7: // Tương ứng "Thong Ke Doanh Thu"
                xuLyThongKeDoanhThuTheoNam();
                break;
            case 8: // Tương ứng "Luu Du Lieu"
                luuDuLieu();
                break;
            case 9: // Tương ứng "Luu & Thoat"
                cout << "Tam biet! Dang luu du lieu & don dep bo nho...\n";
                luuDuLieu(); // Tự động lưu khi thoát
                break;
        }

        if (choice != 9) { // Nếu không phải là "Thoát"
            // Tạm dừng màn hình
            cout << "\nNhan Enter de quay lai Menu Chinh...";
            cin.get(); // Chờ Enter
        }

    } while (choice != 9); // Lặp lại cho đến khi chọn "Thoát" (index 9)

    // 5. Dọn dẹp bộ nhớ trước khi thoát
    cout << "Dang giai phong bo nho...\n";
    for (int i = 0; i < dsNhanVien.soLuong; i++) {
        // Giải phóng Hóa Đơn
        NodeHoaDon* pHoaDon = dsNhanVien.dsnv[i]->dshd.head;
        while (pHoaDon != NULL) {
            // Giải phóng CT_HĐ
            giaiPhongDS_CTHD(pHoaDon->info.dscthd);
            
            NodeHoaDon* pDelHD = pHoaDon;
            pHoaDon = pHoaDon->next;
            delete pDelHD; // Xóa node HĐ
        }
        delete dsNhanVien.dsnv[i]; // Xóa đối tượng NV
    }
    cout << "Da giai phong bo nho Nhan Vien & Hoa Don.\n";

    // Giải phóng Cây Vật Tư
    giaiPhongCayVatTu(dsVatTu.root);
    cout << "Da giai phong bo nho Vat Tu.\n";

    cout << "Don dep hoan tat!\n";
    return 0; // Kết thúc chương trình
}
// ====================================================================
// KẾT THÚC CHƯƠNG TRÌNH
// ====================================================================