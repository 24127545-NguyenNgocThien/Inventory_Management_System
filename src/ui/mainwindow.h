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

private slots:

    //thao tác với menubar (ẩn/hiện/chuyển trang)
    void on_btn_menu_clicked();
    void on_btn_addPage_clicked();
    void on_btn_removePage_clicked();
    void on_btn_showPage_clicked();


    //hỗ trợ thêm sản phẩm
    void on_cbb_type_activated(int index);
    void on_btn_addProduct_clicked();

    //xóa sản phẩm đc chọn
    void on_btn_confirm_clicked();
    void on_btn_cancel_clicked();

    //thanh tìm kiếm
    void on_le_search_textChanged(const QString &arg1);

    //hỗ trợ show sp theo loại
    void on_cbb_type_2_activated(int index);

    //hỗ trợ chọn sp khi muốn delete / edit
    void on_tbw_itemDoubleClicked(QTableWidgetItem *item);

    //hỗ trợ edit thông tin sp
    void on_btn_modify_clicked();
    void on_btn_outModify_clicked();

private:
    Ui::MainWindow *ui;
    CommandManager* cmdManager;
    Database* db;
    bool sidebarStatus;
    std::vector<std::string> productIds;

protected:
    void closeEvent(QCloseEvent *event) override;
    bool MaybeSave();

    void Display(std::map<std::string, std::shared_ptr<Product>> list);
};
#endif // MAINWINDOW_H
