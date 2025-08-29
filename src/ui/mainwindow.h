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
    void refreshTables();
    ~MainWindow();

private slots:

    void on_txtSearch_textChanged(const QString &arg1);
    void on_btnAdd_clicked();
    void on_btnDelete_clicked();
    void on_btnEdit_clicked();
    void on_btnImport_clicked();

    void on_btnExport_2_clicked();
    void on_btnDelete_2_clicked();
    void on_btnSave_2_clicked();

    void on_tblListInvoice_cellDoubleClicked(int row, int column);
private:
    Ui::MainWindow *ui;
    Database* db;
    CommandManager* cmdManager;

    void addRow(QTableWidget* table, QList<QString> data);
    void fillOutInvoiceTable();
    void fillOutProductTable();
    void updateStatusBar();

protected:
    void closeEvent(QCloseEvent *event) override;
    bool MaybeSave();

};
#endif // MAINWINDOW_H
