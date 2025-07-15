#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "app.h"

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_menu_clicked();

    void on_btn_addPage_clicked();
    void on_cbb_type_activated(int index);
    void on_btn_addProduct_clicked();

    void on_btn_removePage_clicked();

    void on_btn_showPage_clicked();
    
    void on_cbb_type_2_activated(int index);

    void Display(std::map<std::string, std::shared_ptr<Product>> list);
    
    void on_le_search_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    bool sidebarStatus;
    std::vector<std::string> checked;
};
#endif // MAINWINDOW_H
