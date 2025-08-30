#include "mainwindow.h"
#include "productDialog.h"
#include "selectionDialog.h"
#include "invoiceDialog.h"
#include "./ui_mainwindow.h"
#include "otherutils.h"
#include "addcommand.h"
#include "deletecommand.h"
#include "editcommand.h"
#include "importCommand.h"
#include "invoice.h"
#include "invoiceService.h"
#include <QCloseEvent>
#include <QLabel>
#include "notify.h"
#include <QPalette>
#include <QStyleFactory>
#include "report.h"
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QLegendMarker>
#include <QPen>

MainWindow::MainWindow(Database& data, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cmdManager(new CommandManager())
    , db(&data)
    , lblCount(new QLabel(this))
{
    db->Load();
    db->LoadInvoices();
    ui->setupUi(this);
    ui->statusBar->addPermanentWidget(lblCount);

    // Đặt ngày mặc định = 6 tháng gần nhất
    QDate today = QDate::currentDate();
    QDate sixMonthsAgo = today.addMonths(-6);
    ui->dateFrom->setDate(sixMonthsAgo);
    ui->dateTo->setDate(today);

    ui->lblRevenue->setText("0 VNĐ");
    ui->lblProfit->setText("0 VNĐ");
    ui->lblInvoices->setText("0");
    ui->lblQuantity->setText("0");
    ui->lblBestSeller->setText("Không có dữ liệu");

    // Gọi ngay để vẽ chart ban đầu
    on_btnView_clicked();

    setWindowTitle("MainWindow[*]");
    qApp->setStyle(QStyleFactory::create("Fusion"));
    ui->tblProducts->setAlternatingRowColors(true);
    ui->tblFormInvoice->setAlternatingRowColors(true);
    ui->tblListInvoice->setAlternatingRowColors(true);

    // Liên kết các acion trên menubar
    connect(ui->actionUndo, &QAction::triggered, [this]() {
        cmdManager->Undo();
        refreshTables();
    });
    ui->actionUndo->setShortcut(QKeySequence::Undo);
    connect(ui->actionRedo, &QAction::triggered, [this]() {
        cmdManager->Redo();
        refreshTables();
    });
    ui->actionRedo->setShortcut(QKeySequence::Redo);
    connect(ui->actionAdd_product, &QAction::triggered, [this]() {
        on_btnAdd_clicked();
    });
    connect(ui->actionDelete_product, &QAction::triggered, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
        on_btnDelete_clicked();
    });
    connect(ui->actionModify_product, &QAction::triggered, [this]() {
        ui->stackedWidget->setCurrentIndex(0);
        on_btnEdit_clicked();
    });
    connect(ui->actionImport_product, &QAction::triggered, [this]() {
        on_btnImport_clicked();
    });
    connect(ui->actionNew_invoice, &QAction::triggered, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
        ui->tabWidget->setCurrentIndex(0);
        on_btnExport_2_clicked();
    });
    connect(ui->actionView_invoices, &QAction::triggered, [this]() {
        ui->stackedWidget->setCurrentIndex(1);
        ui->tabWidget->setCurrentIndex(1);
    });

    // Tạo và thêm label mới
    QLabel *statusLabel = new QLabel("Trạng thái: Sẵn sàng", this);
    ui->statusBar->addWidget(statusLabel);

    // Tạo action thường
    QAction *toggleAction = new QAction("☰ Sidebar", this);
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


void MainWindow::refreshTables(){
    fillOutProductTable();
    fillOutInvoiceTable();
}


//===============================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isWindowModified()) {
        if (Notify::Confirm(this, "Bạn có muốn lưu thay đổi?", "Xác nhận")) {
            if (!db->Save() || !db->SaveInvoices()) {
                event->ignore();  // Không đóng nếu lưu thất bại
                return;
            }
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

    lblCount->setText(QString("Có %1 loại sản phẩm trong kho").arg(db->GetNumberProduct()));
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
            Notify::Info(this, "Thêm sản phẩm thành công! Hãy làm mới trang", "Thành công");
            fillOutProductTable();
            setWindowModified(true);
        } else {
            Notify::Warning(this, "Thêm sản phẩm thất bại.\n"
                                  "Hãy kiểm tra lại thông tin sản phẩm hoặc\n"
                                  "xem sản phẩm đã tồn tại trước đó chưa?", "Thất bại");
        }
    }
}

void MainWindow::on_btnDelete_clicked()
{
    auto selected = ui->tblProducts->currentRow();
    if (selected < 0) {
        Notify::Warning(this, "Hãy chọn sản phẩm để xóa!", "Lỗi");
        return;
    }

    if (!Notify::Confirm(this, "Bạn có chắc muốn xóa sản phẩm này?", "Xác nhận")) {
        return;
    }

    // Lấy item ở cột đầu tiên (cột id)
    QTableWidgetItem *firstItem = ui->tblProducts->item(selected, 0);
    std::string id = firstItem->text().toStdString();

    // Xoá sản phẩm
    Command* cmd = new DeleteCommand(id);
    if (cmdManager->ExecuteCommand(cmd)) {
        Notify::Info(this, "Xóa sản phẩm thành công!", "Thành công");
        ui->tblProducts->removeRow(selected);
        lblCount->setText(QString("Có %1 loại sản phẩm trong kho").arg(db->GetNumberProduct()));
        setWindowModified(true);
    } else {
        Notify::Error(this, "Không thể xóa sản phẩm!", "Lỗi");
    }

}

void MainWindow::on_btnEdit_clicked()
{
    auto selected = ui->tblProducts->currentRow();
    if (selected < 0) {
        Notify::Warning(this, "Hãy chọn sản phẩm để sửa!", "Lỗi");
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
            Notify::Info(this, "Chỉnh sửa sản phẩm thành công! Hãy làm mới trang", "Thành công");
            fillOutProductTable();
            setWindowModified(true);
        } else {
            Notify::Warning(this, "Chỉnh sửa sản phẩm thất bại.\n"
                                  "Hãy kiểm tra lại thông tin sản phẩm hoặc\n"
                                  "xem sản phẩm đã tồn tại trước đó chưa?", "Thất bại");
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
        bool allOk = true;
        for (const auto &p : sel) {
            qDebug() << "Selected:" << p.id << p.name << "qty=" << p.quantity;
            Command* cmd = new ImportCommand(p.id.toStdString(), p.quantity);
            if (!cmdManager->ExecuteCommand(cmd)) {
                allOk = false;
            }
        }

        if (allOk) {
            Notify::Info(this, "Nhập kho thành công!", "Thành công");
        } else {
            Notify::Warning(this, "Một số sản phẩm không thể nhập kho!", "Cảnh báo");
        }

        fillOutProductTable();
        setWindowModified(true);
    }
}

void MainWindow::on_btnView_clicked()
{

    auto& db = Database::GetInstance();

    QDate from = ui->dateFrom->date();
    QDate to   = ui->dateTo->date();
    if (to < from) std::swap(from, to);

    auto invoices = db.GetInvoicesInRange(from, to); // vector<Invoice*>

    if (invoices.empty()) {
        // ---- Chart rỗng với baseline ----
        QChart* chart = new QChart();
        chart->setTitle("Doanh thu và Lợi nhuận theo thời gian (Không có dữ liệu)");
        chart->legend()->setAlignment(Qt::AlignTop);

        // Trục X (trống)
        QBarCategoryAxis* axisX = new QBarCategoryAxis();
        axisX->setTitleText("Thời gian");
        chart->addAxis(axisX, Qt::AlignBottom);

        // Trục Y (chia mốc 100, có số 0)
        QValueAxis* axisY = new QValueAxis();
        axisY->setRange(-100, 100);
        axisY->setTickCount(5);
        axisY->setLabelFormat("%d");
        axisY->setTitleText("Số tiền (đ)");
        chart->addAxis(axisY, Qt::AlignLeft);

        // Vẽ baseline y = 0
        QLineSeries* zeroLine = new QLineSeries();
        zeroLine->append(-0.5, 0);
        zeroLine->append(0.5, 0);

        QPen zeroPen(Qt::gray);
        zeroPen.setWidthF(1.0);
        zeroLine->setPen(zeroPen);

        chart->addSeries(zeroLine);
        zeroLine->attachAxis(axisX);
        zeroLine->attachAxis(axisY);

        chart->legend()->markers(zeroLine).first()->setVisible(false);

        // Clear chart cũ
        QLayoutItem* item;
        while ((item = ui->chartLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        ui->lblRevenue->setText("0 đ");
        ui->lblProfit->setText("0 đ");
        ui->lblInvoices->setText("0");
        ui->lblQuantity->setText("0");
        ui->lblBestSeller->setText("Không có dữ liệu");

        QChartView* chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        ui->chartLayout->addWidget(chartView);
        return;
    }

    int totalDays = from.daysTo(to);

    // ---- Chuẩn bị dữ liệu ----
    QStringList categories;
    QBarSet* revenueSet = new QBarSet("Doanh thu");
    QBarSet* profitSet  = new QBarSet("Lợi nhuận");

    revenueSet->setColor(Qt::blue);   // màu xanh cho doanh thu
    profitSet->setColor(Qt::red);     // màu đỏ cho lợi nhuận

    if (from.addYears(3) < to) {
        // ====== GỘP THEO NĂM ======
        QMap<int, double> revenueByYear, profitByYear;

        for (auto inv : invoices) {
            int year = inv->GetCreatedAt().date().year();
            revenueByYear[year] += inv->Total();
            for (auto& it : inv->GetItems()) {
                auto prodMap = db.GetProduct(it.productId);
                auto found = prodMap.find(it.productId);
                if (found != prodMap.end() && found->second) {
                    double importPrice = found->second->GetImportPrice();
                    profitByYear[year] += (it.salePrice - importPrice) * it.quantity;
                }
            }
        }

        for (auto year : revenueByYear.keys()) {
            *revenueSet << revenueByYear[year];
            *profitSet  << profitByYear[year];
            categories << QString::number(year);
        }
    }
    else if (totalDays > 31) {
        // ====== GỘP THEO THÁNG ======
        QMap<QString, double> revenueByMonth, profitByMonth;

        for (auto inv : invoices) {
            QDate d = inv->GetCreatedAt().date();
            QString key = QString("%1/%2").arg(d.month()).arg(d.year()); // "MM/YYYY"
            revenueByMonth[key] += inv->Total();
            for (auto& it : inv->GetItems()) {
                auto prodMap = db.GetProduct(it.productId);
                auto found = prodMap.find(it.productId);
                if (found != prodMap.end() && found->second) {
                    double importPrice = found->second->GetImportPrice();
                    profitByMonth[key] += (it.salePrice - importPrice) * it.quantity;
                }
            }
        }

        for (auto key : revenueByMonth.keys()) {
            *revenueSet << revenueByMonth[key];
            *profitSet  << profitByMonth[key];
            categories << key;
        }
    }
    else {
        // ====== CHI TIẾT THEO NGÀY ======
        QMap<QDate, double> revenueByDay, profitByDay;
        for (QDate d = from; d <= to; d = d.addDays(1)) {
            revenueByDay[d] = 0.0;
            profitByDay[d]  = 0.0;
        }

        for (auto inv : invoices) {
            QDate d = inv->GetCreatedAt().date();
            if (d < from || d > to) continue;
            revenueByDay[d] += inv->Total();
            for (auto& it : inv->GetItems()) {
                auto prodMap = db.GetProduct(it.productId);
                auto found = prodMap.find(it.productId);
                if (found != prodMap.end() && found->second) {
                    double importPrice = found->second->GetImportPrice();
                    profitByDay[d] += (it.salePrice - importPrice) * it.quantity;
                }
            }
        }

        for (QDate d = from; d <= to; d = d.addDays(1)) {
            *revenueSet << revenueByDay[d];
            *profitSet  << profitByDay[d];
            categories << d.toString("dd/MM");
        }
    }

    // ---- Tạo chart ----
    QBarSeries* series = new QBarSeries();
    series->append(revenueSet);
    series->append(profitSet);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Doanh thu và Lợi nhuận theo thời gian");
    chart->legend()->setAlignment(Qt::AlignTop);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Thời gian");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // ---- Trục Y theo dữ liệu ----
    double minVal = 0, maxVal = 0;
    bool first = true;

    for (int i = 0; i < revenueSet->count(); i++) {
        double r = (*revenueSet)[i];
        double p = (*profitSet)[i];
        if (first) { minVal = maxVal = r; first = false; }
        minVal = std::min({minVal, r, p});
        maxVal = std::max({maxVal, r, p});
    }

    // đảm bảo có 0
    minVal = std::min(minVal, 0.0);
    maxVal = std::max(maxVal, 0.0);

    // chọn step
    double range = maxVal - minVal;
    double step = 50.0;  // bắt đầu từ 50
    while ((range / step) > 12) step *= 2;  // tối đa ~12 tick

    auto roundDown = [](double v, double step){ return std::floor(v / step) * step; };
    auto roundUp   = [](double v, double step){ return std::ceil(v / step) * step; };

    double yMin = roundDown(minVal, step);
    double yMax = roundUp(maxVal, step);

    // số tick = (range / step) + 1
    int tickCount = static_cast<int>((yMax - yMin) / step) + 1;

    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(yMin, yMax);
    axisY->setTickInterval(step);
    axisY->setTickCount(tickCount);
    axisY->setLabelFormat("%.0f");
    axisY->setTitleText("Số tiền (đ)");

    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // ---- Thay chart cũ ----
    QLayoutItem* item;
    while ((item = ui->chartLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(chartView);

    // ---- Cập nhật chỉ số bên trái ----
    double totalRevenue = 0.0, totalProfit = 0.0;
    int invoiceCount = static_cast<int>(invoices.size());
    int totalQty = 0;

    for (auto inv : invoices) {
        totalRevenue += inv->Total();
        for (auto& it : inv->GetItems()) {
            totalQty += it.quantity;
            auto prodMap = db.GetProduct(it.productId);
            auto found = prodMap.find(it.productId);
            if (found != prodMap.end() && found->second) {
                double importPrice = found->second->GetImportPrice();
                totalProfit += (it.salePrice - importPrice) * it.quantity;
            }
        }
    }

    QLocale vi(QLocale::Vietnamese, QLocale::Vietnam);
    ui->lblRevenue->setText(vi.toString(totalRevenue, 'f', 0) + " đ");
    ui->lblProfit->setText(vi.toString(totalProfit,  'f', 0) + " đ");
    ui->lblInvoices->setText(QString::number(invoiceCount));
    ui->lblQuantity->setText(QString::number(totalQty));

    std::map<std::string,int> sold;
    for (auto inv : invoices)
        for (const auto& it : inv->GetItems())
            sold[it.productId] += it.quantity;

    std::pair<std::string,int> best{"", 0};
    for (const auto& kv : sold)
        if (kv.second > best.second) best = kv;

    if (best.second > 0)
        ui->lblBestSeller->setText(QString::fromStdString(best.first) +
                                   " (" + QString::number(best.second) + ")");
    else
        ui->lblBestSeller->setText("Không có dữ liệu");
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
    SelectionDialog dlg(this, true);
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

        fillOutProductTable();
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
    if (ui->tblFormInvoice->rowCount() == 0) return;

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

    if (InvoiceService::CreateInvoice(items, cmdManager)) {
        Notify::Info(this, "Lưu hóa đơn thành công!", "Thành công");
        Notify::LowStockAlert(this, db->GetProduct(""), 5);
    }
    else
    {
        Notify::Error(this, "Lưu hóa đơn thất bại! Kiểm tra lại tồn kho.", "Lỗi");
    }

    setWindowModified(true); // NEW: Đánh dấu cửa sổ đã thay đổi
    //updateStatusBar(); // NEW: Cập nhật trạng thái thanh trạng thái

    ui->tblFormInvoice->clearContents();
    ui->tblFormInvoice->setRowCount(0);
    fillOutProductTable();
    fillOutInvoiceTable();
}

void MainWindow::on_tblListInvoice_cellDoubleClicked(int row, int column)
{
    if(row < 0 || row >= (int)db->GetInvoices().size()) return;

    Invoice* inv = db->GetInvoices()[row];
    auto dlg = new InvoiceDialog(inv, this);

    connect(dlg, &InvoiceDialog::invoiceReturned, this, &MainWindow::refreshTables);

    dlg->setAttribute(Qt::WA_DeleteOnClose);
    dlg->exec();
}
