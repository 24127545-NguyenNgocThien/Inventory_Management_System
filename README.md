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

```
INVENTORY_MANAGERMENT_SYSTEM
│   .gitignore
│   CMakeLists.txt
│   README.md
│
├───bin                                     # Khi build thì file thực thi sẽ ở trong này
│       app.log
│       inventory.json
│       invoices.json
│
├───docs
│       Cuốn báo cáo đề án.docx
│
├───src
│   │   CMakeLists.txt
│   │   main.cpp
│   │
│   ├───core                                
│   │   │   CMakeLists.txt
│   │   │ 
│   │   ├───app                             # Logic chính quản lý app 
│   │   │       app.cpp
│   │   │       app.h
│   │   │       CMakeLists.txt
│   │   │
│   │   ├───commandManager                  # Quản lý các command để Undo, Redo
│   │   │       CMakeLists.txt
│   │   │       commandmanager.cpp
│   │   │       commandmanager.h
│   │   │
│   │   └───data                            # Database lưu dữ liệu làm việc vào các file .json
│   │           CMakeLists.txt
│   │           data.cpp
│   │           data.h
│   │
│   ├───modules
│   │   │   CMakeLists.txt
│   │   │
│   │   ├───command                         # Ứng dụng Command pattern
│   │   │   │   CMakeLists.txt
│   │   │   │   command.h
│   │   │   │
│   │   │   └───types
│   │   │           addcommand.cpp
│   │   │           addcommand.h
│   │   │           CMakeLists.txt
│   │   │           deletecommand.cpp
│   │   │           deletecommand.h
│   │   │           editcommand.cpp
│   │   │           editcommand.h
│   │   │           exportCommand.cpp
│   │   │           exportCommand.h
│   │   │           importCommand.cpp
│   │   │           importCommand.h
│   │   │
│   │   ├───invoice                         # Tạo hóa đơn
│   │   │       CMakeLists.txt
│   │   │       invoice.cpp
│   │   │       invoice.h
│   │   │       invoiceService.cpp
│   │   │       invoiceService.h
│   │   │
│   │   ├───product                         # Ứng dụng Factory pattern
│   │   │   │   CMakeLists.txt
│   │   │   │   product.cpp
│   │   │   │   product.h
│   │   │   │
│   │   │   ├───factory                   
│   │   │   │       CMakeLists.txt
│   │   │   │       productfactory.cpp
│   │   │   │       productfactory.h
│   │   │   │
│   │   │   └───types
│   │   │           CMakeLists.txt
│   │   │           electricproduct.cpp
│   │   │           electricproduct.h
│   │   │           otherproduct.cpp
│   │   │           otherproduct.h
│   │   │           waterproduct.cpp
│   │   │           waterproduct.h
│   │   │
│   │   └───report                          # Phân tích dữ liệu làm việc để thống kê lại
│   │           CMakeLists.txt
│   │           report.cpp
│   │           report.h
│   │
│   ├───ui                                  # module ui được xử lý riêng, tách biệt logic ui vs logic xử lý 
│   │       CMakeLists.txt
│   │       invoiceDialog.cpp
│   │       invoiceDialog.h
│   │       mainwindow.cpp
│   │       mainwindow.h
│   │       mainwindow.ui
│   │       productDialog.cpp
│   │       productDialog.h
│   │       selectionDialog.cpp
│   │       selectionDialog.h
│   │
│   └───utils                               # Các chức năng nhỏ phụ trợ
│       │   CMakeLists.txt
│       │
│       ├───logger                          # Ghi log ra file app.log
│       │       CMakeLists.txt
│       │       log.cpp
│       │       log.h
│       │
│       ├───notify                          # Đăng các thông báo cho user khi thao tác trên GUI 
│       │       CMakeLists.txt
│       │       notify.cpp
│       │       notify.h
│       │
│       └───other                           # Những hàm nhỏ như tạo ID, tách chữ,... phụ trợ cho logic chính được đưa vào đây
│               CMakeLists.txt
│               otherutils.h
│
└───test                                    # Test hiệu năng app, đã có đầy đủ thư viện động cần thiết, chỉ cần extract và chạy App.exe
        App.zip
```

---

## Class diagram
[![App-UML.png](https://i.postimg.cc/g0hgXgyb/App-UML.png)](https://postimg.cc/yJVm2XrL)

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