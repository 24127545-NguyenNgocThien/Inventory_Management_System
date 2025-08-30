# Ứng dụng Quản Lý Cửa Hàng Điện Nước

Ứng dụng được phát triển bằng **Qt6 (C++17)**, sử dụng **JSON** để lưu trữ dữ liệu (sản phẩm, hóa đơn, báo cáo).  
Mục tiêu: hỗ trợ quản lý sản phẩm, hóa đơn, báo cáo doanh thu/lợi nhuận của một cửa hàng điện nước nhỏ.

---

## 🚀 Tính năng chính

### 1. Quản lý Sản phẩm (ProductsPage)
- Hiển thị danh sách sản phẩm trong bảng (`tblProducts`).
- Thêm sản phẩm:
  - Nút **Thêm** (`btnAdd`) mở `ProductDialog`.
  - User chọn loại (Điện / Nước / Khác) → nhập thông tin sản phẩm + các trường bổ sung (extras).
- Sửa sản phẩm:
  - Nút **Sửa** (`btnEdit`) mở `ProductDialog` với dữ liệu đã có.
  - Trường **Loại** bị khóa, chỉ cho chỉnh thông tin khác.
- Nhập sản phẩm từ danh sách:
  - Nút **Nhập hàng** (`btnImport`) mở `SelectionDialog` cho phép chọn nhiều sản phẩm cùng lúc (lọc theo Type → Name → Brand, hoặc tìm kiếm nhanh).
  - Mỗi sản phẩm có **QSpinBox** để chọn số lượng nhập.

### 2. Quản lý Hóa đơn (InvoicesPage)
- Tab 1:
  - Nút **Thêm** (`btnAdd_2`) mở `SelectionDialog` để chọn sản phẩm + số lượng.
  - Kết quả được in vào bảng `tblIFormInvoice`.
- Tab 2:
  - Hiển thị danh sách hóa đơn đã lưu (`tblListInvoice`).
  - Có thể chọn và xem chi tiết.

### 3. Báo cáo (ReportsPage)
- Các trường ngày (`dateFrom`, `dateTo`) mặc định là ngày hôm nay.
- Khi bấm **Xem** (`btnView`):
  - Các label trong groupBox sẽ hiển thị giá trị thực tế (doanh thu, lợi nhuận).
- Biểu đồ doanh thu & lợi nhuận:
  - `label_3` hiển thị **2 chart** (Revenue + Profit) bằng QtCharts.

### 4. Thanh công cụ & Menu
- **ToolBar / MenuBar** gồm các QAction chính:
  - Tải dữ liệu / Lưu dữ liệu.
  - Thoát chương trình.
  - Chuyển trang: Sản phẩm / Hóa đơn / Báo cáo.
- Các QAction có logic xử lý (vd: mở dialog lưu file JSON, load lại dữ liệu…).

### 5. StatusBar
- Hiển thị thông báo trạng thái.
- Có label thống kê số lượng sản phẩm: ví dụ *"Có 123 sản phẩm"*.

---

## 🖼️ Giao diện
- Giao diện GUI (Graphic user interface)
- Chủ đề chính: **xanh dương tối + trắng**.
- Phát triển bằng file .ui 

---

## ⚙️ Cấu trúc dự án


---

## 🛠️ Build & Run

### Yêu cầu
- Qt 6.x (>= 6.5)
- CMake >= 3.16
- Compiler: MinGW (MSYS2) hoặc MSVC

### Các bước
- Clone repo về máy
- Mở Qt và chọn `Open Project...` > chọn file `CMakeLists.txt`
- Nhấn phím `Run` ở góc trái hoặc dùng tổ hợp phím `Ctrl + R`
- File thực thi sẽ nằm trong thư mục `/bin`

---

## 📌 Ghi chú phát triển

- Áp dụng các Design Pattern (Singleton Pattern, Factory Pattern, Command Pattern)
- Có thể mở rộng:
  - Quản lý người dùng & phân quyền.
  - Xuất báo cáo ra PDF/Excel.
  - Kết nối với CSDL ngoài (SQLite, MySQL).

---

## 👨‍💻 Tác giả
- Nguyễn Ngọc Thiên (24127545)
- Nguyễn Chí Tài (24127529)
- Nguyễn Văn Tiến (24127250)
- Hà Như Lương (24127446)

Học tập & phát triển trong quá trình nghiên cứu Qt6 C++