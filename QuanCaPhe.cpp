#include "QuanCaPhe.h"

#include <algorithm>
#include <ctime>

QuanCaPhe::QuanCaPhe(std::string thuMucDuLieu) : luuTru(thuMucDuLieu)
{
}

//Hàm phụ

bool QuanCaPhe::baoLoi(const std::string& thongBao)
{
    this->loiCuoi = thongBao;
    return false;
}

bool QuanCaPhe::canDangNhap()
{
    if (!this->daDangNhap()) return this->baoLoi("Bạn cần đăng nhập trước.");
    return true;
}

bool QuanCaPhe::canQuanLy()
{
    if (!this->canDangNhap()) return false;
    if (!this->laQuanLy()) return this->baoLoi("Chỉ quản lý mới được thực hiện thao tác này.");
    return true;
}

bool QuanCaPhe::tuDongLuu()
{
    if (this->luuDuLieu()) return true;
    return this->baoLoi("Đã thực hiện nhưng không lưu được dữ liệu vào thư mục \"" + this->luuTru.getThuMuc()
                        + "\". File có đang mở bằng Excel không?");
}

int QuanCaPhe::viTriBan(int maBan) const
{
    for (size_t i = 0; i < this->danhSachBan.size(); i++) {
        if (this->danhSachBan[i].getMaBan() == maBan) return static_cast<int>(i);
    }
    return -1;
}

int QuanCaPhe::viTriHoaDonDangMo(int maBan) const
{
    for (size_t i = 0; i < this->danhSachHoaDon.size(); i++) {
        const HoaDon& hd = this->danhSachHoaDon[i];
        if (hd.getMaBan() == maBan && !hd.isDaThanhToan()) return static_cast<int>(i);
    }
    return -1;
}

int QuanCaPhe::viTriNhanVien(const std::string& maNV) const
{
    for (size_t i = 0; i < this->danhSachNhanVien.size(); i++) {
        if (this->danhSachNhanVien[i].getMaNV() == maNV) return static_cast<int>(i);
    }
    return -1;
}

int QuanCaPhe::taoMaHoaDonMoi() const
{
    int maLonNhat = 0;
    for (const auto& hd : this->danhSachHoaDon) maLonNhat = std::max(maLonNhat, hd.getMaHD());
    return maLonNhat + 1;
}

bool QuanCaPhe::monDangCoTrongHoaDonMo(int maMon) const
{
    for (const auto& hd : this->danhSachHoaDon) {
        if (!hd.isDaThanhToan() && hd.getSoLuongMon(maMon) > 0) return true;
    }
    return false;
}

std::string QuanCaPhe::thoiGianHienTai()
{
    std::time_t bayGio = std::time(nullptr);
    std::tm t{};
#ifdef _WIN32
    localtime_s(&t, &bayGio);
#else
    localtime_r(&bayGio, &t);
#endif
    char buf[20];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &t);
    return buf;
}

std::string QuanCaPhe::ngayHomNay()
{
    return thoiGianHienTai().substr(0, 10);
}

// Dữ liệu

bool QuanCaPhe::taiDuLieu()
{
    if (!this->luuTru.coDuLieu()) {
        this->taoDuLieuMau();
        return this->luuDuLieu();
    }

    this->luuTru.docMenu(this->menu);
    this->luuTru.docKho(this->kho);
    this->luuTru.docBan(this->danhSachBan);
    this->luuTru.docNhanVien(this->danhSachNhanVien);
    this->luuTru.docHoaDon(this->danhSachHoaDon);
    this->maNVDangNhap.clear();

    // Đồng bộ trạng thái bàn với hóa đơn đang mở (phòng khi file bị sửa tay)
    for (auto& ban : this->danhSachBan) {
        if (this->viTriHoaDonDangMo(ban.getMaBan()) >= 0) ban.setTrangThai(DANG_PHUC_VU);
        else if (ban.getTrangThai() == DANG_PHUC_VU) ban.setTrangThai(TRONG);
    }

    // Không còn tài khoản nào thì không ai đăng nhập được -> tạo lại tài khoản quản lý mặc định
    if (this->danhSachNhanVien.empty()) {
        this->danhSachNhanVien.push_back(NhanVien("QL01", "admin", "admin123", "QuanLy", "Quản lý"));
        return this->luuDuLieu();
    }
    return true;
}

bool QuanCaPhe::luuDuLieu()
{
    bool ok = this->luuTru.luuMenu(this->menu);
    ok = this->luuTru.luuKho(this->kho) && ok;
    ok = this->luuTru.luuBan(this->danhSachBan) && ok;
    ok = this->luuTru.luuNhanVien(this->danhSachNhanVien) && ok;
    ok = this->luuTru.luuHoaDon(this->danhSachHoaDon) && ok;
    if (!ok) return this->baoLoi("Không lưu được dữ liệu vào thư mục \"" + this->luuTru.getThuMuc() + "\".");
    return true;
}

std::string QuanCaPhe::getLoiCuoi() const
{
    return this->loiCuoi;
}

void QuanCaPhe::taoDuLieuMau()
{
    this->kho = Kho();
    this->kho.themNguyenLieu(NguyenLieu(1, "Cà phê hạt", "g", 2000, 300));
    this->kho.themNguyenLieu(NguyenLieu(2, "Sữa đặc", "ml", 3000, 500));
    this->kho.themNguyenLieu(NguyenLieu(3, "Sữa tươi", "ml", 5000, 1000));
    this->kho.themNguyenLieu(NguyenLieu(4, "Bột matcha", "g", 500, 100));
    this->kho.themNguyenLieu(NguyenLieu(5, "Đường", "g", 3000, 500));
    this->kho.themNguyenLieu(NguyenLieu(6, "Đá viên", "g", 20000, 3000));
    this->kho.themNguyenLieu(NguyenLieu(7, "Bánh tiramisu", "cái", 20, 5));

    Mon caPheDen(1, "Cà phê đen", "Cà phê", 25000);
    caPheDen.datThanhPhan(1, 18);
    caPheDen.datThanhPhan(5, 10);
    caPheDen.datThanhPhan(6, 150);

    Mon caPheSua(2, "Cà phê sữa", "Cà phê", 30000);
    caPheSua.datThanhPhan(1, 18);
    caPheSua.datThanhPhan(2, 30);
    caPheSua.datThanhPhan(6, 150);

    Mon bacXiu(3, "Bạc xỉu", "Cà phê", 32000);
    bacXiu.datThanhPhan(1, 10);
    bacXiu.datThanhPhan(2, 20);
    bacXiu.datThanhPhan(3, 80);
    bacXiu.datThanhPhan(6, 150);

    Mon matcha(4, "Matcha latte", "Trà", 45000);
    matcha.datThanhPhan(4, 5);
    matcha.datThanhPhan(3, 150);
    matcha.datThanhPhan(5, 10);
    matcha.datThanhPhan(6, 150);

    Mon tiramisu(5, "Bánh tiramisu", "Bánh", 45000);
    tiramisu.datThanhPhan(7, 1);

    Mon nuocSuoi(6, "Nước suối", "Nước đóng chai", 15000);   // không có công thức -> không trừ kho

    this->menu = Menu();
    this->menu.themMon(caPheDen);
    this->menu.themMon(caPheSua);
    this->menu.themMon(bacXiu);
    this->menu.themMon(matcha);
    this->menu.themMon(tiramisu);
    this->menu.themMon(nuocSuoi);

    this->danhSachBan.clear();
    for (int i = 1; i <= 8; i++) {
        this->danhSachBan.push_back(Ban(i, "Bàn " + std::to_string(i), i <= 4 ? 2 : 4, TRONG));
    }

    this->danhSachNhanVien.clear();
    this->danhSachNhanVien.push_back(NhanVien("QL01", "admin", "admin123", "QuanLy", "Trần Thị B"));
    this->danhSachNhanVien.push_back(NhanVien("NV01", "nhanvien01", "123456", "NhanVien", "Nguyễn Văn A"));

    this->danhSachHoaDon.clear();
    this->maNVDangNhap.clear();
}

// Đăng nhập / nhân viên

bool QuanCaPhe::dangNhap(const std::string& tenDangNhap, const std::string& matKhau)
{
    for (const auto& nv : this->danhSachNhanVien) {
        if (nv.xacThuc(tenDangNhap, matKhau)) {
            this->maNVDangNhap = nv.getMaNV();
            return true;
        }
    }
    return this->baoLoi("Sai tên đăng nhập hoặc mật khẩu.");
}

void QuanCaPhe::dangXuat()
{
    this->maNVDangNhap.clear();
}

bool QuanCaPhe::daDangNhap() const
{
    return this->getNhanVienDangNhap() != nullptr;
}

bool QuanCaPhe::laQuanLy() const
{
    const NhanVien* nv = this->getNhanVienDangNhap();
    return nv != nullptr && nv->laQuanLy();
}

const NhanVien* QuanCaPhe::getNhanVienDangNhap() const
{
    if (this->maNVDangNhap.empty()) return nullptr;
    int i = this->viTriNhanVien(this->maNVDangNhap);
    return i >= 0 ? &this->danhSachNhanVien[i] : nullptr;
}

bool QuanCaPhe::doiMatKhau(const std::string& matKhauCu, const std::string& matKhauMoi)
{
    if (!this->canDangNhap()) return false;
    NhanVien& nv = this->danhSachNhanVien[this->viTriNhanVien(this->maNVDangNhap)];
    if (nv.getMatKhau() != matKhauCu) return this->baoLoi("Mật khẩu cũ không đúng.");
    if (matKhauMoi.empty()) return this->baoLoi("Mật khẩu mới không được để trống.");
    nv.setMatKhau(matKhauMoi);
    return this->tuDongLuu();
}

bool QuanCaPhe::themNhanVien(const std::string& maNV, const std::string& tenDangNhap, const std::string& matKhau,
                             const std::string& vaiTro, const std::string& hoTen)
{
    if (!this->canQuanLy()) return false;
    if (maNV.empty() || tenDangNhap.empty() || matKhau.empty())
        return this->baoLoi("Mã nhân viên, tên đăng nhập và mật khẩu không được để trống.");
    if (vaiTro != "QuanLy" && vaiTro != "NhanVien") return this->baoLoi("Vai trò phải là \"QuanLy\" hoặc \"NhanVien\".");
    for (const auto& nv : this->danhSachNhanVien) {
        if (nv.getMaNV() == maNV) return this->baoLoi("Mã nhân viên " + maNV + " đã tồn tại.");
        if (nv.getTenDangNhap() == tenDangNhap) return this->baoLoi("Tên đăng nhập " + tenDangNhap + " đã tồn tại.");
    }
    this->danhSachNhanVien.push_back(NhanVien(maNV, tenDangNhap, matKhau, vaiTro, hoTen));
    return this->tuDongLuu();
}

bool QuanCaPhe::xoaNhanVien(const std::string& maNV)
{
    if (!this->canQuanLy()) return false;
    if (maNV == this->maNVDangNhap) return this->baoLoi("Không thể xóa tài khoản đang đăng nhập.");
    int i = this->viTriNhanVien(maNV);
    if (i < 0) return this->baoLoi("Không tìm thấy nhân viên " + maNV + ".");
    this->danhSachNhanVien.erase(this->danhSachNhanVien.begin() + i);
    return this->tuDongLuu();
}

const std::vector<NhanVien>& QuanCaPhe::getDanhSachNhanVien() const
{
    return this->danhSachNhanVien;
}

// Menu

int QuanCaPhe::themMon(const std::string& tenMon, const std::string& loai, double gia)
{
    if (!this->canQuanLy()) return -1;
    if (tenMon.empty()) { this->baoLoi("Tên món không được để trống."); return -1; }
    if (gia < 0) { this->baoLoi("Giá món không được âm."); return -1; }
    int ma = this->menu.taoMaMoi();
    this->menu.themMon(Mon(ma, tenMon, loai, gia, true));
    this->tuDongLuu();
    return ma;
}

bool QuanCaPhe::suaMon(int maMon, const std::string& tenMon, const std::string& loai, double gia)
{
    if (!this->canQuanLy()) return false;
    Mon* mon = this->menu.timMon(maMon);
    if (mon == nullptr) return this->baoLoi("Không tìm thấy món mã " + std::to_string(maMon) + ".");
    if (tenMon.empty()) return this->baoLoi("Tên món không được để trống.");
    if (gia < 0) return this->baoLoi("Giá món không được âm.");
    mon->setTenMon(tenMon);
    mon->setLoai(loai);
    mon->setGia(gia);       // hóa đơn đang mở vẫn giữ giá lúc gọi món
    return this->tuDongLuu();
}

bool QuanCaPhe::xoaMon(int maMon)
{
    if (!this->canQuanLy()) return false;
    if (this->menu.timMon(maMon) == nullptr) return this->baoLoi("Không tìm thấy món mã " + std::to_string(maMon) + ".");
    if (this->monDangCoTrongHoaDonMo(maMon))
        return this->baoLoi("Món đang có trong hóa đơn chưa thanh toán. Hãy ngừng bán (datDangBan) thay vì xóa.");
    this->menu.xoaMon(maMon);
    return this->tuDongLuu();
}

bool QuanCaPhe::datDangBan(int maMon, bool dangBan)
{
    if (!this->canQuanLy()) return false;
    Mon* mon = this->menu.timMon(maMon);
    if (mon == nullptr) return this->baoLoi("Không tìm thấy món mã " + std::to_string(maMon) + ".");
    mon->setDangBan(dangBan);
    return this->tuDongLuu();
}

bool QuanCaPhe::datThanhPhan(int maMon, int maNL, double soLuong)
{
    if (!this->canQuanLy()) return false;
    Mon* mon = this->menu.timMon(maMon);
    if (mon == nullptr) return this->baoLoi("Không tìm thấy món mã " + std::to_string(maMon) + ".");
    if (this->kho.timNguyenLieu(maNL) == nullptr)
        return this->baoLoi("Không tìm thấy nguyên liệu mã " + std::to_string(maNL) + ".");
    if (soLuong <= 0) return this->baoLoi("Định lượng phải lớn hơn 0.");
    mon->datThanhPhan(maNL, soLuong);
    return this->tuDongLuu();
}

bool QuanCaPhe::xoaThanhPhan(int maMon, int maNL)
{
    if (!this->canQuanLy()) return false;
    Mon* mon = this->menu.timMon(maMon);
    if (mon == nullptr) return this->baoLoi("Không tìm thấy món mã " + std::to_string(maMon) + ".");
    if (!mon->xoaThanhPhan(maNL)) return this->baoLoi("Công thức của món không có nguyên liệu này.");
    return this->tuDongLuu();
}

const Menu& QuanCaPhe::getMenu() const
{
    return this->menu;
}

// Kho

int QuanCaPhe::themNguyenLieu(const std::string& ten, const std::string& donVi, double soLuongTon, double mucToiThieu)
{
    if (!this->canQuanLy()) return -1;
    if (ten.empty()) { this->baoLoi("Tên nguyên liệu không được để trống."); return -1; }
    if (soLuongTon < 0 || mucToiThieu < 0) { this->baoLoi("Số lượng không được âm."); return -1; }
    int ma = this->kho.taoMaMoi();
    this->kho.themNguyenLieu(NguyenLieu(ma, ten, donVi, soLuongTon, mucToiThieu));
    this->tuDongLuu();
    return ma;
}

bool QuanCaPhe::suaNguyenLieu(int maNL, const std::string& ten, const std::string& donVi, double soLuongTon, double mucToiThieu)
{
    if (!this->canQuanLy()) return false;
    NguyenLieu* nl = this->kho.timNguyenLieu(maNL);
    if (nl == nullptr) return this->baoLoi("Không tìm thấy nguyên liệu mã " + std::to_string(maNL) + ".");
    if (ten.empty()) return this->baoLoi("Tên nguyên liệu không được để trống.");
    if (soLuongTon < 0 || mucToiThieu < 0) return this->baoLoi("Số lượng không được âm.");
    nl->setTenNL(ten);
    nl->setDonVi(donVi);
    nl->setSoLuongTon(soLuongTon);     // dùng khi kiểm kê
    nl->setMucToiThieu(mucToiThieu);
    return this->tuDongLuu();
}

bool QuanCaPhe::xoaNguyenLieu(int maNL)
{
    if (!this->canQuanLy()) return false;
    if (this->kho.timNguyenLieu(maNL) == nullptr)
        return this->baoLoi("Không tìm thấy nguyên liệu mã " + std::to_string(maNL) + ".");
    for (const auto& mon : this->menu.getDanhSachMon()) {
        if (mon.dungNguyenLieu(maNL))
            return this->baoLoi("Nguyên liệu đang có trong công thức món \"" + mon.getTenMon() + "\".");
    }
    this->kho.xoaNguyenLieu(maNL);
    return this->tuDongLuu();
}

bool QuanCaPhe::nhapKho(int maNL, double soLuong)
{
    if (!this->canQuanLy()) return false;
    if (soLuong <= 0) return this->baoLoi("Số lượng nhập phải lớn hơn 0.");
    if (!this->kho.nhapKho(maNL, soLuong))
        return this->baoLoi("Không tìm thấy nguyên liệu mã " + std::to_string(maNL) + ".");
    return this->tuDongLuu();
}

const Kho& QuanCaPhe::getKho() const
{
    return this->kho;
}

std::vector<NguyenLieu> QuanCaPhe::getNguyenLieuSapHet() const
{
    return this->kho.getDanhSachSapHet();
}

int QuanCaPhe::soPhanCoTheBan(int maMon) const
{
    const Mon* mon = this->menu.timMon(maMon);
    if (mon == nullptr) return 0;
    return this->kho.soPhanToiDa(*mon);
}

// Bàn

int QuanCaPhe::themBan(const std::string& tenBan, int soGhe)
{
    if (!this->canQuanLy()) return -1;
    if (soGhe <= 0) { this->baoLoi("Số ghế phải lớn hơn 0."); return -1; }
    int ma = 1;
    for (const auto& ban : this->danhSachBan) ma = std::max(ma, ban.getMaBan() + 1);
    this->danhSachBan.push_back(Ban(ma, tenBan.empty() ? "Bàn " + std::to_string(ma) : tenBan, soGhe, TRONG));
    this->tuDongLuu();
    return ma;
}

bool QuanCaPhe::xoaBan(int maBan)
{
    if (!this->canQuanLy()) return false;
    int i = this->viTriBan(maBan);
    if (i < 0) return this->baoLoi("Không tìm thấy bàn mã " + std::to_string(maBan) + ".");
    if (this->viTriHoaDonDangMo(maBan) >= 0) return this->baoLoi("Bàn đang có hóa đơn chưa thanh toán.");
    this->danhSachBan.erase(this->danhSachBan.begin() + i);
    return this->tuDongLuu();
}

bool QuanCaPhe::datTruocBan(int maBan)
{
    if (!this->canDangNhap()) return false;
    int i = this->viTriBan(maBan);
    if (i < 0) return this->baoLoi("Không tìm thấy bàn mã " + std::to_string(maBan) + ".");
    if (this->danhSachBan[i].getTrangThai() != TRONG) return this->baoLoi("Chỉ đặt trước được bàn đang trống.");
    this->danhSachBan[i].setTrangThai(DAT_TRUOC);
    return this->tuDongLuu();
}

bool QuanCaPhe::huyDatTruoc(int maBan)
{
    if (!this->canDangNhap()) return false;
    int i = this->viTriBan(maBan);
    if (i < 0) return this->baoLoi("Không tìm thấy bàn mã " + std::to_string(maBan) + ".");
    if (this->danhSachBan[i].getTrangThai() != DAT_TRUOC) return this->baoLoi("Bàn này không được đặt trước.");
    this->danhSachBan[i].setTrangThai(TRONG);
    return this->tuDongLuu();
}

bool QuanCaPhe::moBan(int maBan)
{
    if (!this->canDangNhap()) return false;
    int i = this->viTriBan(maBan);
    if (i < 0) return this->baoLoi("Không tìm thấy bàn mã " + std::to_string(maBan) + ".");
    if (this->viTriHoaDonDangMo(maBan) >= 0) return this->baoLoi("Bàn đang phục vụ.");
    this->danhSachHoaDon.push_back(HoaDon(this->taoMaHoaDonMoi(), maBan, thoiGianHienTai()));
    this->danhSachBan[i].setTrangThai(DANG_PHUC_VU);
    return this->tuDongLuu();
}

bool QuanCaPhe::dongBan(int maBan)
{
    if (!this->canDangNhap()) return false;
    int i = this->viTriBan(maBan);
    if (i < 0) return this->baoLoi("Không tìm thấy bàn mã " + std::to_string(maBan) + ".");
    int iHD = this->viTriHoaDonDangMo(maBan);
    if (iHD >= 0) {
        if (!this->danhSachHoaDon[iHD].isRong())
            return this->baoLoi("Bàn còn món chưa thanh toán. Hãy thanh toán hoặc hủy món trước.");
        this->danhSachHoaDon.erase(this->danhSachHoaDon.begin() + iHD);
    }
    this->danhSachBan[i].setTrangThai(TRONG);
    return this->tuDongLuu();
}

const std::vector<Ban>& QuanCaPhe::getDanhSachBan() const
{
    return this->danhSachBan;
}

// Gọi món / thanh toán

bool QuanCaPhe::goiMon(int maBan, int maMon, int soLuong)
{
    if (!this->canDangNhap()) return false;
    if (soLuong <= 0) return this->baoLoi("Số lượng phải lớn hơn 0.");
    int iBan = this->viTriBan(maBan);
    if (iBan < 0) return this->baoLoi("Không tìm thấy bàn mã " + std::to_string(maBan) + ".");
    const Mon* mon = this->menu.timMon(maMon);
    if (mon == nullptr) return this->baoLoi("Không tìm thấy món mã " + std::to_string(maMon) + ".");
    if (!mon->isDangBan()) return this->baoLoi("Món \"" + mon->getTenMon() + "\" đang ngừng bán.");

    // Kiểm tra kho trước, thiếu thì không mở bàn, không trừ gì cả
    std::string thieu;
    if (!this->kho.kiemTraDu(*mon, soLuong, thieu)) return this->baoLoi(thieu);

    int iHD = this->viTriHoaDonDangMo(maBan);
    if (iHD < 0) {
        this->danhSachHoaDon.push_back(HoaDon(this->taoMaHoaDonMoi(), maBan, thoiGianHienTai()));
        iHD = static_cast<int>(this->danhSachHoaDon.size()) - 1;
        this->danhSachBan[iBan].setTrangThai(DANG_PHUC_VU);
    }
    this->kho.truKho(*mon, soLuong);
    this->danhSachHoaDon[iHD].themMon(mon->getMaMon(), mon->getTenMon(), soLuong, mon->getGia());
    return this->tuDongLuu();
}

bool QuanCaPhe::huyMon(int maBan, int maMon, int soLuong)
{
    if (!this->canDangNhap()) return false;
    if (soLuong <= 0) return this->baoLoi("Số lượng phải lớn hơn 0.");
    int iHD = this->viTriHoaDonDangMo(maBan);
    if (iHD < 0) return this->baoLoi("Bàn chưa có hóa đơn.");
    HoaDon& hd = this->danhSachHoaDon[iHD];
    int daGoi = hd.getSoLuongMon(maMon);
    if (daGoi == 0) return this->baoLoi("Bàn này chưa gọi món đó.");

    int soHuy = std::min(soLuong, daGoi);
    hd.giamMon(maMon, soHuy);
    const Mon* mon = this->menu.timMon(maMon);
    if (mon != nullptr) this->kho.hoanKho(*mon, soHuy);
    return this->tuDongLuu();
}

const HoaDon* QuanCaPhe::getHoaDonDangMo(int maBan) const
{
    int i = this->viTriHoaDonDangMo(maBan);
    return i >= 0 ? &this->danhSachHoaDon[i] : nullptr;
}

double QuanCaPhe::tinhTienBan(int maBan, double giamGia, double vat) const
{
    const HoaDon* hd = this->getHoaDonDangMo(maBan);
    return hd != nullptr ? hd->tinhTongCong(giamGia, vat) : -1;
}

bool QuanCaPhe::thanhToan(int maBan, const std::string& hinhThuc, double giamGia, double vat)
{
    if (!this->canDangNhap()) return false;
    int iHD = this->viTriHoaDonDangMo(maBan);
    if (iHD < 0) return this->baoLoi("Bàn chưa có hóa đơn.");
    HoaDon& hd = this->danhSachHoaDon[iHD];
    if (hd.isRong()) return this->baoLoi("Hóa đơn chưa có món nào.");
    if (giamGia < 0) return this->baoLoi("Giảm giá không được âm.");
    if (vat < 0 || vat > 100) return this->baoLoi("VAT phải trong khoảng 0 - 100%.");
    if (hinhThuc.empty()) return this->baoLoi("Chưa chọn hình thức thanh toán.");

    hd.thanhToan(hinhThuc, giamGia, vat, thoiGianHienTai());
    int iBan = this->viTriBan(maBan);
    if (iBan >= 0) this->danhSachBan[iBan].setTrangThai(TRONG);
    return this->tuDongLuu();
}

// Thống kê

double QuanCaPhe::doanhThuTheoNgay(const std::string& ngay) const
{
    return ThongKe::doanhThuTheoNgay(this->danhSachHoaDon, ngay);
}

double QuanCaPhe::doanhThuTheoThang(const std::string& thang) const
{
    return ThongKe::doanhThuTheoThang(this->danhSachHoaDon, thang);
}

double QuanCaPhe::doanhThuKhoangNgay(const std::string& tuNgay, const std::string& denNgay) const
{
    return ThongKe::doanhThuKhoangNgay(this->danhSachHoaDon, tuNgay, denNgay);
}

int QuanCaPhe::soHoaDonTheoNgay(const std::string& ngay) const
{
    return ThongKe::demHoaDonTheoNgay(this->danhSachHoaDon, ngay);
}

double QuanCaPhe::tongDoanhThu() const
{
    return ThongKe::tongDoanhThu(this->danhSachHoaDon);
}

std::vector<std::pair<std::string, int>> QuanCaPhe::monBanChay(int soMon) const
{
    return ThongKe::monBanChay(this->danhSachHoaDon, soMon);
}

const std::vector<HoaDon>& QuanCaPhe::getDanhSachHoaDon() const
{
    return this->danhSachHoaDon;
}

const HoaDon* QuanCaPhe::getHoaDon(int maHD) const
{
    for (const auto& hd : this->danhSachHoaDon) {
        if (hd.getMaHD() == maHD) return &hd;
    }
    return nullptr;
}
