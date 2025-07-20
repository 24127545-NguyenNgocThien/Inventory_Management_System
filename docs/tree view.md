/InventoryManager
│
├── /src
│   ├── /core                # Logic chính của ứng dụng
│   │   ├── app.h/cpp                   # Quản lý vòng đời app (khởi tạo, cấu hình)
│   │   ├── data.h/cpp                  # Kết nối và truy vấn database
│   │   └── commandmanager.h/cpp        # Quản lý việc gọi lệnh trong ui
│   │
│   ├── /modules             # Các tính năng độc lập
│   │   ├── /product         # Quản lý sản phẩm
│   │   │   ├── product.h/cpp               # Lớp cơ sở Product
│   │   │   ├── electricproduct.h/cpp       # Class con 
│   │   │   ├── waterproduct.h/cpp          # Class con 
│   │   │   ├── otherproduct.h/cpp          # Class con 
│   │   │   └── productFactory.h/cpp        # Factory pattern tạo Product
│   │   │
│   │   ├── /command         # Xử lý các thao tác CRUD
│   │   │   ├── command.h/cpp           # Lớp cơ sở Command
│   │   │   ├── addCommand.h/cpp        # Thêm sản phẩm
│   │   │   ├── deleteCommand.h/cpp     # Xóa sản phẩm
│   │   │   └── editCommand.h/cpp       # Sửa sản phẩm
│   │   │
│   │   └── /user            # Quản lý người dùng
│   │       └── user.h/cpp   # Đăng nhập, phân quyền
│   │
│   ├── /ui                  # Giao diện người dùng (Qt)
│   │   ├── mainwindow.h/cpp # Xử lý giao diện chính
│   │   └── mainwindow.ui    # File thiết kế UI (Qt Designer)
│   │
│   └── /utils               # Tiện ích dùng chung
│       ├── logger.h/cpp      # Ghi log thao tác
│       ├── notify.h/cpp      # Cảnh báo sản phẩm sắp hết
│       └── otherutils.h      # Các func hỗ trợ nhỏ
│
├── main.cpp                 # Entry point (tạo file .exe)
└── /docs                    # Tài liệu thiết kế, UML