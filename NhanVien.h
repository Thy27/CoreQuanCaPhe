#pragma once

#include <string>

class NhanVien
{
private:
    std::string maNV;
    std::string tenDangNhap;
    std::string matKhau;
    std::string vaiTro; // "QuanLy" hoặc "NhanVien"
    std::string hoTen;

public:
    NhanVien(std::string ma = "", std::string user = "", std::string pass = "", std::string role = "", std::string name = "");

    std::string getMaNV() const;
    std::string getTenDangNhap() const;
    std::string getMatKhau() const;
    std::string getVaiTro() const;
    std::string getHoTen() const;
    bool laQuanLy() const;

    void setMatKhau(std::string pass);
    bool xacThuc(std::string user, std::string pass) const;
};

