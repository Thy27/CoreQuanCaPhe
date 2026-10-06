#pragma once

#include <string>

class NguyenLieu
{
private:
    int maNL;
    std::string tenNL;
    std::string donVi;      // "g", "ml", "cái"...
    double soLuongTon;
    double mucToiThieu;     // tồn <= mức này thì báo sắp hết

public:
    NguyenLieu(int id = 0, std::string ten = "", std::string dv = "", double ton = 0.0, double toiThieu = 0.0);

    int getMaNL() const;
    std::string getTenNL() const;
    std::string getDonVi() const;
    double getSoLuongTon() const;
    double getMucToiThieu() const;

    void setTenNL(std::string ten);
    void setDonVi(std::string dv);
    void setSoLuongTon(double ton);
    void setMucToiThieu(double toiThieu);

    bool du(double soLuong) const;
    void nhap(double soLuong);
    bool xuat(double soLuong);  // false (và không trừ) nếu không đủ
    bool sapHet() const;
};
