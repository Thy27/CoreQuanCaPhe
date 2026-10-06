#pragma once

#include <string>

enum TrangThaiBan 
{
    TRONG,
    DANG_PHUC_VU,
    DAT_TRUOC
};

class Ban 
{
private:
    int maBan;
    std::string tenBan;
    int soGhe;
    TrangThaiBan trangThai;

public:
    Ban(int id = 0, std::string name = "", int seats = 4, TrangThaiBan status = TRONG);

    int getMaBan() const;
    std::string getTenBan() const;
    int getSoGhe() const;
    TrangThaiBan getTrangThai() const;

    void setTrangThai(TrangThaiBan status);
    std::string getTrangThaiString() const;
};

