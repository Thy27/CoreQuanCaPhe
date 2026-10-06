#pragma once

#include <string>
#include <utility>
#include <vector>
#include "HoaDon.h"

// Chỉ tính hóa đơn đã thanh toán. Ngày dạng "YYYY-MM-DD", tháng dạng "YYYY-MM",
// so với thời gian thanh toán của hóa đơn.
class ThongKe
{
private:
    static double doanhThuTheoTienTo(const std::vector<HoaDon>& dsHoaDon, const std::string& tienTo);

public:
    static double tongDoanhThu(const std::vector<HoaDon>& dsHoaDon);
    static int demTongHoaDon(const std::vector<HoaDon>& dsHoaDon);

    static double doanhThuTheoNgay(const std::vector<HoaDon>& dsHoaDon, const std::string& ngay);
    static double doanhThuTheoThang(const std::vector<HoaDon>& dsHoaDon, const std::string& thang);
    static double doanhThuKhoangNgay(const std::vector<HoaDon>& dsHoaDon, const std::string& tuNgay, const std::string& denNgay);
    static int demHoaDonTheoNgay(const std::vector<HoaDon>& dsHoaDon, const std::string& ngay);
    // Các món bán chạy nhất: (tên món, tổng số lượng), giảm dần
    static std::vector<std::pair<std::string, int>> monBanChay(const std::vector<HoaDon>& dsHoaDon, int soMon);
};

