#pragma once

#include <string>

class ChiTietHoaDon 
{
private:
    int maMon;
    std::string tenMon;
    int soLuong;
    double donGia;

public:
    ChiTietHoaDon(int mMon = 0, std::string tMon = "", int sl = 0, double gia = 0.0);

    int getMaMon() const;
    std::string getTenMon() const;
    int getSoLuong() const;
    double getDonGia() const;

    void setSoLuong(int sl);
    double getThanhTien() const;
};
