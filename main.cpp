// main.cpp - Chuong trinh chinh (Phan 1: Khai bao & khoi tao)
#include "structs.h"
#include "vattu.h"
#include "nhanvien.h"
#include "hoadon.h"
#include "io.h"
#include "ui.h"
#include "undo.h"
#include "report.h"
#include <conio.h>
#include <iostream>
#include <cstring>
#include <cstdlib>  // Cho system()
using namespace std;

// =========================
// BIEN TOAN CUC
// =========================

PTRVT g_root_vattu = NULL;
DS_NHANVIEN g_dsnv;
UndoStack g_undoStack;

const char* DATA_FILE = "quanly_vattu.txt";

// =========================
// MENU CHINH
// =========================

void MenuVatTu();
void MenuNhanVien();
void MenuHoaDon();
void MenuThongKe();

// =========================
// CHUC NANG VAT TU
// =========================

void ThemVatTu() {
    clrscr();
    drawLogo();
    drawHotkeys();
    
    gotoxy(50, 3);
    cout << "=== THEM VAT TU ===" << endl;
    
    char ma[11], ten[51], dvt[11];
    int soluong;
    
    gotoxy(50, 5);
    cout << "Ma vat tu: ";
    inputString(ma, 11, 61, 5);
    
    gotoxy(50, 6);
    cout << "Ten vat tu: ";
    inputString(ten, 51, 62, 6);
    
    gotoxy(50, 7);
    cout << "Don vi tinh: ";
    inputString(dvt, 11, 63, 7);
    
    gotoxy(50, 8);
    cout << "So luong ton: ";
    soluong = inputInt(64, 8);
    
    // Kiem tra validate
    if (strlen(ma) == 0 || strlen(ten) == 0 || strlen(dvt) == 0) {
        showError("Cac truong khong duoc rong!");
        return;
    }
    
    if (soluong < 0) {
        showError("So luong phai >= 0!");
        return;
    }
    
    // Them vao cay
    bool success;
    g_root_vattu = InsertVatTu(g_root_vattu, ma, ten, dvt, soluong, success);
    
    if (success) {
        // Push undo
        PushUndo(g_undoStack, CreateUndoAddVT(ma));
        showSuccess("Da them vat tu thanh cong!");
    } else {
        showError("Ma vat tu da ton tai!");
    }
}

void SuaVatTu() {
    clrscr();
    drawLogo();
    drawHotkeys();
    
    gotoxy(50, 3);
    cout << "=== SUA VAT TU ===" << endl;
    
    char ma[11], ten[51], dvt[11];
    
    gotoxy(50, 5);
    cout << "Ma vat tu can sua: ";
    inputString(ma, 11, 69, 5);
    
    // Tim vat tu
    PTRVT vt = SearchVatTu(g_root_vattu, ma);
    if (vt == NULL) {
        showError("Khong tim thay vat tu!");
        return;
    }
    
    // Hien thi thong tin hien tai
    gotoxy(50, 7);
    cout << "Ten hien tai: " << vt->TENVT;
    gotoxy(50, 8);
    cout << "DVT hien tai: " << vt->DVT;
    
    // Nhap thong tin moi
    gotoxy(50, 10);
    cout << "Ten moi: ";
    inputString(ten, 51, 59, 10);
    
    gotoxy(50, 11);
    cout << "DVT moi: ";
    inputString(dvt, 11, 59, 11);
    
    // Backup truoc khi sua (cho undo)
    VATTU backup = *vt;
    PushUndo(g_undoStack, CreateUndoEditVT(backup));
    
    // Cap nhat
    if (strlen(ten) > 0) strcpy(vt->TENVT, ten);
    if (strlen(dvt) > 0) strcpy(vt->DVT, dvt);
    
    showSuccess("Da cap nhat vat tu thanh cong!");
}

void XoaVatTu() {
    clrscr();
    drawLogo();
    drawHotkeys();
    
    gotoxy(50, 3);
    cout << "=== XOA VAT TU ===" << endl;
    
    char ma[11];
    
    gotoxy(50, 5);
    cout << "Ma vat tu can xoa: ";
    inputString(ma, 11, 69, 5);
    
    // Xac nhan
    gotoxy(50, 7);
    cout << "Ban co chac chan muon xoa? (Y/N): ";
    char confirm = _getch();
    
    if (confirm != 'Y' && confirm != 'y') {
        showMessage("Da huy xoa.");
        return;
    }
    
    // Xoa
    bool success;
    VATTU backup;
    g_root_vattu = DeleteVatTu(g_root_vattu, ma, success, backup);
    
    if (success) {
        // Push undo
        PushUndo(g_undoStack, CreateUndoDelVT(backup));
        showSuccess("Da xoa vat tu thanh cong!");
    } else {
        showError("Khong tim thay vat tu!");
    }
}

void InDanhSachVatTu() {
    clrscr();
    drawLogo();
    
    gotoxy(5, 12);
    cout << "=== DANH SACH VAT TU (THEO TEN TANG DAN) ===" << endl;
    
    gotoxy(5, 13);
    cout << "Ma VT | Ten VT | DVT | So Luong Ton" << endl;
    cout << "------------------------------------------------" << endl;
    
    // In order traversal tu dong sap xep theo ma (MAVT)
    // Luu y: de bai yeu cau in theo TEN, nhung AVL tree sap xep theo MA
    // Trong thuc te, co the copy vao mang roi sap xep theo TEN
    InOrderTraversal(g_root_vattu, PrintVatTu);
    
    gotoxy(5, 22);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// main.cpp - Phan 2: Chuc nang nhan vien va hoa don

// =========================
// CHUC NANG NHAN VIEN
// =========================

void ThemNhanVien() {
    clrscr();
    drawLogo();
    drawHotkeys();
    
    gotoxy(50, 3);
    cout << "=== THEM NHAN VIEN ===" << endl;
    
    char manv[10], ho[30], ten[20], phai[5];
    
    gotoxy(50, 5);
    cout << "Ma nhan vien: ";
    inputString(manv, 10, 64, 5);
    
    gotoxy(50, 6);
    cout << "Ho: ";
    inputString(ho, 30, 54, 6);
    
    gotoxy(50, 7);
    cout << "Ten: ";
    inputString(ten, 20, 55, 7);
    
    gotoxy(50, 8);
    cout << "Phai (Nam/Nu): ";
    inputString(phai, 5, 65, 8);
    
    // Validate
    if (strlen(manv) == 0 || strlen(ho) == 0 || 
        strlen(ten) == 0 || strlen(phai) == 0) {
        showError("Cac truong khong duoc rong!");
        return;
    }
    
    // Them nhan vien
    if (InsertNhanVien(g_dsnv, manv, ho, ten, phai)) {
        // Push undo
        PushUndo(g_undoStack, CreateUndoAddNV(manv));
        showSuccess("Da them nhan vien thanh cong!");
    } else {
        showError("Ma nhan vien da ton tai hoac danh sach day!");
    }
}

void InDanhSachNhanVien() {
    clrscr();
    drawLogo();
    
    gotoxy(5, 12);
    PrintDSNhanVien(g_dsnv);
    
    gotoxy(5, 22);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// =========================
// CHUC NANG HOA DON
// =========================

void LapHoaDonMoi() {
    clrscr();
    drawLogo();
    drawHotkeys();
    
    gotoxy(50, 3);
    cout << "=== LAP HOA DON ===" << endl;
    
    // Nhap thong tin hoa don
    char sohd[21], manv[10];
    Date ngay;
    char loai;
    
    gotoxy(50, 5);
    cout << "So hoa don: ";
    inputString(sohd, 21, 62, 5);
    
    gotoxy(50, 6);
    cout << "Ma nhan vien lap: ";
    inputString(manv, 10, 68, 6);
    
    // Kiem tra nhan vien ton tai
    PTRNV nv = SearchNhanVien(g_dsnv, manv);
    if (nv == NULL) {
        showError("Ma nhan vien khong ton tai!");
        return;
    }
    
    gotoxy(50, 7);
    cout << "Ngay lap (dd/mm/yyyy): ";
    char dateStr[15];
    inputString(dateStr, 15, 73, 7);
    sscanf(dateStr, "%d/%d/%d", &ngay.ngay, &ngay.thang, &ngay.nam);
    
    if (!IsValidDate(ngay)) {
        showError("Ngay khong hop le!");
        return;
    }
    
    gotoxy(50, 8);
    cout << "Loai (N=Nhap, X=Xuat): ";
    loai = _getch();
    cout << loai;
    
    if (loai != 'N' && loai != 'X') {
        showError("Loai hoa don khong hop le!");
        return;
    }
    
    // Tao hoa don moi
    PTRHD hd = CreateHoaDon(sohd, ngay, loai);
    
    // Nhap chi tiet hoa don
    gotoxy(50, 10);
    cout << "=== NHAP CHI TIET HOA DON ===" << endl;
    
    int row = 12;
    while (true) {
        char mavt[11];
        int soluong;
        float dongia, vat;
        
        gotoxy(50, row++);
        cout << "Ma vat tu (Enter de ket thuc): ";
        inputString(mavt, 11, 82, row - 1);
        
        if (strlen(mavt) == 0) break;
        
        gotoxy(50, row++);
        cout << "So luong: ";
        soluong = inputInt(60, row - 1);
        
        gotoxy(50, row++);
        cout << "Don gia: ";
        dongia = inputFloat(59, row - 1);
        
        gotoxy(50, row++);
        cout << "VAT (%): ";
        vat = inputFloat(59, row - 1);
        
        // Them vao hoa don va cap nhat ton
        if (!LapHoaDon(hd, g_root_vattu, mavt, soluong, dongia, vat)) {
            showError("Loi khi lap hoa don! Xem thong bao tren.");
            FreeHoaDonList(hd);
            return;
        }
        
        row++;
    }
    
    // Luu hoa don vao nhan vien
    if (nv->dshd == NULL) {
        nv->dshd = hd;
    } else {
        PTRHD p = nv->dshd;
        while (p->next != NULL) p = p->next;
        p->next = hd;
    }
    
    showSuccess("Da lap hoa don thanh cong!");
}

void InHoaDonTheoSo() {
    clrscr();
    drawLogo();
    
    gotoxy(50, 3);
    cout << "=== IN HOA DON ===" << endl;
    
    char sohd[21];
    gotoxy(50, 5);
    cout << "So hoa don: ";
    inputString(sohd, 21, 62, 5);
    
    // Tim hoa don
    bool found = false;
    PTRNV nv_found = NULL;
    PTRHD hd_found = NULL;
    
    for (int i = 0; i < g_dsnv.soNV; i++) {
        PTRHD hd = SearchHoaDon(g_dsnv.nodes[i]->dshd, sohd);
        if (hd != NULL) {
            found = true;
            nv_found = g_dsnv.nodes[i];
            hd_found = hd;
            break;
        }
    }
    
    if (!found) {
        showError("Khong tim thay hoa don!");
        return;
    }
    
    // In thong tin hoa don
    gotoxy(5, 8);
    cout << "========== HOA DON ==========" << endl;
    cout << "So HD: " << hd_found->SoHD << endl;
    printf("Ngay lap: %02d/%02d/%04d\n", hd_found->NgayLap.ngay, 
           hd_found->NgayLap.thang, hd_found->NgayLap.nam);
    cout << "Nhan vien: " << nv_found->HO << " " << nv_found->TEN << endl;
    cout << "Loai: " << (hd_found->Loai == 'N' ? "Nhap" : "Xuat") << endl;
    
    PrintCTHD(hd_found->dscthd);
    
    cout << "\nTONG TRI GIA: " << CalculateTotalValue(hd_found->dscthd) << endl;
    cout << "=============================" << endl;
    
    gotoxy(5, 22);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// main.cpp - Phan 3: Thong ke va menu

// =========================
// CHUC NANG THONG KE
// =========================

void ThongKeTheoNgay() {
    clrscr();
    drawLogo();
    
    gotoxy(50, 3);
    cout << "=== THONG KE HOA DON THEO NGAY ===" << endl;
    
    Date from, to;
    
    gotoxy(50, 5);
    cout << "Tu ngay (dd/mm/yyyy): ";
    char dateStr[15];
    inputString(dateStr, 15, 72, 5);
    sscanf(dateStr, "%d/%d/%d", &from.ngay, &from.thang, &from.nam);
    
    gotoxy(50, 6);
    cout << "Den ngay (dd/mm/yyyy): ";
    inputString(dateStr, 15, 73, 6);
    sscanf(dateStr, "%d/%d/%d", &to.ngay, &to.thang, &to.nam);
    
    if (!IsValidDate(from) || !IsValidDate(to)) {
        showError("Ngay khong hop le!");
        return;
    }
    
    // Thong ke
    HoaDonInfo hdList[1000];
    int count = ThongKeHoaDonTheoNgay(g_dsnv, from, to, hdList, 1000);
    
    clrscr();
    drawLogo();
    InBangThongKeHD(hdList, count);
    
    gotoxy(5, 22);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

void Top10VatTu() {
    clrscr();
    drawLogo();
    
    gotoxy(50, 3);
    cout << "=== TOP 10 VAT TU DOANH THU CAO ===" << endl;
    
    Date from, to;
    
    gotoxy(50, 5);
    cout << "Tu ngay (dd/mm/yyyy): ";
    char dateStr[15];
    inputString(dateStr, 15, 72, 5);
    sscanf(dateStr, "%d/%d/%d", &from.ngay, &from.thang, &from.nam);
    
    gotoxy(50, 6);
    cout << "Den ngay (dd/mm/yyyy): ";
    inputString(dateStr, 15, 73, 6);
    sscanf(dateStr, "%d/%d/%d", &to.ngay, &to.thang, &to.nam);
    
    if (!IsValidDate(from) || !IsValidDate(to)) {
        showError("Ngay khong hop le!");
        return;
    }
    
    // Tinh doanh thu
    VatTuDoanhThu vtList[1000];
    int count = TinhDoanhThuVatTu(g_root_vattu, g_dsnv, from, to, vtList, 1000);
    
    // Sap xep
    SortDoanhThuDesc(vtList, count);
    
    clrscr();
    drawLogo();
    InTop10VatTu(vtList, count);
    
    gotoxy(5, 22);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

void DoanhThuThang() {
    clrscr();
    drawLogo();
    
    gotoxy(50, 3);
    cout << "=== DOANH THU THEO THANG ===" << endl;
    
    int nam;
    gotoxy(50, 5);
    cout << "Nhap nam: ";
    nam = inputInt(60, 5);
    
    if (nam < 1900 || nam > 2100) {
        showError("Nam khong hop le!");
        return;
    }
    
    // Tinh doanh thu
    float doanhThu[13];
    DoanhThuTheoThang(g_dsnv.nodes, g_dsnv.soNV, nam, doanhThu);
    
    clrscr();
    drawLogo();
    InDoanhThuTheoThang(doanhThu);
    
    gotoxy(5, 22);
    cout << "Nhan phim bat ky de tiep tuc...";
    _getch();
}

// =========================
// CAC MENU
// =========================

void MenuVatTu() {
    const char* items[] = {
        "Them vat tu",
        "Sua vat tu",
        "Xoa vat tu",
        "In danh sach vat tu",
        "Quay lai"
    };
    
    while (true) {
        int choice = showMenu(items, 5, "QUAN LY VAT TU");
        
        switch (choice) {
            case 0: ThemVatTu(); break;
            case 1: SuaVatTu(); break;
            case 2: XoaVatTu(); break;
            case 3: InDanhSachVatTu(); break;
            case 4: case -1: return;
        }
    }
}

void MenuNhanVien() {
    const char* items[] = {
        "Them nhan vien",
        "In danh sach nhan vien",
        "Quay lai"
    };
    
    while (true) {
        int choice = showMenu(items, 3, "QUAN LY NHAN VIEN");
        
        switch (choice) {
            case 0: ThemNhanVien(); break;
            case 1: InDanhSachNhanVien(); break;
            case 2: case -1: return;
        }
    }
}

void MenuHoaDon() {
    const char* items[] = {
        "Lap hoa don moi",
        "In hoa don theo so",
        "Quay lai"
    };
    
    while (true) {
        int choice = showMenu(items, 3, "QUAN LY HOA DON");
        
        switch (choice) {
            case 0: LapHoaDonMoi(); break;
            case 1: InHoaDonTheoSo(); break;
            case 2: case -1: return;
        }
    }
}

void MenuThongKe() {
    const char* items[] = {
        "Thong ke hoa don theo ngay",
        "Top 10 vat tu doanh thu cao",
        "Doanh thu theo thang",
        "Quay lai"
    };
    
    while (true) {
        int choice = showMenu(items, 4, "THONG KE & BAO CAO");
        
        switch (choice) {
            case 0: ThongKeTheoNgay(); break;
            case 1: Top10VatTu(); break;
            case 2: DoanhThuThang(); break;
            case 3: case -1: return;
        }
    }
}

// =========================
// HAM MAIN
// =========================

int main() {
    // Cai dat console
    system("color 0F");  // Nen den, chu trang
    system("mode con: cols=120 lines=30");
    
    // Khoi tao
    InitDSNhanVien(g_dsnv);
    InitUndoStack(g_undoStack);
    
    // Doc du lieu tu file (neu co)
    if (LoadAllFromFile(g_root_vattu, g_dsnv, DATA_FILE)) {
        showMessage("Da tai du lieu thanh cong!");
    }
    
    // Menu chinh
    const char* mainMenu[] = {
        "Quan ly vat tu",
        "Quan ly nhan vien",
        "Quan ly hoa don",
        "Thong ke & Bao cao",
        "Undo",
        "Luu du lieu",
        "Thoat"
    };
    
    while (true) {
        int choice = showMenu(mainMenu, 7, "MENU CHINH");
        
        switch (choice) {
            case 0: MenuVatTu(); break;
            case 1: MenuNhanVien(); break;
            case 2: MenuHoaDon(); break;
            case 3: MenuThongKe(); break;
            case 4:
                ExecuteUndo(g_undoStack, g_root_vattu, g_dsnv);
                showMessage("Nhan phim bat ky de tiep tuc...");
                break;
            case 5:
                if (SaveAllToFile(g_root_vattu, g_dsnv, DATA_FILE)) {
                    showSuccess("Da luu du lieu thanh cong!");
                } else {
                    showError("Luu du lieu that bai!");
                }
                break;
            case 6: case -1:
                // Hoi co muon luu truoc khi thoat
                gotoxy(40, 20);
                cout << "Ban co muon luu du lieu truoc khi thoat? (Y/N): ";
                char save = _getch();
                if (save == 'Y' || save == 'y') {
                    SaveAllToFile(g_root_vattu, g_dsnv, DATA_FILE);
                }
                
                // Giai phong bo nho
                FreeVatTuTree(g_root_vattu);
                FreeDSNhanVien(g_dsnv);
                
                clrscr();
                gotoxy(50, 15);
                cout << "Cam on ban da su dung chuong trinh!" << endl;
                return 0;
        }
    }
    
    return 0;
}
