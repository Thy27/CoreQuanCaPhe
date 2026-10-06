#include "ThongKe.h"

#include <algorithm>
#include <map>

double ThongKe::tongDoanhThu(const std::vector<HoaDon>& dsHoaDon)
{
    double tong = 0;
    for (const auto& hd : dsHoaDon) {
        if (hd.isDaThanhToan()) {
            tong += hd.tinhTongCong();   // giảm giá + VAT đã lưu lúc thanh toán
        }
    }
    return tong;
}

int ThongKe::demTongHoaDon(const std::vector<HoaDon>& dsHoaDon)
 {
    int count = 0;
    for (const auto& hd : dsHoaDon) {
        if (hd.isDaThanhToan()) {
            count++;
        }
    }
    return count;
}

double ThongKe::doanhThuTheoTienTo(const std::vector<HoaDon>& dsHoaDon, const std::string& tienTo)
{
    double tong = 0;
    for (const auto& hd : dsHoaDon) {
        if (hd.isDaThanhToan() && hd.getThoiGianThanhToan().compare(0, tienTo.size(), tienTo) == 0) {
            tong += hd.tinhTongCong();
        }
    }
    return tong;
}

double ThongKe::doanhThuTheoNgay(const std::vector<HoaDon>& dsHoaDon, const std::string& ngay)
{
    return doanhThuTheoTienTo(dsHoaDon, ngay);
}

double ThongKe::doanhThuTheoThang(const std::vector<HoaDon>& dsHoaDon, const std::string& thang)
{
    return doanhThuTheoTienTo(dsHoaDon, thang);
}

double ThongKe::doanhThuKhoangNgay(const std::vector<HoaDon>& dsHoaDon, const std::string& tuNgay, const std::string& denNgay)
{
    double tong = 0;
    for (const auto& hd : dsHoaDon) {
        std::string ngay = hd.getThoiGianThanhToan().substr(0, 10);
        if (hd.isDaThanhToan() && ngay.size() == 10 && ngay >= tuNgay && ngay <= denNgay) {
            tong += hd.tinhTongCong();
        }
    }
    return tong;
}

int ThongKe::demHoaDonTheoNgay(const std::vector<HoaDon>& dsHoaDon, const std::string& ngay)
{
    int count = 0;
    for (const auto& hd : dsHoaDon) {
        if (hd.isDaThanhToan() && hd.getThoiGianThanhToan().compare(0, ngay.size(), ngay) == 0) {
            count++;
        }
    }
    return count;
}

std::vector<std::pair<std::string, int>> ThongKe::monBanChay(const std::vector<HoaDon>& dsHoaDon, int soMon)
{
    std::map<int, std::pair<std::string, int>> tongTheoMon;   // maMon -> (tên, số lượng)
    for (const auto& hd : dsHoaDon) {
        if (!hd.isDaThanhToan()) continue;
        for (const auto& ct : hd.getDanhSachChiTiet()) {
            auto& muc = tongTheoMon[ct.getMaMon()];
            muc.first = ct.getTenMon();
            muc.second += ct.getSoLuong();
        }
    }

    std::vector<std::pair<std::string, int>> ketQua;
    for (const auto& p : tongTheoMon) ketQua.push_back(p.second);
    std::stable_sort(ketQua.begin(), ketQua.end(),
        [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) { return a.second > b.second; });
    if (soMon >= 0 && ketQua.size() > static_cast<size_t>(soMon)) ketQua.resize(soMon);
    return ketQua;
}
