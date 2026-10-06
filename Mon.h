#pragma once

#include <string>
#include <vector>

// Một dòng trong công thức: 1 phần món cần soLuong nguyên liệu maNL (theo đơn vị của nguyên liệu)
struct ThanhPhan
{
    int maNL;
    double soLuong;
};

class Mon
{
private:
    int maMon;
    std::string tenMon;
    std::string loai;       // "Cà phê", "Trà", "Bánh"...
    double gia;
    bool dangBan;           // false: tạm ẩn khỏi menu, không gọi được
    std::vector<ThanhPhan> congThuc;

public:
    Mon(int id = 0, std::string ten = "", std::string loaiMon = "", double giaBan = 0.0, bool ban = true);

    int getMaMon() const;
    std::string getTenMon() const;
    std::string getLoai() const;
    double getGia() const;
    bool isDangBan() const;
    const std::vector<ThanhPhan>& getCongThuc() const;
    bool coCongThuc() const;    // false: món không theo dõi kho (vd: nước đóng chai nhập theo thùng)

    void setTenMon(std::string ten);
    void setLoai(std::string loaiMon);
    void setGia(double giaBan);
    void setDangBan(bool ban);

    void datThanhPhan(int maNL, double soLuong);   // thêm mới, hoặc sửa định lượng nếu đã có
    bool xoaThanhPhan(int maNL);
    bool dungNguyenLieu(int maNL) const;
};
