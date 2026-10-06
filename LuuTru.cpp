#include "LuuTru.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <locale>
#include <map>
#include <sstream>

namespace fs = std::filesystem;

namespace
{
    const char* const FILE_MENU = "menu.csv";
    const char* const FILE_KHO = "kho.csv";
    const char* const FILE_BAN = "ban.csv";
    const char* const FILE_NHAN_VIEN = "nhanvien.csv";
    const char* const FILE_HOA_DON = "hoadon.csv";
    const char* const FILE_CHI_TIET = "chitiethoadon.csv";

    typedef std::vector<std::string> DongCSV;

    // std::string trong chương trình là UTF-8; trên Windows phải đổi sang path Unicode,
    // nếu không thì thư mục có dấu (vd. "D:\Dự án quản lý cafe") sẽ không mở được.
    fs::path duongDan(const std::string& thuMuc, const char* tenFile)
    {
#if defined(__cpp_char8_t)
        fs::path goc(std::u8string(reinterpret_cast<const char8_t*>(thuMuc.data()), thuMuc.size()));
#else
        fs::path goc = fs::u8path(thuMuc);
#endif
        return goc / tenFile;
    }

    // Bọc trường trong dấu nháy kép nếu có dấu phẩy hoặc nháy kép (chuẩn CSV)
    std::string bocTruong(const std::string& s)
    {
        if (s.find_first_of(",\"\r\n") == std::string::npos) return s;
        std::string kq = "\"";
        for (char c : s) {
            if (c == '"') kq += "\"\"";
            else if (c == '\r' || c == '\n') kq += ' ';   // mỗi bản ghi nằm trên đúng 1 dòng
            else kq += c;
        }
        return kq + "\"";
    }

    DongCSV tachDong(const std::string& dong)
    {
        DongCSV truong;
        std::string hienTai;
        bool trongNhay = false;
        for (size_t i = 0; i < dong.size(); i++) {
            char c = dong[i];
            if (trongNhay) {
                if (c != '"') hienTai += c;
                else if (i + 1 < dong.size() && dong[i + 1] == '"') { hienTai += '"'; i++; }
                else trongNhay = false;
            } else if (c == '"') {
                trongNhay = true;
            } else if (c == ',') {
                truong.push_back(hienTai);
                hienTai.clear();
            } else {
                hienTai += c;
            }
        }
        truong.push_back(hienTai);
        return truong;
    }

    std::string soThanhChuoi(double x)
    {
        std::ostringstream out;
        out.imbue(std::locale::classic());
        out << std::setprecision(15) << x;
        return out.str();
    }

    template <typename T>
    bool docSo(const std::string& s, T& kq)
    {
        std::istringstream in(s);
        in.imbue(std::locale::classic());
        in >> kq;
        return !in.fail();
    }

    bool docFileCSV(const fs::path& duongDanFile, std::vector<DongCSV>& cacDong)
    {
        std::ifstream f(duongDanFile);
        if (!f.is_open()) return false;
        std::string dong;
        bool laTieuDe = true;
        while (std::getline(f, dong)) {
            if (!dong.empty() && dong.back() == '\r') dong.pop_back();
            if (laTieuDe) { laTieuDe = false; continue; }
            if (dong.empty()) continue;
            cacDong.push_back(tachDong(dong));
        }
        return true;
    }

    // Ghi BOM UTF-8 (để Excel hiện đúng tiếng Việt) + tiêu đề + các dòng
    bool ghiFileCSV(const fs::path& duongDanFile, const std::string& tieuDe, const std::vector<DongCSV>& cacDong)
    {
        std::error_code ec;
        if (duongDanFile.has_parent_path()) fs::create_directories(duongDanFile.parent_path(), ec);
        std::ofstream f(duongDanFile);
        if (!f.is_open()) return false;
        f << "\xEF\xBB\xBF" << tieuDe << '\n';
        for (const auto& dong : cacDong) {
            for (size_t i = 0; i < dong.size(); i++) {
                if (i > 0) f << ',';
                f << bocTruong(dong[i]);
            }
            f << '\n';
        }
        f.flush();
        return f.good();
    }

    std::string trangThaiThanhChuoi(TrangThaiBan tt)
    {
        switch (tt) {
            case DANG_PHUC_VU: return "DANG_PHUC_VU";
            case DAT_TRUOC: return "DAT_TRUOC";
            default: return "TRONG";
        }
    }

    TrangThaiBan chuoiThanhTrangThai(const std::string& s)
    {
        if (s == "DANG_PHUC_VU") return DANG_PHUC_VU;
        if (s == "DAT_TRUOC") return DAT_TRUOC;
        return TRONG;
    }
}

LuuTru::LuuTru(std::string thuMucDuLieu)
{
    this->thuMuc = thuMucDuLieu;
}

std::string LuuTru::getThuMuc() const
{
    return this->thuMuc;
}

bool LuuTru::coDuLieu() const
{
    const char* cacFile[] = { FILE_MENU, FILE_KHO, FILE_BAN, FILE_NHAN_VIEN, FILE_HOA_DON, FILE_CHI_TIET };
    for (const char* tenFile : cacFile) {
        std::error_code ec;
        if (fs::exists(duongDan(this->thuMuc, tenFile), ec)) return true;
    }
    return false;
}

// Menu
// congThuc dạng "maNL:soLuong;maNL:soLuong", vd. "1:18;5:10"

bool LuuTru::luuMenu(const Menu& menu) const
{
    std::vector<DongCSV> cacDong;
    for (const auto& mon : menu.getDanhSachMon()) {
        std::string congThuc;
        for (const auto& tp : mon.getCongThuc()) {
            if (!congThuc.empty()) congThuc += ';';
            congThuc += std::to_string(tp.maNL) + ':' + soThanhChuoi(tp.soLuong);
        }
        cacDong.push_back({ std::to_string(mon.getMaMon()), mon.getTenMon(), mon.getLoai(),
                            soThanhChuoi(mon.getGia()), mon.isDangBan() ? "1" : "0", congThuc });
    }
    return ghiFileCSV(duongDan(this->thuMuc, FILE_MENU), "maMon,tenMon,loai,gia,dangBan,congThuc", cacDong);
}

bool LuuTru::docMenu(Menu& menu) const
{
    std::vector<DongCSV> cacDong;
    if (!docFileCSV(duongDan(this->thuMuc, FILE_MENU), cacDong)) return false;

    Menu ketQua;
    for (const auto& t : cacDong) {
        int ma = 0;
        double gia = 0;
        if (t.size() < 5 || !docSo(t[0], ma) || !docSo(t[3], gia)) continue;   // bỏ dòng hỏng

        Mon mon(ma, t[1], t[2], gia, t[4] != "0");
        if (t.size() > 5) {
            std::stringstream ss(t[5]);
            std::string muc;
            while (std::getline(ss, muc, ';')) {
                size_t haiCham = muc.find(':');
                int maNL = 0;
                double soLuong = 0;
                if (haiCham != std::string::npos && docSo(muc.substr(0, haiCham), maNL)
                    && docSo(muc.substr(haiCham + 1), soLuong)) {
                    mon.datThanhPhan(maNL, soLuong);
                }
            }
        }
        ketQua.themMon(mon);
    }
    menu = ketQua;
    return true;
}

//Kho
bool LuuTru::luuKho(const Kho& kho) const
{
    std::vector<DongCSV> cacDong;
    for (const auto& nl : kho.getDanhSachNguyenLieu()) {
        cacDong.push_back({ std::to_string(nl.getMaNL()), nl.getTenNL(), nl.getDonVi(),
                            soThanhChuoi(nl.getSoLuongTon()), soThanhChuoi(nl.getMucToiThieu()) });
    }
    return ghiFileCSV(duongDan(this->thuMuc, FILE_KHO), "maNL,tenNL,donVi,soLuongTon,mucToiThieu", cacDong);
}

bool LuuTru::docKho(Kho& kho) const
{
    std::vector<DongCSV> cacDong;
    if (!docFileCSV(duongDan(this->thuMuc, FILE_KHO), cacDong)) return false;

    Kho ketQua;
    for (const auto& t : cacDong) {
        int ma = 0;
        double ton = 0, toiThieu = 0;
        if (t.size() < 5 || !docSo(t[0], ma) || !docSo(t[3], ton) || !docSo(t[4], toiThieu)) continue;
        ketQua.themNguyenLieu(NguyenLieu(ma, t[1], t[2], ton, toiThieu));
    }
    kho = ketQua;
    return true;
}

// Bàn
bool LuuTru::luuBan(const std::vector<Ban>& dsBan) const
{
    std::vector<DongCSV> cacDong;
    for (const auto& ban : dsBan) {
        cacDong.push_back({ std::to_string(ban.getMaBan()), ban.getTenBan(),
                            std::to_string(ban.getSoGhe()), trangThaiThanhChuoi(ban.getTrangThai()) });
    }
    return ghiFileCSV(duongDan(this->thuMuc, FILE_BAN), "maBan,tenBan,soGhe,trangThai", cacDong);
}

bool LuuTru::docBan(std::vector<Ban>& dsBan) const
{
    std::vector<DongCSV> cacDong;
    if (!docFileCSV(duongDan(this->thuMuc, FILE_BAN), cacDong)) return false;

    std::vector<Ban> ketQua;
    for (const auto& t : cacDong) {
        int ma = 0, soGhe = 0;
        if (t.size() < 4 || !docSo(t[0], ma) || !docSo(t[2], soGhe)) continue;
        ketQua.push_back(Ban(ma, t[1], soGhe, chuoiThanhTrangThai(t[3])));
    }
    dsBan = ketQua;
    return true;
}

// Nhân viên
bool LuuTru::luuNhanVien(const std::vector<NhanVien>& dsNhanVien) const
{
    std::vector<DongCSV> cacDong;
    for (const auto& nv : dsNhanVien) {
        cacDong.push_back({ nv.getMaNV(), nv.getTenDangNhap(), nv.getMatKhau(), nv.getVaiTro(), nv.getHoTen() });
    }
    return ghiFileCSV(duongDan(this->thuMuc, FILE_NHAN_VIEN), "maNV,tenDangNhap,matKhau,vaiTro,hoTen", cacDong);
}

bool LuuTru::docNhanVien(std::vector<NhanVien>& dsNhanVien) const
{
    std::vector<DongCSV> cacDong;
    if (!docFileCSV(duongDan(this->thuMuc, FILE_NHAN_VIEN), cacDong)) return false;

    std::vector<NhanVien> ketQua;
    for (const auto& t : cacDong) {
        if (t.size() < 5 || t[0].empty() || t[1].empty()) continue;
        ketQua.push_back(NhanVien(t[0], t[1], t[2], t[3], t[4]));
    }
    dsNhanVien = ketQua;
    return true;
}

// Hóa đơn
bool LuuTru::luuHoaDon(const std::vector<HoaDon>& dsHoaDon) const
{
    std::vector<DongCSV> dongHoaDon, dongChiTiet;
    for (const auto& hd : dsHoaDon) {
        std::string maHD = std::to_string(hd.getMaHD());
        dongHoaDon.push_back({ maHD, std::to_string(hd.getMaBan()), hd.getThoiGianVao(),
                               hd.isDaThanhToan() ? "1" : "0", hd.getHinhThucThanhToan(),
                               soThanhChuoi(hd.getGiamGia()), soThanhChuoi(hd.getVAT()), hd.getThoiGianThanhToan() });
        for (const auto& ct : hd.getDanhSachChiTiet()) {
            dongChiTiet.push_back({ maHD, std::to_string(ct.getMaMon()), ct.getTenMon(),
                                    std::to_string(ct.getSoLuong()), soThanhChuoi(ct.getDonGia()) });
        }
    }
    bool ok1 = ghiFileCSV(duongDan(this->thuMuc, FILE_HOA_DON),
                          "maHD,maBan,thoiGianVao,daThanhToan,hinhThuc,giamGia,vat,thoiGianThanhToan", dongHoaDon);
    bool ok2 = ghiFileCSV(duongDan(this->thuMuc, FILE_CHI_TIET), "maHD,maMon,tenMon,soLuong,donGia", dongChiTiet);
    return ok1 && ok2;
}

bool LuuTru::docHoaDon(std::vector<HoaDon>& dsHoaDon) const
{
    std::vector<DongCSV> dongHoaDon, dongChiTiet;
    if (!docFileCSV(duongDan(this->thuMuc, FILE_HOA_DON), dongHoaDon)) return false;
    docFileCSV(duongDan(this->thuMuc, FILE_CHI_TIET), dongChiTiet);

    // HoaDon không cho thêm món sau khi đã thanh toán, nên phải: tạo hóa đơn -> thêm chi tiết -> thanh toán
    std::vector<HoaDon> ketQua;
    std::vector<const DongCSV*> dongGoc;
    std::map<int, size_t> viTri;   // maHD -> vị trí trong ketQua
    for (const auto& t : dongHoaDon) {
        int maHD = 0, maBan = 0;
        if (t.size() < 8 || !docSo(t[0], maHD) || !docSo(t[1], maBan) || viTri.count(maHD)) continue;
        viTri[maHD] = ketQua.size();
        ketQua.push_back(HoaDon(maHD, maBan, t[2]));
        dongGoc.push_back(&t);
    }

    for (const auto& t : dongChiTiet) {
        int maHD = 0, maMon = 0, soLuong = 0;
        double donGia = 0;
        if (t.size() < 5 || !docSo(t[0], maHD) || !docSo(t[1], maMon)
            || !docSo(t[3], soLuong) || !docSo(t[4], donGia)) continue;
        auto it = viTri.find(maHD);
        if (it != viTri.end()) ketQua[it->second].themMon(maMon, t[2], soLuong, donGia);
    }

    for (size_t i = 0; i < ketQua.size(); i++) {
        const DongCSV& t = *dongGoc[i];
        double giamGia = 0, vat = 0;
        docSo(t[5], giamGia);
        docSo(t[6], vat);
        if (t[3] == "1") ketQua[i].thanhToan(t[4], giamGia, vat, t[7]);
    }

    dsHoaDon = ketQua;
    return true;
}
