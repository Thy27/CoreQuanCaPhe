#include "Ban.h"

Ban::Ban(int id, std::string name, int seats, TrangThaiBan status) 
{
    this->maBan = id;
    this->tenBan = name;
    this->soGhe = seats;
    this->trangThai = status;
}

int Ban::getMaBan() const { return this->maBan; }
std::string Ban::getTenBan() const { return this->tenBan; }
int Ban::getSoGhe() const { return this->soGhe; }
TrangThaiBan Ban::getTrangThai() const { return this->trangThai; }

void Ban::setTrangThai(TrangThaiBan status) {
    this->trangThai = status;
}

std::string Ban::getTrangThaiString() const {
    switch (this->trangThai) {
        case TRONG: return "Trống";
        case DANG_PHUC_VU: return "Đang phục vụ";
        case DAT_TRUOC: return "Đặt trước";
        default: return "Không rõ";
    }
}