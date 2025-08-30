#ifndef NOTIFY_H
#define NOTIFY_H

#include <QString>
#include <QMessageBox>
#include <QWidget>
#include <map>
#include <memory>
#include "product.h"

class Notify
{
public:
    // Hiển thị thông báo thông tin
    static void Info(QWidget* parent, const QString& message, const QString& title = "Thông báo");

    // Hiển thị cảnh báo
    static void Warning(QWidget* parent, const QString& message, const QString& title = "Cảnh báo");

    // Hiển thị lỗi
    static void Error(QWidget* parent, const QString& message, const QString& title = "Lỗi");

    // Hiển thị hộp thoại xác nhận (Yes/No)
    static bool Confirm(QWidget* parent, const QString& message, const QString& title = "Xác nhận");

    // Thông báo tồn kho thấp
    static void LowStockAlert(QWidget* parent, const std::map<std::string, std::shared_ptr<Product>>& products, int threshold);
};

#endif // NOTIFY_H
