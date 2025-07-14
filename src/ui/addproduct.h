#ifndef ADDPRODUCT_H
#define ADDPRODUCT_H

#include "app.h"

#include <QMainWindow>

namespace Ui {
class AddProduct;
}

class AddProduct : public QMainWindow
{
    Q_OBJECT

public:
    explicit AddProduct(QWidget *parent = nullptr);
    ~AddProduct();

signals:
    void productAdded();

private slots:
    void on_btn_addProduct_clicked();
    void on_cbb_type_activated(int index);

private:
    Ui::AddProduct *ui;
    ProductType GetCurrType();

};

#endif // ADDPRODUCT_H
