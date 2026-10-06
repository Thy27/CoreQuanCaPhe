#include "Mon.h"

Mon::Mon(int id, std::string ten, std::string loaiMon, double giaBan, bool ban)
{
    this->maMon = id;
    this->tenMon = ten;
    this->loai = loaiMon;
    this->gia = giaBan;
    this->dangBan = ban;
}

int Mon::getMaMon() const { return this->maMon; }
std::string Mon::getTenMon() const { return this->tenMon; }
std::string Mon::getLoai() const { return this->loai; }
double Mon::getGia() const { return this->gia; }
bool Mon::isDangBan() const { return this->dangBan; }
const std::vector<ThanhPhan>& Mon::getCongThuc() const { return this->congThuc; }
bool Mon::coCongThuc() const { return !this->congThuc.empty(); }

void Mon::setTenMon(std::string ten) { this->tenMon = ten; }
void Mon::setLoai(std::string loaiMon) { this->loai = loaiMon; }
void Mon::setGia(double giaBan) { this->gia = giaBan; }
void Mon::setDangBan(bool ban) { this->dangBan = ban; }

void Mon::datThanhPhan(int maNL, double soLuong)
{
    for (auto& tp : this->congThuc) {
        if (tp.maNL == maNL) {
            tp.soLuong = soLuong;
            return;
        }
    }
    this->congThuc.push_back({ maNL, soLuong });
}

bool Mon::xoaThanhPhan(int maNL)
{
    for (auto it = this->congThuc.begin(); it != this->congThuc.end(); ++it) {
        if (it->maNL == maNL) {
            this->congThuc.erase(it);
            return true;
        }
    }
    return false;
}

bool Mon::dungNguyenLieu(int maNL) const
{
    for (const auto& tp : this->congThuc) {
        if (tp.maNL == maNL) return true;
    }
    return false;
}
