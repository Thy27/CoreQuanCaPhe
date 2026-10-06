#include "HoaDon.h"

#include <cmath>

HoaDon::HoaDon(int id, int mBan, std::string timeIn) {
    this->maHD = id;
    this->maBan = mBan;
    this->thoiGianVao = timeIn;
    this->thoiGianThanhToan = "";
    this->giamGia = 0.0;
    this->vat = 0.0;
    this->daThanhToan = false;
    this->hinhThucThanhToan = "Tiền mặt";
}

int HoaDon::getMaHD() const { return this->maHD; }
int HoaDon::getMaBan() const { return this->maBan; }
bool HoaDon::isDaThanhToan() const { return this->daThanhToan; }
bool HoaDon::isRong() const { return this->danhSachChiTiet.empty(); }
std::string HoaDon::getThoiGianVao() const { return this->thoiGianVao; }
std::string HoaDon::getThoiGianThanhToan() const { return this->thoiGianThanhToan; }
double HoaDon::getGiamGia() const { return this->giamGia; }
double HoaDon::getVAT() const { return this->vat; }
std::string HoaDon::getHinhThucThanhToan() const { return this->hinhThucThanhToan; }

const std::vector<ChiTietHoaDon>& HoaDon::getDanhSachChiTiet() const {
    return this->danhSachChiTiet;
}

int HoaDon::getSoLuongMon(int maMon) const {
    for (const auto& item : this->danhSachChiTiet) {
        if (item.getMaMon() == maMon) return item.getSoLuong();
    }
    return 0;
}

bool HoaDon::themMon(int maMon, std::string tenMon, int soLuong, double donGia) {
    if (this->daThanhToan || soLuong <= 0 || donGia < 0) return false;
    for (auto& item : this->danhSachChiTiet) {
        if (item.getMaMon() == maMon) {
            item.setSoLuong(item.getSoLuong() + soLuong);
            return true;
        }
    }
    this->danhSachChiTiet.push_back(ChiTietHoaDon(maMon, tenMon, soLuong, donGia));
    return true;
}

bool HoaDon::giamMon(int maMon, int soLuong) {
    if (this->daThanhToan || soLuong <= 0) return false;
    for (auto it = this->danhSachChiTiet.begin(); it != this->danhSachChiTiet.end(); ++it) {
        if (it->getMaMon() == maMon) {
            if (soLuong >= it->getSoLuong()) this->danhSachChiTiet.erase(it);
            else it->setSoLuong(it->getSoLuong() - soLuong);
            return true;
        }
    }
    return false;
}

double HoaDon::tinhTamTinh() const {
    double tamTinh = 0;
    for (const auto& item : this->danhSachChiTiet) {
        tamTinh += item.getThanhTien();
    }
    return tamTinh;
}

double HoaDon::tinhTongCong() const {
    return this->tinhTongCong(this->giamGia, this->vat);
}

// Chỉ tính, không ghi đè giamGia/vat của hóa đơn (bản cũ dùng mutable nên ThongKe gọi
// tinhTongCong(0, 0) làm mất VAT của hóa đơn đã thanh toán)
double HoaDon::tinhTongCong(double discount, double taxPercent) const {
    double sauGiamGia = this->tinhTamTinh() - discount;
    if (sauGiamGia < 0) sauGiamGia = 0;
    return std::round(sauGiamGia * (1 + taxPercent / 100.0));   // làm tròn tới đồng
}

bool HoaDon::thanhToan(std::string hinhThuc, double discount, double taxPercent, std::string thoiGianTT) {
    if (this->daThanhToan || this->danhSachChiTiet.empty()) return false;
    if (discount < 0 || taxPercent < 0) return false;
    this->hinhThucThanhToan = hinhThuc;
    this->giamGia = discount;
    this->vat = taxPercent;
    this->thoiGianThanhToan = thoiGianTT;
    this->daThanhToan = true;
    return true;
}
