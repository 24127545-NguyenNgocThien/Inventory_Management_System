#include "mainwindow.h"
#include "productDialog.h"
#include "selectionDialog.h"
#include "./ui_mainwindow.h"
#include "otherutils.h"
#include "addcommand.h"
#include "deletecommand.h"
#include "editcommand.h"
#include "importCommand.h"
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

    fillOutProductTable();
    fillOutInvoiceTable();
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
void MainWindow::addRow(QTableWidget* table, QList<QString> data)
{
    int row = table->rowCount();
    table->insertRow(row);

    for (int i = 0; i < table->columnCount(); ++i)
    {
        table->setItem(row, i, new QTableWidgetItem(data[i]));
    }
}

void MainWindow::fillOutInvoiceTable()
{
    ui->tblListInvoice->setRowCount(0);

    auto invoices = db->GetInvoices();
    for (auto invoice : invoices) {
        QList<QString> data = {
            QString::fromStdString(invoice->GetId()),
            invoice->GetCreatedAt().toString("dd/MM/yyyy"),
            QString::number(invoice->Total()),
        };
        addRow(ui->tblListInvoice, data);
    }
}

void MainWindow::fillOutProductTable()
{
    ui->tblProducts->setRowCount(0);

    auto list = db->GetProduct("");
    for (auto pair : list)
    {
        QList<QString> data = {
            QString::fromStdString(pair.second->GetId()),
            pair.second->GetName(),
            pair.second->GetBrand(),
            QString::number(pair.second->GetImportPrice()),
            QString::number(pair.second->GetSalePrice()),
            QString::number(pair.second->GetQuantity())
        };
        addRow(ui->tblProducts, data);
    }
}





// chuyển dữ liệu từ db->GetProduct("") sang QList<SelectedProduct>
QList<SelectedProduct> buildProductListFromDb(Database* db) {
    QList<SelectedProduct> list;
    auto map = db->GetProduct(""); // giả sử trả map<string, Product*>
    for (auto &pair : map) {
        auto prod = pair.second;
        SelectedProduct s;
        // Giả sử bạn có các hàm tương ứng trong Product:
        s.id = QString::fromStdString(prod->GetId());
        s.name = prod->GetName();
        s.brand = prod->GetBrand();
        s.importPrice = prod->GetImportPrice();
        s.salePrice = prod->GetSalePrice();
        s.stock = prod->GetQuantity();
        // xác định type từ id hoặc prod->GetType() nếu có
        // ví dụ DetermineType trả 0=Điện,1=Nước,2=Khác
        int t = DetermineType(prod->GetId());
        if (t == 0) s.type = "Điện";
        else if (t == 1) s.type = "Nước";
        else s.type = "Khác";

        list.append(s);
    }
    return list;
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
            fillOutProductTable();

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
            fillOutProductTable();

            setWindowModified(true); // NEW: Đánh dấu cửa sổ đã thay đổi
            //updateStatusBar(); // NEW: Cập nhật trạng thái thanh trạng thái

        } else {
            QMessageBox::warning(this, "Thất bại", "Chỉnh sửa sản phẩm thất bại\n"
                                                   "hãy kiểm tra lại thông tin sản phẩm hoặc\n"
                                                   "xem sản phẩm đã tồn tại trước đó chưa?");
        }
    }
}

void MainWindow::on_btnImport_clicked()
{
    SelectionDialog dlg(this);
    QList<SelectedProduct> all = buildProductListFromDb(db);
    dlg.loadData(all);
    if (dlg.exec() == QDialog::Accepted) {
        auto sel = dlg.getSelections();
        for (const auto &p : sel) {
            qDebug() << "Selected:" << p.id << p.name << "qty=" << p.quantity;\
            Command* cmd = new ImportCommand(p.id.toStdString(), p.quantity);
            cmdManager->ExecuteCommand(cmd);
        }

        // Bổ sung: Cập nhật lại bảng sản phẩm sau khi thêm thành công
        fillOutProductTable();

        setWindowModified(true); // NEW: Đánh dấu cửa sổ đã thay đổi
        //updateStatusBar(); // NEW: Cập nhật trạng thái thanh trạng thái
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

void MainWindow::on_btnExport_2_clicked()
{
    SelectionDialog dlg(this);
    QList<SelectedProduct> all = buildProductListFromDb(db);
    dlg.loadData(all);
    if (dlg.exec() == QDialog::Accepted) {
        auto sel = dlg.getSelections();
        for (const auto &p : sel) {
            qDebug() << "Selected:" << p.id << p.name << "qty=" << p.quantity;
            QList<QString> data = {
                p.id,
                QString::number(p.salePrice),
                QString::number(p.quantity),
                QString::number(p.salePrice * p.quantity)
            };
            addRow(ui->tblFormInvoice, data);
        }

        setWindowModified(true); // NEW: Đánh dấu cửa sổ đã thay đổi
        //updateStatusBar(); // NEW: Cập nhật trạng thái thanh trạng thái
    }

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
    fillOutProductTable();
    fillOutInvoiceTable();
}



