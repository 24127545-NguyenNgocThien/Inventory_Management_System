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
