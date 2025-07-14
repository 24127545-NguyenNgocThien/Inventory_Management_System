#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "app.h"

#include <QMainWindow>
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void Display(std::map<std::string, std::shared_ptr<Product>> list, const std::vector<int> hideCols);

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void on_act_add_2_triggered();
    void on_act_search_triggered();
    // void on_act_remove_triggered();
    // void on_tbw_display_doubleclicked(int row, int col)

    void on_act_showAll_triggered();
    void on_act_showElec_triggered();
    void on_act_showWater_triggered();
    void on_act_showOther_triggered();

    // void onCheckboxChanged(QTableWidgetItem *item);
    // void on_btn_confirm_clicked();
    // void on_btn_cancel_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<std::string> checked;
};
#endif // MAINWINDOW_H
