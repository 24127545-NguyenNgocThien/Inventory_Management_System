#include "invoiceDialog.h"
#include "data.h"
#include "notify.h"
#include "mainwindow.h"
#include "QHeaderView"

InvoiceDialog::InvoiceDialog(Invoice* inv, QWidget* parent)
    : QDialog(parent), invoice(inv)
{

    setWindowTitle(QString("Chi tiết hóa đơn: %1")
                       .arg(QString::fromStdString(inv ? inv->GetId() : "")));
    resize(700, 420);

    setModal(true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Bảng sản phẩm
    tblItems = new QTableWidget(this);
    tblItems->setColumnCount(4);
    tblItems->setHorizontalHeaderLabels({"ID", "Đơn giá", "Số lượng", "Thành tiền"});
    tblItems->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tblItems->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tblItems->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblItems->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(tblItems);

    // Tổng tiền
    lblTotal = new QLabel(this);
    mainLayout->addWidget(lblTotal);

    // Nút bấm
    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnReturn = new QPushButton("Hoàn trả", this);
    btnClose  = new QPushButton("Đóng", this);
    btnLayout->addStretch();
    btnLayout->addWidget(btnReturn);
    btnLayout->addWidget(btnClose);
    mainLayout->addLayout(btnLayout);

    connect(btnReturn, &QPushButton::clicked, this, &InvoiceDialog::onReturnClicked);
    connect(btnClose,  &QPushButton::clicked, this, &InvoiceDialog::onCloseClicked);

    fillTable();
}

void InvoiceDialog::fillTable()
{
    tblItems->setRowCount(0);
    double total = 0.0;

    if (!invoice) {
        lblTotal->setText("Tổng cộng: 0");
        return;
    }

    for(const auto& item : invoice->GetItems())
    {
        int row = tblItems->rowCount();
        tblItems->insertRow(row);

        tblItems->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(item.productId)));
        tblItems->setItem(row, 1, new QTableWidgetItem(QString::number(item.salePrice)));
        tblItems->setItem(row, 2, new QTableWidgetItem(QString::number(item.quantity)));
        tblItems->setItem(row, 3, new QTableWidgetItem(QString::number(item.salePrice * item.quantity)));

        total += item.salePrice * item.quantity;
    }

    lblTotal->setText(QString("Tổng cộng: %1").arg(QString::number(total)));
}

void InvoiceDialog::onReturnClicked()
{
    if (!invoice) {
        Notify::Error(this, "Không tìm thấy hóa đơn!", "Lỗi");
        return;
    }

    if (Database::GetInstance().ReturnInvoice(invoice->GetId()))
    {
        Notify::Info(this, "Hoàn trả hóa đơn thành công!", "Thành công");
        emit invoiceReturned();
        accept();
    }
    else
    {
        Notify::Error(this, "Hoàn trả thất bại hoặc hóa đơn không tồn tại!", "Lỗi");
    }
}

void InvoiceDialog::onCloseClicked()
{
    reject();
}
