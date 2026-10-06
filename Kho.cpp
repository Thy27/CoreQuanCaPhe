#include "Kho.h"

#include <algorithm>
#include <cmath>
#include <locale>
#include <sstream>

namespace
{
    std::string soThanhChuoi(double x)
    {
        std::ostringstream out;
        out.imbue(std::locale::classic());
        out << x;
        return out.str();
    }
}

bool Kho::themNguyenLieu(const NguyenLieu& nl)
{
    if (this->timNguyenLieu(nl.getMaNL()) != nullptr) return false;
    this->danhSachNguyenLieu.push_back(nl);
    return true;
}

bool Kho::xoaNguyenLieu(int maNL)
{
    for (auto it = this->danhSachNguyenLieu.begin(); it != this->danhSachNguyenLieu.end(); ++it) {
        if (it->getMaNL() == maNL) {
            this->danhSachNguyenLieu.erase(it);
            return true;
        }
    }
    return false;
}

NguyenLieu* Kho::timNguyenLieu(int maNL)
{
    for (auto& nl : this->danhSachNguyenLieu) {
        if (nl.getMaNL() == maNL) return &nl;
    }
    return nullptr;
}

const NguyenLieu* Kho::timNguyenLieu(int maNL) const
{
    for (const auto& nl : this->danhSachNguyenLieu) {
        if (nl.getMaNL() == maNL) return &nl;
    }
    return nullptr;
}

int Kho::taoMaMoi() const
{
    int maLonNhat = 0;
    for (const auto& nl : this->danhSachNguyenLieu) {
        maLonNhat = std::max(maLonNhat, nl.getMaNL());
    }
    return maLonNhat + 1;
}

const std::vector<NguyenLieu>& Kho::getDanhSachNguyenLieu() const
{
    return this->danhSachNguyenLieu;
}

std::vector<NguyenLieu> Kho::getDanhSachSapHet() const
{
    std::vector<NguyenLieu> ketQua;
    for (const auto& nl : this->danhSachNguyenLieu) {
        if (nl.sapHet()) ketQua.push_back(nl);
    }
    return ketQua;
}

bool Kho::nhapKho(int maNL, double soLuong)
{
    NguyenLieu* nl = this->timNguyenLieu(maNL);
    if (nl == nullptr || soLuong <= 0) return false;
    nl->nhap(soLuong);
    return true;
}

bool Kho::kiemTraDu(const Mon& mon, int soPhan, std::string& thongBaoThieu) const
{
    thongBaoThieu.clear();
    for (const auto& tp : mon.getCongThuc()) {
        const NguyenLieu* nl = this->timNguyenLieu(tp.maNL);
        double canDung = tp.soLuong * soPhan;
        if (nl == nullptr) {
            thongBaoThieu += "Món \"" + mon.getTenMon() + "\" dùng nguyên liệu mã "
                           + std::to_string(tp.maNL) + " nhưng kho không có nguyên liệu này.\n";
        } else if (!nl->du(canDung)) {
            thongBaoThieu += "Không đủ " + nl->getTenNL() + ": cần " + soThanhChuoi(canDung) + " " + nl->getDonVi()
                           + ", còn " + soThanhChuoi(nl->getSoLuongTon()) + " " + nl->getDonVi() + ".\n";
        }
    }
    if (!thongBaoThieu.empty()) thongBaoThieu.pop_back();   
    return thongBaoThieu.empty();
}

bool Kho::truKho(const Mon& mon, int soPhan)
{
    std::string thieu;
    if (soPhan <= 0 || !this->kiemTraDu(mon, soPhan, thieu)) return false;
    for (const auto& tp : mon.getCongThuc()) {
        this->timNguyenLieu(tp.maNL)->xuat(tp.soLuong * soPhan);
    }
    return true;
}

void Kho::hoanKho(const Mon& mon, int soPhan)
{
    if (soPhan <= 0) return;
    for (const auto& tp : mon.getCongThuc()) {
        NguyenLieu* nl = this->timNguyenLieu(tp.maNL);
        if (nl != nullptr) nl->nhap(tp.soLuong * soPhan);
    }
}

int Kho::soPhanToiDa(const Mon& mon) const
{
    if (!mon.coCongThuc()) return -1;
    int ketQua = -1;
    for (const auto& tp : mon.getCongThuc()) {
        const NguyenLieu* nl = this->timNguyenLieu(tp.maNL);
        if (nl == nullptr || tp.soLuong <= 0) return 0;
        int soPhan = static_cast<int>(std::floor(nl->getSoLuongTon() / tp.soLuong + 1e-9));
        if (ketQua < 0 || soPhan < ketQua) ketQua = soPhan;
    }
    return ketQua;
}
