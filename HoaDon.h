#pragma once

#include <vector>
#include <string>
#include "ChiTietHoaDon.h"

class HoaDon
{
private:
    int maHD;
    int maBan;
    std::string thoiGianVao;        // "YYYY-MM-DD HH:MM:SS"
    std::string thoiGianThanhToan;  // rỗng khi chưa thanh toán; ThongKe lọc theo ngày dựa vào trường này
    std::vector<ChiTietHoaDon> danhSachChiTiet;
    double giamGia;                 // số tiền giảm (VND), trừ trước khi tính VAT
    double vat;                     // % VAT
    bool daThanhToan;
    std::string hinhThucThanhToan;

public:
    HoaDon(int id = 0, int mBan = 0, std::string timeIn = "");

    int getMaHD() const;
    int getMaBan() const;
    bool isDaThanhToan() const;
    bool isRong() const;
    std::string getThoiGianVao() const;
    std::string getThoiGianThanhToan() const;
    double getGiamGia() const;
    double getVAT() const;
    std::string getHinhThucThanhToan() const;
    // Chỉ cho đọc: sửa số lượng phải qua themMon/giamMon để còn kiểm tra
    const std::vector<ChiTietHoaDon>& getDanhSachChiTiet() const;
    int getSoLuongMon(int maMon) const;

    bool themMon(int maMon, std::string tenMon, int soLuong, double donGia);
    bool giamMon(int maMon, int soLuong);     // giảm hết thì xóa dòng đó khỏi hóa đơn
    double tinhTamTinh() const;
    double tinhTongCong() const;                                    // theo giảm giá + VAT đã lưu khi thanh toán
    double tinhTongCong(double discount, double taxPercent) const;  // xem trước, không thay đổi hóa đơn
    bool thanhToan(std::string hinhThuc, double discount, double taxPercent, std::string thoiGianTT = "");
};

