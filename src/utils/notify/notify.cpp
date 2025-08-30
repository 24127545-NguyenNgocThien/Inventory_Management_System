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

void Notify::LowStockAlert(QWidget* parent, const std::map<std::string, std::shared_ptr<Product>>& products, int threshold)
{
    QStringList lowStockList;
    for (const auto& [id, product] : products)
    {
        if (product->GetQuantity() <= threshold)
        {
            QString line = QString("%1 (Còn: %2 vật phẩm)")
                               .arg(product->GetName())
                               .arg(product->GetQuantity());
            lowStockList.append(line);
        }
    }

    if (lowStockList.isEmpty()) return;

    if (lowStockList.size() == 1)
    {
        QMessageBox::warning(parent, "Thông báo",
                             QString("Sản phẩm còn ít hàng:\n\n%1").arg(lowStockList.first()));
    }
    else
    {
        QString message = "Các sản phẩm sau còn ít hàng:\n\n" + lowStockList.join("\n");
        QMessageBox::warning(parent, "Thông báo", message);
    }
}
