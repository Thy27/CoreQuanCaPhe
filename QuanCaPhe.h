#pragma once

#include <string>
#include <utility>
#include <vector>
#include "Menu.h"
#include "Kho.h"
#include "LuuTru.h"
#include "Ban.h"
#include "HoaDon.h"
#include "NhanVien.h"
#include "ThongKe.h"

// Quy ước:
//  - Gọi taiDuLieu() một lần khi mở chương trình (lần đầu chưa có file sẽ tự tạo dữ liệu mẫu).
//  - Hàm trả về bool: false = thất bại. Hàm trả về mã (int): -1 = thất bại.
//    Khi thất bại, getLoiCuoi() cho biết lý do bằng tiếng Việt (hiện thẳng lên MessageBox được).
//  - Mọi thay đổi được tự động lưu xuống file CSV.
//  - Con trỏ / tham chiếu trả về chỉ để đọc và dùng ngay, đừng giữ lại sau khi gọi hàm thay đổi dữ liệu.
//  - Ngày dạng "YYYY-MM-DD", tháng dạng "YYYY-MM". Chuỗi là UTF-8 (biên dịch với /utf-8).
//  - Quyền: quản lý mới được sửa menu, kho, bàn, nhân viên. Nhân viên được gọi món, thanh toán, đặt bàn.
class QuanCaPhe
{
private:
    Menu menu;
    Kho kho;
    std::vector<Ban> danhSachBan;
    std::vector<HoaDon> danhSachHoaDon;
    std::vector<NhanVien> danhSachNhanVien;
    LuuTru luuTru;
    std::string maNVDangNhap;
    std::string loiCuoi;

    bool baoLoi(const std::string& thongBao);
    bool canDangNhap();
    bool canQuanLy();
    bool tuDongLuu();
    int viTriBan(int maBan) const;
    int viTriHoaDonDangMo(int maBan) const;
    int viTriNhanVien(const std::string& maNV) const;
    int taoMaHoaDonMoi() const;
    bool monDangCoTrongHoaDonMo(int maMon) const;
    void taoDuLieuMau();

public:
    QuanCaPhe(std::string thuMucDuLieu = "data");

    //Dữ liệu
    bool taiDuLieu();
    bool luuDuLieu();
    std::string getLoiCuoi() const;

    // Đăng nhập / nhân viên
    bool dangNhap(const std::string& tenDangNhap, const std::string& matKhau);
    void dangXuat();
    bool daDangNhap() const;
    bool laQuanLy() const;
    const NhanVien* getNhanVienDangNhap() const;
    bool doiMatKhau(const std::string& matKhauCu, const std::string& matKhauMoi);
    bool themNhanVien(const std::string& maNV, const std::string& tenDangNhap, const std::string& matKhau,
                      const std::string& vaiTro, const std::string& hoTen);     // vaiTro: "QuanLy" / "NhanVien"
    bool xoaNhanVien(const std::string& maNV);
    const std::vector<NhanVien>& getDanhSachNhanVien() const;

    // Menu
    int themMon(const std::string& tenMon, const std::string& loai, double gia);   // trả về mã món mới
    bool suaMon(int maMon, const std::string& tenMon, const std::string& loai, double gia);
    bool xoaMon(int maMon);
    bool datDangBan(int maMon, bool dangBan);                   // ẩn/hiện món mà không xóa
    bool datThanhPhan(int maMon, int maNL, double soLuong);     // công thức: 1 phần món cần soLuong nguyên liệu maNL
    bool xoaThanhPhan(int maMon, int maNL);
    const Menu& getMenu() const;

    // Kho
    int themNguyenLieu(const std::string& ten, const std::string& donVi, double soLuongTon, double mucToiThieu);
    bool suaNguyenLieu(int maNL, const std::string& ten, const std::string& donVi, double soLuongTon, double mucToiThieu);
    bool xoaNguyenLieu(int maNL);
    bool nhapKho(int maNL, double soLuong);
    const Kho& getKho() const;
    std::vector<NguyenLieu> getNguyenLieuSapHet() const;
    int soPhanCoTheBan(int maMon) const;        // -1: món không theo dõi kho, 0: hết

    // Bàn
    int themBan(const std::string& tenBan, int soGhe);          // trả về mã bàn mới
    bool xoaBan(int maBan);
    bool datTruocBan(int maBan);
    bool huyDatTruoc(int maBan);
    bool moBan(int maBan);
    bool dongBan(int maBan);                                    // đóng bàn đã mở nhưng chưa gọi món
    const std::vector<Ban>& getDanhSachBan() const;

    //Gọi món/hóa đơn
    bool goiMon(int maBan, int maMon, int soLuong);             // tự mở bàn nếu chưa mở; trừ kho ngay
    bool huyMon(int maBan, int maMon, int soLuong);             // trả lại kho
    const HoaDon* getHoaDonDangMo(int maBan) const;             // nullptr nếu bàn chưa có hóa đơn
    double tinhTienBan(int maBan, double giamGia, double vat) const;   // xem trước tổng tiền; -1 nếu không có hóa đơn
    bool thanhToan(int maBan, const std::string& hinhThuc, double giamGia, double vat);  // giamGia: số tiền, vat: %

    // Thống kê
    double doanhThuTheoNgay(const std::string& ngay) const;
    double doanhThuTheoThang(const std::string& thang) const;
    double doanhThuKhoangNgay(const std::string& tuNgay, const std::string& denNgay) const;
    int soHoaDonTheoNgay(const std::string& ngay) const;
    double tongDoanhThu() const;
    std::vector<std::pair<std::string, int>> monBanChay(int soMon) const;
    const std::vector<HoaDon>& getDanhSachHoaDon() const;
    const HoaDon* getHoaDon(int maHD) const;

    static std::string thoiGianHienTai();   // "YYYY-MM-DD HH:MM:SS"
    static std::string ngayHomNay();        // "YYYY-MM-DD"
};
