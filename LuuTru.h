#pragma once

#include <string>
#include <vector>
#include "Menu.h"
#include "Kho.h"
#include "Ban.h"
#include "HoaDon.h"
#include "NhanVien.h"

//   menu.csv, kho.csv, ban.csv, nhanvien.csv, hoadon.csv, chitiethoadon.csv
// Hàm doc...: trả về false nếu chưa có file (khi đó dữ liệu truyền vào giữ nguyên).
// Hàm luu...: trả về false nếu không ghi được (vd. file đang mở bằng Excel).
class LuuTru
{
private:
    std::string thuMuc;     // đường dẫn UTF-8, tương đối với thư mục chạy chương trình hoặc tuyệt đối

public:
    LuuTru(std::string thuMucDuLieu = "data");

    std::string getThuMuc() const;
    bool coDuLieu() const;  // đã có ít nhất một file dữ liệu chưa (false = chạy lần đầu)

    bool luuMenu(const Menu& menu) const;
    bool docMenu(Menu& menu) const;

    bool luuKho(const Kho& kho) const;
    bool docKho(Kho& kho) const;

    bool luuBan(const std::vector<Ban>& dsBan) const;
    bool docBan(std::vector<Ban>& dsBan) const;

    bool luuNhanVien(const std::vector<NhanVien>& dsNhanVien) const;
    bool docNhanVien(std::vector<NhanVien>& dsNhanVien) const;

    bool luuHoaDon(const std::vector<HoaDon>& dsHoaDon) const;     // ghi cả hoadon.csv và chitiethoadon.csv
    bool docHoaDon(std::vector<HoaDon>& dsHoaDon) const;
};
