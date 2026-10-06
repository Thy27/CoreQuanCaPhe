#pragma once

#include <string>
#include <vector>
#include "Mon.h"

class Menu
{
private:
    std::vector<Mon> danhSachMon;

public:
    bool themMon(const Mon& mon);       // false nếu trùng mã
    bool xoaMon(int maMon);
    Mon* timMon(int maMon);             // nullptr nếu không có
    const Mon* timMon(int maMon) const;
    int taoMaMoi() const;

    const std::vector<Mon>& getDanhSachMon() const;
    std::vector<Mon> getMonDangBan() const;
    std::vector<Mon> locTheoLoai(const std::string& loai) const;
    std::vector<Mon> timTheoTen(const std::string& tuKhoa) const;   // không phân biệt hoa/thường với chữ không dấu
    std::vector<std::string> getDanhSachLoai() const;
};
