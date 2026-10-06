#include "NhanVien.h"

NhanVien::NhanVien(std::string ma, std::string user, std::string pass, std::string role, std::string name)
{
    this->maNV = ma;
    this->tenDangNhap = user;
    this->matKhau = pass;
    this->vaiTro = role;
    this->hoTen = name;
}

std::string NhanVien::getMaNV() const { return this->maNV; }
std::string NhanVien::getTenDangNhap() const { return this->tenDangNhap; }
std::string NhanVien::getMatKhau() const { return this->matKhau; }
std::string NhanVien::getVaiTro() const { return this->vaiTro; }
std::string NhanVien::getHoTen() const { return this->hoTen; }

bool NhanVien::laQuanLy() const
{
    return this->vaiTro == "QuanLy";
}

void NhanVien::setMatKhau(std::string pass)
{
    this->matKhau = pass;
}

bool NhanVien::xacThuc(std::string user, std::string pass) const
{
    return (this->tenDangNhap == user && this->matKhau == pass);
}
