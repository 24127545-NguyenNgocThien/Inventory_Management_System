#include "addproduct.h"
#include "ui_addproduct.h"

#include <QMessageBox>
#include <QDebug>
#include <QString>

AddProduct::AddProduct(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AddProduct)
{
    ui->setupUi(this);

    ui->formLayoutWidget->hide();
    ui->formLayoutWidget_2->hide();
    ui->formLayoutWidget_3->hide();
    ui->formLayoutWidget_4->hide();
    ui->btn_addProduct->hide();
}

AddProduct::~AddProduct()
{
    delete ui;
}

//settup ẩn hiện các mục để điền thông tin đầu vào tùy vào type

void AddProduct::on_cbb_type_activated(int index)
{
    if(index != 0)
    {
        ui->formLayoutWidget->show();
        ui->btn_addProduct->show();
    }

    switch(index)
    {
    case 1:
        ui->formLayoutWidget_2->show();
        ui->formLayoutWidget_3->show();
        ui->formLayoutWidget_4->hide();
        break;
    case 2:
        ui->formLayoutWidget_2->hide();
        ui->formLayoutWidget_3->show();
        ui->formLayoutWidget_4->show();
        break;
    case 3:
        ui->formLayoutWidget_2->hide();
        ui->formLayoutWidget_3->hide();
        ui->formLayoutWidget_4->hide();
        break;
    default:
        ui->formLayoutWidget->hide();
        ui->formLayoutWidget_2->hide();
        ui->formLayoutWidget_3->hide();
        ui->formLayoutWidget_4->hide();
        ui->btn_addProduct->hide();
        break;
    }
}


ProductType AddProduct::GetCurrType()
{
    switch(ui->cbb_type->currentIndex())
    {
        case 1:
            return ProductType::Electric;
        case 2:
            return ProductType::Water;
        case 3:
            return ProductType::Other;
        default:
            throw "Error!";
    }
}


//slot nút ADD

void AddProduct::on_btn_addProduct_clicked()
{
    //kiểm tra thông tin đầu vào
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
            GetCurrType(),
            ui->le_name->text().toStdString(),
            ui->le_brand->text().toStdString(),
            ui->le_quantity->text().toInt(),
            ui->le_price->text().toDouble(),
            ui->le_extra1->text().toStdString(),
            ui->le_extra2->text().toDouble(),
            ui->le_extra3->text().toDouble()
            ))
    {
        qDebug() << "AddProduct() Successed";
        QMessageBox::information(this, "Thành công", "Thêm sản phẩm thành công");
    }
    else
    {
        qDebug() << "AddProduct() Failed: Sản phẩm cùng loại, tên và hãng đã tồn tại!";
        QMessageBox::critical(this, "Lỗi", "Không thể thêm sản phẩm");
    }

    emit productAdded();
    this->close();
}


