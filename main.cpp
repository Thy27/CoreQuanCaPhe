#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include "QuanCaPhe.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

namespace fs = std::filesystem;

static int soDat = 0, soLoi = 0;

static void kiemTra(bool dieuKien, const std::string& moTa)
{
    if (dieuKien) soDat++;
    else soLoi++;
    std::cout << (dieuKien ? "  [OK]  " : "  [LOI] ") << moTa << "\n";
}

// Thao tác phải bị từ chối; in kèm lý do. Đọc getLoiCuoi() bên trong hàm vì thứ tự tính
// đối số không xác định (MSVC tính từ phải sang trái -> đọc lỗi trước khi gọi hàm).
static void kiemTraTuChoi(const QuanCaPhe& q, bool biTuChoi, const std::string& moTa)
{
    kiemTra(biTuChoi, moTa + ": " + q.getLoiCuoi());
}

static bool bang(double a, double b)
{
    return std::fabs(a - b) < 1e-6;
}

static double ton(const QuanCaPhe& q, int maNL)
{
    const NguyenLieu* nl = q.getKho().timNguyenLieu(maNL);
    return nl != nullptr ? nl->getSoLuongTon() : -1;
}

static TrangThaiBan trangThai(const QuanCaPhe& q, int maBan)
{
    for (const auto& ban : q.getDanhSachBan()) {
        if (ban.getMaBan() == maBan) return ban.getTrangThai();
    }
    return TRONG;
}

int main()
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    const std::string THU_MUC = "data_kiemthu";
    const std::string TEN_DAC_BIET = "Trà \"đặc biệt\", size L";
    std::error_code ec;
    fs::remove_all(THU_MUC, ec);

    {
        QuanCaPhe q(THU_MUC);

        std::cout << "== Tải dữ liệu lần đầu ==\n";
        kiemTra(q.taiDuLieu(), "Chưa có file -> tạo dữ liệu mẫu và lưu");
        kiemTra(fs::exists(THU_MUC + "/menu.csv") && fs::exists(THU_MUC + "/chitiethoadon.csv"), "Đã tạo các file CSV");
        kiemTra(q.getMenu().getDanhSachMon().size() == 6 && q.getDanhSachBan().size() == 8, "Có 6 món, 8 bàn mẫu");

        std::cout << "== Đăng nhập, phân quyền ==\n";
        kiemTra(!q.goiMon(2, 2, 1), "Chưa đăng nhập thì không gọi món được");
        kiemTraTuChoi(q, !q.dangNhap("admin", "sai"), "Sai mật khẩu -> từ chối");
        kiemTra(q.dangNhap("nhanvien01", "123456") && !q.laQuanLy(), "Nhân viên đăng nhập được, không phải quản lý");
        kiemTraTuChoi(q, q.themMon("Món lạ", "Khác", 1000) == -1, "Nhân viên không được thêm món");

        std::cout << "== Gọi món + trừ kho (cùng kịch bản demo Core 2) ==\n";
        kiemTra(q.goiMon(2, 2, 2), "Bàn 2 gọi 2 cà phê sữa");
        kiemTra(bang(ton(q, 1), 2000 - 36) && bang(ton(q, 2), 3000 - 60) && bang(ton(q, 6), 20000 - 300),
                "Trừ kho đúng công thức (cà phê 36g, sữa đặc 60ml, đá 300g)");
        kiemTra(trangThai(q, 2) == DANG_PHUC_VU, "Bàn 2 tự chuyển sang Đang phục vụ");
        q.goiMon(2, 5, 1);
        q.goiMon(2, 4, 1);
        kiemTra(bang(q.getHoaDonDangMo(2)->tinhTamTinh(), 150000), "Tạm tính 150000");
        kiemTra(bang(q.tinhTienBan(2, 0, 10), 165000), "Tổng VAT 10% = 165000");
        kiemTra(bang(q.tinhTienBan(2, 20000, 10), 143000), "Giảm 20000 rồi VAT 10% = 143000");
        kiemTra(bang(q.tinhTienBan(2, 999999, 10), 0), "Giảm quá tạm tính -> 0, không âm");

        std::cout << "== Hủy món + hoàn kho ==\n";
        kiemTra(q.huyMon(2, 4, 1) && bang(ton(q, 4), 500) && bang(ton(q, 3), 5000),
                "Hủy matcha -> trả lại bột matcha, sữa tươi");
        kiemTra(bang(q.getHoaDonDangMo(2)->tinhTamTinh(), 105000), "Tạm tính còn 105000");
        q.goiMon(2, 4, 1);

        std::cout << "== Không đủ kho ==\n";
        kiemTraTuChoi(q, !q.goiMon(1, 5, 100), "Gọi 100 bánh -> từ chối");
        kiemTra(bang(ton(q, 7), 19) && trangThai(q, 1) == TRONG && q.getHoaDonDangMo(1) == nullptr,
                "Kho không bị trừ, bàn 1 không bị mở");
        kiemTra(q.soPhanCoTheBan(5) == 19, "Còn bán được 19 bánh");
        kiemTra(q.soPhanCoTheBan(6) == -1 && q.goiMon(1, 6, 3), "Nước suối không theo dõi kho vẫn bán được");
        kiemTra(q.huyMon(1, 6, 99) && q.getHoaDonDangMo(1)->isRong(), "Hủy quá số đã gọi -> chỉ hủy hết số đã gọi");
        kiemTra(q.dongBan(1) && trangThai(q, 1) == TRONG, "Đóng bàn 1 không có món");

        std::cout << "== Thanh toán + thống kê ==\n";
        kiemTra(q.thanhToan(2, "Chuyển khoản", 0, 10), "Thanh toán bàn 2");
        kiemTra(trangThai(q, 2) == TRONG && q.getHoaDonDangMo(2) == nullptr, "Bàn 2 trở về Trống");
        kiemTra(bang(q.tongDoanhThu(), 165000), "Tổng doanh thu tính cả VAT = 165000 (bản cũ ra 150000)");
        kiemTra(bang(q.doanhThuTheoNgay(QuanCaPhe::ngayHomNay()), 165000), "Doanh thu hôm nay = 165000");
        kiemTra(q.soHoaDonTheoNgay(QuanCaPhe::ngayHomNay()) == 1, "Hôm nay có 1 hóa đơn");
        kiemTraTuChoi(q, !q.thanhToan(2, "Tiền mặt", 0, 10), "Không thanh toán lại được");

        q.goiMon(3, 1, 1);   // để bàn 3 đang mở, kiểm tra sau khi tải lại

        std::cout << "== Quản lý menu / kho ==\n";
        q.dangXuat();
        kiemTra(q.dangNhap("admin", "admin123") && q.laQuanLy(), "Quản lý đăng nhập");
        int maMoi = q.themMon(TEN_DAC_BIET, "Trà", 39000);
        kiemTra(maMoi == 7, "Thêm món có dấu phẩy và nháy kép trong tên, mã mới = 7");
        kiemTra(q.datThanhPhan(maMoi, 5, 15), "Đặt công thức cho món mới");
        kiemTra(!q.datThanhPhan(maMoi, 99, 1), "Không đặt được nguyên liệu không tồn tại");
        kiemTraTuChoi(q, !q.xoaNguyenLieu(1), "Không xóa được nguyên liệu đang dùng");
        kiemTraTuChoi(q, !q.xoaMon(1), "Không xóa được món đang trong hóa đơn mở");
        kiemTra(q.nhapKho(7, 10) && bang(ton(q, 7), 29), "Nhập kho 10 bánh -> 29");
        kiemTra(q.suaNguyenLieu(4, "Bột matcha", "g", 50, 100) && q.getNguyenLieuSapHet().size() == 1,
                "Kiểm kê matcha còn 50g -> báo sắp hết");
        kiemTra(q.datDangBan(3, false) && !q.goiMon(4, 3, 1), "Món ngừng bán thì không gọi được");
        kiemTra(q.themNhanVien("NV02", "thungan", "abc", "NhanVien", "Lê Văn C"), "Thêm nhân viên");
        kiemTra(!q.themNhanVien("NV03", "thungan", "x", "NhanVien", "Trùng"), "Không trùng tên đăng nhập");
        kiemTra(!q.xoaNhanVien("QL01"), "Không tự xóa tài khoản đang đăng nhập");
    }

    {
        std::cout << "== Tải lại từ file ==\n";
        QuanCaPhe q(THU_MUC);
        kiemTra(q.taiDuLieu(), "Đọc lại dữ liệu");
        kiemTra(bang(ton(q, 1), 2000 - 36 - 18) && bang(ton(q, 7), 29) && bang(ton(q, 4), 50), "Tồn kho giữ nguyên");
        const Mon* mon = q.getMenu().timMon(7);
        kiemTra(mon != nullptr && mon->getTenMon() == TEN_DAC_BIET && mon->getCongThuc().size() == 1,
                "Tên món có dấu, phẩy, nháy kép và công thức đọc lại đúng");
        kiemTra(!q.getMenu().timMon(3)->isDangBan(), "Trạng thái ngừng bán giữ nguyên");
        kiemTra(trangThai(q, 3) == DANG_PHUC_VU && q.getHoaDonDangMo(3) != nullptr
                && q.getHoaDonDangMo(3)->getSoLuongMon(1) == 1, "Bàn 3 vẫn đang mở với món đã gọi");
        kiemTra(bang(q.tongDoanhThu(), 165000) && bang(q.doanhThuTheoNgay(QuanCaPhe::ngayHomNay()), 165000),
                "Doanh thu giữ nguyên");
        auto top = q.monBanChay(1);
        kiemTra(top.size() == 1 && top[0].first == "Cà phê sữa" && top[0].second == 2, "Món bán chạy nhất: Cà phê sữa x2");
        kiemTra(q.dangNhap("thungan", "abc"), "Nhân viên mới thêm đăng nhập được");
        kiemTra(q.daDangNhap() && q.getNhanVienDangNhap()->getHoTen() == "Lê Văn C", "Họ tên có dấu đọc lại đúng");
    }

    {
        std::cout << "== ThongKe theo ngày / tháng / khoảng ngày ==\n";
        std::vector<HoaDon> ds;
        HoaDon a(1, 1, "2026-10-01 08:00:00");
        a.themMon(1, "A", 2, 10000);
        a.thanhToan("Tiền mặt", 0, 0, "2026-10-01 09:00:00");
        HoaDon b(2, 1, "2026-10-05 08:00:00");
        b.themMon(2, "B", 1, 30000);
        b.thanhToan("Tiền mặt", 0, 0, "2026-10-05 09:00:00");
        HoaDon c(3, 2, "2026-11-02 08:00:00");
        c.themMon(1, "A", 5, 10000);
        c.thanhToan("Thẻ", 0, 10, "2026-11-02 09:00:00");
        HoaDon d(4, 3, "2026-10-05 10:00:00");
        d.themMon(2, "B", 9, 30000);   // chưa thanh toán -> không tính
        ds = { a, b, c, d };

        kiemTra(bang(ThongKe::doanhThuTheoNgay(ds, "2026-10-01"), 20000), "Ngày 01/10 = 20000");
        kiemTra(bang(ThongKe::doanhThuTheoThang(ds, "2026-10"), 50000), "Tháng 10 = 50000");
        kiemTra(bang(ThongKe::doanhThuKhoangNgay(ds, "2026-10-02", "2026-11-30"), 85000), "02/10 - 30/11 = 85000");
        kiemTra(ThongKe::demTongHoaDon(ds) == 3 && bang(ThongKe::tongDoanhThu(ds), 105000), "Tổng 3 hóa đơn = 105000");
        kiemTra(bang(c.getVAT(), 10) && bang(c.tinhTongCong(), 55000), "Tính thống kê không làm mất VAT của hóa đơn");
        kiemTra(!a.themMon(1, "A", 1, 10000), "Hóa đơn đã thanh toán không thêm món được");
        auto top = ThongKe::monBanChay(ds, 5);
        kiemTra(top.size() == 2 && top[0].first == "A" && top[0].second == 7, "Món bán chạy: A x7");
    }

    fs::remove_all(THU_MUC, ec);
    std::cout << "\nKết quả: " << soDat << " đạt, " << soLoi << " lỗi\n";
    return soLoi == 0 ? 0 : 1;
}
