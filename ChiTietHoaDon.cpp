#include "ChiTietHoaDon.h"

ChiTietHoaDon::ChiTietHoaDon(int mMon, std::string tMon, int sl, double gia) 
{
    this->maMon = mMon;
    this->tenMon = tMon;
    this->soLuong = sl;
    this->donGia = gia;
}

int ChiTietHoaDon::getMaMon() const { return this->maMon; }
std::string ChiTietHoaDon::getTenMon() const { return this->tenMon; }
int ChiTietHoaDon::getSoLuong() const { return this->soLuong; }
double ChiTietHoaDon::getDonGia() const { return this->donGia; }

void ChiTietHoaDon::setSoLuong(int sl) 
{
    this->soLuong = sl;
}

double ChiTietHoaDon::getThanhTien() const 
{
    return this->soLuong * this->donGia;
}