#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QMessageBox>
#include <QString>
#include <QTableWidgetItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , sidebarStatus(true)
{
    ui->setupUi(this);
    ui->sidebar->setHidden(sidebarStatus);
    ui->stackedWidget->setCurrentIndex(0);
    ui->tbw->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_menu_clicked()
{
    sidebarStatus = !sidebarStatus;
    ui->sidebar->setHidden(sidebarStatus);
}

//---------------------Settup for Page 1------------------------
void MainWindow::on_btn_addPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    on_cbb_type_activated(0);
}

void MainWindow::on_cbb_type_activated(int index)
{
    switch(index)
    {
    case 1:
        ui->lbl_extra1->show();
        ui->lbl_extra1->setText("Tiêu chuẩn an toàn :");
        ui->le_extra1->show();
        ui->lbl_extra2->show();
        ui->lbl_extra2->setText("Oát :");
        ui->le_extra2->show();
        ui->lbl_extra3->show();
        ui->le_extra3->show();
        break;
    case 2:
        ui->lbl_extra1->show();
        ui->lbl_extra1->setText("Chất liệu :");
        ui->lbl_extra2->show();
        ui->lbl_extra2->setText("Độ chịu nhiệt :");
        ui->lbl_extra3->hide();
        ui->le_extra3->hide();
        break;
    default:
        ui->lbl_extra1->hide();
        ui->le_extra1->hide();
        ui->lbl_extra2->hide();
        ui->le_extra2->hide();
        ui->lbl_extra3->hide();
        ui->le_extra3->hide();
        break;
    }

    if(index == 0)
    {
        ui->btn_addProduct->hide();
    }
    else
    {
        ui->btn_addProduct->show();
    }
}

//      LOGIC THÊM SẢN PHẨM MỚI
void MainWindow::on_btn_addProduct_clicked()
{
    //kiểm tra thông tin đầu vào
    ProductType type;
    switch(ui->cbb_type->currentIndex())
    {
    case 1:
        type = ProductType::Electric;
        break;
    case 2:
        type = ProductType::Water;
        break;
    case 3:
        type = ProductType::Other;
        break;
    default:
        throw "Error!";
    }

    if(ui->le_name->text().isEmpty() || ui->le_brand->text().isEmpty())
    {
        QMessageBox::warning(this, "Lỗi", "Tên và hãng không được trống");
        return;
    }

    bool ok;
    int quantity = ui->le_quantity->text().toInt(&ok);
    if(!ok || quantity <= 0)
    {
        qDebug() << "Số lượng không hợp lệ";
        ui->le_quantity->clear();
        return;
    }

    double price = ui->le_price->text().toDouble(&ok);
    if(!ok || price <= 0)
    {
        qDebug() << "Giá tiền không hợp lệ";
        ui->le_price->clear();
        return;
    }

    // Gọi hàm thêm sản phẩm
    if(App::GetInstance().AddProduct(
            type,
            ui->le_name->text(),
            ui->le_brand->text(),
            ui->le_quantity->text().toInt(),
            ui->le_price->text().toDouble(),
            ui->le_extra1->text(),
            ui->le_extra2->text().toDouble(),
            ui->le_extra3->text().toDouble()
            ))
    {
        QMessageBox::information(this, "Thành công", "Thêm sản phẩm thành công");
    }
    else
    {
        QMessageBox::critical(this, "Lỗi", "Không thể thêm sản phẩm");
    }

    //Xoá nội dung cũ đã nhập
    ui->cbb_type->setCurrentIndex(0);
    ui->le_name->clear();
    ui->le_brand->clear();
    ui->le_quantity->clear();
    ui->le_price->clear();
    ui->le_extra1->clear();
    ui->le_extra2->clear();
    ui->le_extra3->clear();
}


//=========================================================================

void MainWindow::on_btn_removePage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lbl_title->setText("Remove Product");
    ui->cbb_type_2->hide();
    ui->btn_confirm->show();
    ui->btn_cancel->show();
    on_cbb_type_2_activated(0);
    ui->tbw->setColumnHidden(0, false);
}



void MainWindow::on_btn_showPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lbl_title->setText("Show Product by");
    ui->cbb_type_2->show();
    ui->btn_confirm->hide();
    ui->btn_cancel->hide();
    on_cbb_type_2_activated(0);
    ui->tbw->setColumnHidden(0, true);
}

//------------------------hiển thị danh sách sản phẩm cho trước-----------------------------
void MainWindow::Display(std::map<std::string, std::shared_ptr<Product>> list)
{
    ui->tbw->clearContents();
    int numRows = list.size();
    ui->tbw->setRowCount(numRows);
    
    int row = 0;
    for(const auto& pair : list)
    {
        int col = 0;
        QTableWidgetItem *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable); // cho phép tích
        checkItem->setCheckState(Qt::Unchecked);  // mặc định chưa được tích

        ui->tbw->setItem(row, col++, checkItem);
        ui->tbw->setItem(row, col++, new QTableWidgetItem(pair.first.c_str()));
        ui->tbw->setItem(row, col++, new QTableWidgetItem(pair.second->GetName()));
        ui->tbw->setItem(row, col++, new QTableWidgetItem(pair.second->GetBrand()));
        ui->tbw->setItem(row, col++, new QTableWidgetItem(QString::number(pair.second->GetQuantity())));
        ui->tbw->setItem(row, col, new QTableWidgetItem(QString::number(pair.second->GetPrice())));
        row++;
    }
}


void MainWindow::on_cbb_type_2_activated(int index)
{
    switch(index)
    {
    case 1:
        on_le_search_textChanged("E");
        break;
    case 2:
        on_le_search_textChanged("W");
        break;
    case 3:
        on_le_search_textChanged("O");
        break;
    default:
        on_le_search_textChanged("");
        break;
    }
}


void MainWindow::on_le_search_textChanged(const QString &arg1)
{
    std::map<std::string, std::shared_ptr<Product>> list
        = App::GetInstance().GetProductById(arg1.toStdString());

    Display(list);
}

