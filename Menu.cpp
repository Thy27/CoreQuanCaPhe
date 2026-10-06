#include "Menu.h"

#include <algorithm>

namespace
{
    // Chỉ đổi A-Z; chữ có dấu (UTF-8 nhiều byte) giữ nguyên
    std::string chuThuong(std::string s)
    {
        for (auto& c : s) {
            if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
        }
        return s;
    }
}

bool Menu::themMon(const Mon& mon)
{
    if (this->timMon(mon.getMaMon()) != nullptr) return false;
    this->danhSachMon.push_back(mon);
    return true;
}

bool Menu::xoaMon(int maMon)
{
    for (auto it = this->danhSachMon.begin(); it != this->danhSachMon.end(); ++it) {
        if (it->getMaMon() == maMon) {
            this->danhSachMon.erase(it);
            return true;
        }
    }
    return false;
}

Mon* Menu::timMon(int maMon)
{
    for (auto& mon : this->danhSachMon) {
        if (mon.getMaMon() == maMon) return &mon;
    }
    return nullptr;
}

const Mon* Menu::timMon(int maMon) const
{
    for (const auto& mon : this->danhSachMon) {
        if (mon.getMaMon() == maMon) return &mon;
    }
    return nullptr;
}

int Menu::taoMaMoi() const
{
    int maLonNhat = 0;
    for (const auto& mon : this->danhSachMon) {
        maLonNhat = std::max(maLonNhat, mon.getMaMon());
    }
    return maLonNhat + 1;
}

const std::vector<Mon>& Menu::getDanhSachMon() const
{
    return this->danhSachMon;
}

std::vector<Mon> Menu::getMonDangBan() const
{
    std::vector<Mon> ketQua;
    for (const auto& mon : this->danhSachMon) {
        if (mon.isDangBan()) ketQua.push_back(mon);
    }
    return ketQua;
}

std::vector<Mon> Menu::locTheoLoai(const std::string& loai) const
{
    std::vector<Mon> ketQua;
    for (const auto& mon : this->danhSachMon) {
        if (mon.getLoai() == loai) ketQua.push_back(mon);
    }
    return ketQua;
}

std::vector<Mon> Menu::timTheoTen(const std::string& tuKhoa) const
{
    std::string tuKhoaThuong = chuThuong(tuKhoa);
    std::vector<Mon> ketQua;
    for (const auto& mon : this->danhSachMon) {
        if (chuThuong(mon.getTenMon()).find(tuKhoaThuong) != std::string::npos) ketQua.push_back(mon);
    }
    return ketQua;
}

std::vector<std::string> Menu::getDanhSachLoai() const
{
    std::vector<std::string> ketQua;
    for (const auto& mon : this->danhSachMon) {
        if (std::find(ketQua.begin(), ketQua.end(), mon.getLoai()) == ketQua.end()) {
            ketQua.push_back(mon.getLoai());
        }
    }
    return ketQua;
}
