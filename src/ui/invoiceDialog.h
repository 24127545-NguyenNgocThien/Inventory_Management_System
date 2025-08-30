#ifndef INVOICEDIALOG_H
#define INVOICEDIALOG_H

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "invoice.h"

class MainWindow;

class InvoiceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InvoiceDialog(Invoice* inv, QWidget* parent = nullptr);

signals:
    void invoiceReturned();

private slots:
    void onReturnClicked();
    void onCloseClicked();

private:
    Invoice* invoice;

    QTableWidget* tblItems;
    QLabel* lblTotal;
    QPushButton* btnClose;
    QPushButton* btnReturn;

    void fillTable();
};

#endif // INVOICEDIALOG_H
