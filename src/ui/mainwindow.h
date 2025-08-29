#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commandmanager.h"
#include "data.h"
#include <QTableWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Database& data, QWidget *parent = nullptr);
    ~MainWindow();

    void addProductRow(const std::string& id, const QString& name, const QString& brand,
                       double importPrice, double salePrice, int quantity);

private slots:

    void on_txtSearch_textChanged(const QString &arg1);

    void on_btnAdd_clicked();

    void on_btnDelete_clicked();

    void on_btnEdit_clicked();

    void on_btnAdd_2_clicked();

    void on_btnDelete_2_clicked();

    void on_btnSave_2_clicked();

private:
    Ui::MainWindow *ui;
    Database* db;
    CommandManager* cmdManager;

    void updateStatusBar();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool MaybeSave();

};
#endif // MAINWINDOW_H
