#include "NguyenLieu.h"

#include <cmath>

namespace
{
    // Sai số cho phép khi so sánh số thực (vd. 0.3 - 0.1 - 0.2 không ra đúng 0)
    const double SAI_SO = 1e-9;
}

NguyenLieu::NguyenLieu(int id, std::string ten, std::string dv, double ton, double toiThieu)
{
    this->maNL = id;
    this->tenNL = ten;
    this->donVi = dv;
    this->soLuongTon = ton;
    this->mucToiThieu = toiThieu;
}

int NguyenLieu::getMaNL() const { return this->maNL; }
std::string NguyenLieu::getTenNL() const { return this->tenNL; }
std::string NguyenLieu::getDonVi() const { return this->donVi; }
double NguyenLieu::getSoLuongTon() const { return this->soLuongTon; }
double NguyenLieu::getMucToiThieu() const { return this->mucToiThieu; }

void NguyenLieu::setTenNL(std::string ten) { this->tenNL = ten; }
void NguyenLieu::setDonVi(std::string dv) { this->donVi = dv; }
void NguyenLieu::setSoLuongTon(double ton) { this->soLuongTon = ton; }
void NguyenLieu::setMucToiThieu(double toiThieu) { this->mucToiThieu = toiThieu; }

bool NguyenLieu::du(double soLuong) const
{
    return this->soLuongTon + SAI_SO >= soLuong;
}

void NguyenLieu::nhap(double soLuong)
{
    this->soLuongTon += soLuong;
}

bool NguyenLieu::xuat(double soLuong)
{
    if (!this->du(soLuong)) return false;
    this->soLuongTon -= soLuong;
    if (std::fabs(this->soLuongTon) < SAI_SO) this->soLuongTon = 0;
    return true;
}

bool NguyenLieu::sapHet() const
{
    return this->soLuongTon <= this->mucToiThieu;
}
