#pragma once

#include <string>
#include <vector>
#include "Mon.h"
#include "NguyenLieu.h"

class Kho
{
private:
    std::vector<NguyenLieu> danhSachNguyenLieu;

public:
    bool themNguyenLieu(const NguyenLieu& nl);     // false nếu trùng mã
    bool xoaNguyenLieu(int maNL);
    NguyenLieu* timNguyenLieu(int maNL);           // nullptr nếu không có
    const NguyenLieu* timNguyenLieu(int maNL) const;
    int taoMaMoi() const;
    const std::vector<NguyenLieu>& getDanhSachNguyenLieu() const;
    std::vector<NguyenLieu> getDanhSachSapHet() const;

    bool nhapKho(int maNL, double soLuong);

    // Bán hàng: trừ kho theo công thức của món
    bool kiemTraDu(const Mon& mon, int soPhan, std::string& thongBaoThieu) const;
    bool truKho(const Mon& mon, int soPhan);       // kiểm tra đủ hết rồi mới trừ, không trừ dở dang
    void hoanKho(const Mon& mon, int soPhan);      // trả lại kho khi hủy món
    int soPhanToiDa(const Mon& mon) const;         // còn làm được bao nhiêu phần và -1 nếu món không có công thức
};
