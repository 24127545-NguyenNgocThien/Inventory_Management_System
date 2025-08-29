#include "mainwindow.h"
#include "productDialog.h"
#include "./ui_mainwindow.h"
#include "otherutils.h"
#include "addcommand.h"
#include "deletecommand.h"
#include "editcommand.h"
#include "invoice.h"
#include "invoiceService.h"
#include <QMessageBox>
#include <QCloseEvent>
#include <QLabel>

MainWindow::MainWindow(Database& data, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cmdManager(new CommandManager())
    , db(&data)
{
    db->Load();
    db->LoadInvoices();
    ui->setupUi(this);
    setWindowTitle("MainWindow[*]");

    // Tạo và thêm label mới
    QLabel *statusLabel = new QLabel("Trạng thái: Sẵn sàng", this);
    ui->statusBar->addWidget(statusLabel);

    // Tạo action thường
    QAction *toggleAction = new QAction("☰ Toggle Sidebar", this);
    connect(toggleAction, &QAction::triggered, [this]() {
        ui->sidebarDock_2->setVisible(!ui->sidebarDock_2->isVisible());
    });
    ui->toolBar->addAction(toggleAction);

    // Khi chọn item trong sidebar → đổi trang
    connect(ui->listWidget, &QListWidget::currentRowChanged,
            ui->stackedWidget, &QStackedWidget::setCurrentIndex);

    // Đặt mặc định hiển thị trang đầu tiên
    ui->listWidget->setCurrentRow(0);

    // Tùy chọn các bảng mở rộng hết cửa sổ
    ui->tblProducts->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblFormInvoice->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tblListInvoice->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    auto list = db->GetProduct("");
    for (auto pair : list)
    {
        addProductRow(pair.second->GetId(),
                      pair.second->GetName(),
                      pair.second->GetBrand(),
                      pair.second->GetImportPrice(),
                      pair.second->GetSalePrice(),
                      pair.second->GetQuantity()
                      );
    }
    addProductRow("SP001", "Bóng đèn", "Điện quang", 12000, 15000, 50);
    addProductRow("SP002", "Ổ cắm điện", "Điện quang", 50000, 55000, 20);
    addProductRow("SP003", "Công tắc", "Điện quang",  10000, 12000, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cmdManager;
}

//===============================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isWindowModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Xác nhận",
            "Bạn có muốn lưu thay đổi?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Save) {
            if (!db->Save() || !db->SaveInvoices()) {
                event->ignore();  // Không đóng nếu lưu thất bại
                return;
            }
        }
        else if (reply == QMessageBox::Cancel) {
            event->ignore();  // Không đóng
            return;
        }
    }
    event->accept();  // Cho phép đóng
}

//=======================Helper: thêm dòng vào bảng====================================
void MainWindow::addProductRow(const std::string& id, const QString& name, const QString& brand,
                               double importPrice, double salePrice, int quantity)
{
    int row = ui->tblProducts->rowCount();
    ui->tblProducts->insertRow(row);

    ui->tblProducts->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(id)));
    ui->tblProducts->setItem(row, 1, new QTableWidgetItem(name));
    ui->tblProducts->setItem(row, 2, new QTableWidgetItem(brand));
    ui->tblProducts->setItem(row, 3, new QTableWidgetItem(QString::number(importPrice)));
    ui->tblProducts->setItem(row, 4, new QTableWidgetItem(QString::number(salePrice)));
    ui->tblProducts->setItem(row, 5, new QTableWidgetItem(QString::number(quantity)));
}

//===================Products Page================================

void MainWindow::on_txtSearch_textChanged(const QString &arg1)
{
    for (int i = 0; i < ui->tblProducts->rowCount(); ++i) {
        bool match = false;
        for (int j = 0; j < ui->tblProducts->columnCount(); ++j) {
            QTableWidgetItem *item = ui->tblProducts->item(i, j);
            if (item && item->text().contains(arg1, Qt::CaseInsensitive)) {
                match = true;
                break;
            }
        }
        ui->tblProducts->setRowHidden(i, !match);
    }
}

void MainWindow::on_btnAdd_clicked()
{
    ProductDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        ProductType type;
        type = DetermineType(dialog.getCategory());

        ProductParam productInfo;
        productInfo.name = dialog.getName();
        productInfo.brand = dialog.getBrand();
        productInfo.importPrice = dialog.getImportPrice();
        productInfo.salePrice = dialog.getSalePrice();
        productInfo.quantity = dialog.getQuantity();

        auto extras = dialog.getExtras();
        if (!extras.isEmpty()) {
            if (extras.contains("extra1")) productInfo.extra1 = extras["extra1"];
            if (extras.contains("extra2")) productInfo.extra2 = extras["extra2"].toDouble();
            if (extras.contains("extra3")) productInfo.extra3 = extras["extra3"].toDouble();
        }

        Command* cmd = new AddCommand(type, productInfo);
        if (cmdManager->ExecuteCommand(cmd)) {
            QMessageBox::information(this, "Thành công", "Thêm sản phẩm thành công! "
                                                         "Hãy làm mới trang");

            // Bổ sung: Cập nhật lại bảng sản phẩm sau khi thêm thành công
            // Xóa tất cả các dòng hiện có
            ui->tblProducts->setRowCount(0); // NEW
            // Tải lại dữ liệu từ database và hiển thị
            auto list = db->GetProduct(""); // NEW
            for (auto pair : list) // NEW
            { // NEW
                addProductRow(pair.second->GetId(), // NEW
                              pair.second->GetName(), // NEW
                              pair.second->GetBrand(), // NEW
                              pair.second->GetImportPrice(), // NEW
                              pair.second->GetSalePrice(), // NEW
                              pair.second->GetQuantity() // NEW
                              ); // NEW
            } // NEW

            setWindowModified(true); // NEW: Đánh dấu cửa sổ đã thay đổi
            //updateStatusBar(); // NEW: Cập nhật trạng thái thanh trạng thái

        } else {
            QMessageBox::warning(this, "Thất bại", "Thêm sản phẩm thất bại\n"
                                                   "hãy kiểm tra lại thông tin sản phẩm hoặc\n"
                                                   "xem sản phẩm đã tồn tại trước đó chưa?");
        }
    }
}


void MainWindow::on_btnDelete_clicked()
{
    auto selected = ui->tblProducts->currentRow();
    if (selected < 0) {
        QMessageBox::warning(this, "Lỗi", "Hãy chọn sản phẩm để xóa!");
        return;
    }

    // Lấy item ở cột đầu tiên (cột id)
    QTableWidgetItem *firstItem = ui->tblProducts->item(selected, 0);
    std::string id = firstItem->text().toStdString();

    // Xoá sản phẩm
    Command* cmd = new DeleteCommand(id);
    cmdManager->ExecuteCommand(cmd);
    ui->tblProducts->removeRow(selected);

}


void MainWindow::on_btnEdit_clicked()
{
    auto selected = ui->tblProducts->currentRow();
    if (selected < 0) {
        QMessageBox::warning(this, "Lỗi", "Hãy chọn sản phẩm để sửa!");
        return;
    }

    // Lấy item ở cột đầu tiên (cột id)
    QTableWidgetItem *firstItem = ui->tblProducts->item(selected, 0);
    std::string id = firstItem->text().toStdString();
    auto product = db->GetProduct(id);

    ProductDialog dialog(this);
    int category = DetermineType(product[id]->GetId());
    QMap<QString,QString> extras;
    if(category == 0)
    {
        extras = {
            {"extra1", QString(product[id]->GetExtraData1())},
            {"extra2", QString::number(product[id]->GetExtraData2())},
            {"extra3", QString::number(product[id]->GetExtraData3())}
        };
    }
    else if(category == 1)
    {
        extras = {
            {"extra1", QString(product[id]->GetExtraData1())},
            {"extra2", QString::number(product[id]->GetExtraData2())}
        };
    }
    dialog.setProductData(product[id]->GetName(),
                          product[id]->GetBrand(),
                          product[id]->GetImportPrice(),
                          product[id]->GetSalePrice(),
                          product[id]->GetQuantity(),
                          category,
                          extras,
                          true);
    if (dialog.exec() == QDialog::Accepted) {
        // Lấy dữ liệu cập nhật và apply EditCommand
        ProductParam productInfo;
        productInfo.name = dialog.getName();
        productInfo.brand = dialog.getBrand();
        productInfo.importPrice = dialog.getImportPrice();
        productInfo.salePrice = dialog.getSalePrice();
        productInfo.quantity = dialog.getQuantity();

        auto extras = dialog.getExtras();
        if (!extras.isEmpty()) {
            if (extras.contains("extra1")) productInfo.extra1 = extras["extra1"];
            if (extras.contains("extra2")) productInfo.extra2 = extras["extra2"].toDouble();
            if (extras.contains("extra3")) productInfo.extra3 = extras["extra3"].toDouble();
        }

        Command* cmd = new EditCommand(id, productInfo);
        if (cmdManager->ExecuteCommand(cmd)) {
            QMessageBox::information(this, "Thành công", "Chỉnh sửa sản phẩm thành công! "
                                                         "Hãy làm mới trang");

            // Bổ sung: Cập nhật lại bảng sản phẩm sau khi thêm thành công
            // Xóa tất cả các dòng hiện có
            ui->tblProducts->setRowCount(0); // NEW
            // Tải lại dữ liệu từ database và hiển thị
            auto list = db->GetProduct(""); // NEW
            for (auto pair : list) // NEW
            { // NEW
                addProductRow(pair.second->GetId(), // NEW
                              pair.second->GetName(), // NEW
                              pair.second->GetBrand(), // NEW
                              pair.second->GetImportPrice(), // NEW
                              pair.second->GetSalePrice(), // NEW
                              pair.second->GetQuantity() // NEW
                              ); // NEW
            } // NEW

            setWindowModified(true); // NEW: Đánh dấu cửa sổ đã thay đổi
            //updateStatusBar(); // NEW: Cập nhật trạng thái thanh trạng thái

        } else {
            QMessageBox::warning(this, "Thất bại", "Chỉnh sửa sản phẩm thất bại\n"
                                                   "hãy kiểm tra lại thông tin sản phẩm hoặc\n"
                                                   "xem sản phẩm đã tồn tại trước đó chưa?");
        }
    }
}

//============================Invoice Page=========================

void calculateTotal(QTableWidget* table, QLabel* lbl)
{
    double total = 0.0;
    for (int row = 0; row < table->rowCount(); ++row)
    {
        QTableWidgetItem *item = table->item(row, 3); // Cột thành tiền
        total += item->text().toDouble();
    }
    QString content = "Total: " + QString::number(total);
    lbl->setText(content);
}

void MainWindow::on_btnAdd_2_clicked()
{
    int row = ui->tblFormInvoice->rowCount();
    ui->tblFormInvoice->insertRow(row);
    // TODO: thêm combobox sản phẩm, spinbox số lượng, tính thành tiền

    calculateTotal(ui->tblFormInvoice, ui->lblTotal);
}


void MainWindow::on_btnDelete_2_clicked()
{
    int row = ui->tblFormInvoice->currentRow();
    if (row >= 0) {
        ui->tblFormInvoice->removeRow(row);
    }

    calculateTotal(ui->tblFormInvoice, ui->lblTotal);
}


void MainWindow::on_btnSave_2_clicked()
{
    std::vector<InvoiceItem> items;
    for(int row = 0; row < ui->tblFormInvoice->rowCount(); ++row)
    {
        QTableWidgetItem *item0 = ui->tblFormInvoice->item(row, 0); // Cột id
        QTableWidgetItem *item1 = ui->tblFormInvoice->item(row, 1); // Cột đơn giá
        QTableWidgetItem *item2 = ui->tblFormInvoice->item(row, 2); // Cột số lượng

        InvoiceItem item;
        item.productId = item0->text().toStdString();
        item.salePrice = item1->text().toDouble();
        item.quantity = item2->text().toInt();
        items.push_back(item);
    }
    InvoiceService::CreateInvoice(items, cmdManager);

    ui->tblFormInvoice->clearContents();
    ui->tblFormInvoice->setRowCount(0);
}

