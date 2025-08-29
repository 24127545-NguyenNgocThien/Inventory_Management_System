#include "notify.h"

void Notify::Info(QWidget* parent, const QString& message, const QString& title)
{
    QMessageBox::information(parent, title, message);
}

void Notify::Warning(QWidget* parent, const QString& message, const QString& title)
{
    QMessageBox::warning(parent, title, message);
}

void Notify::Error(QWidget* parent, const QString& message, const QString& title)
{
    QMessageBox::critical(parent, title, message);
}

bool Notify::Confirm(QWidget* parent, const QString& message, const QString& title)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(parent, title, message,
                                  QMessageBox::Yes | QMessageBox::No);
    return (reply == QMessageBox::Yes);
}

void Notify::LowStockAlert(QWidget* parent, const QString& productName, int quantity, int threshold)
{
    if(quantity <= threshold)
    {
        QString msg = QString("Sản phẩm \"%1\" hiện chỉ còn %2 trong kho.\nHãy xem xét bổ sung.").arg(productName).arg(quantity);
        QMessageBox::warning(parent, "Thông báo", msg);
    }
}
