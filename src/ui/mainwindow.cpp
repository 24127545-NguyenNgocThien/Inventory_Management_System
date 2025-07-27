#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "otherutils.h"
#include "addcommand.h"
#include "deletecommand.h"
#include "editcommand.h"
#include <QMessageBox>
#include <QCloseEvent>

MainWindow::MainWindow(Database& data, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , cmdManager(new CommandManager())
    , db(&data)
    , sidebarStatus(true)
{
    db->Load();
    ui->setupUi(this);
    setWindowTitle("MainWindow[*]");
    ui->sidebar->setHidden(sidebarStatus);

    QAction* act_undo = new QAction("Undo", this);
    connect(act_undo, SIGNAL(triggered(bool)), this, SLOT(ActionUndo()));
    act_undo->setShortcut(QKeySequence::Undo);
    QAction* act_redo = new QAction("Redo", this);
    connect(act_redo, SIGNAL(triggered(bool)), this, SLOT(ActionRedo()));
    act_undo->setShortcut(QKeySequence::Redo);
    QAction* act_menu = new QAction("Menu", this);
    connect(act_menu, SIGNAL(triggered(bool)), this, SLOT(ActionMenu()));
    act_menu->setShortcut(QKeySequence("Ctrl+M"));
    ui->menuBar->addAction(act_undo);
    ui->menuBar->addAction(act_redo);
    ui->menuBar->addAction(act_menu);

    ui->stackedWidget->setCurrentIndex(0);
    ui->tbw->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeMode::Stretch);
    ui->tbw->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete cmdManager;
}

//===============================================================
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isWindowModified()) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
            this,
            "Xác nhận",
            "Bạn có muốn lưu thay đổi?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
            );

        if (reply == QMessageBox::Save) {
            if (!db->Save()) {
                event->ignore();  // Không đóng nếu lưu thất bại
                return;
            }
        }
        else if (reply == QMessageBox::Cancel) {
            event->ignore();  // Không đóng
            return;
        }
    }
    event->accept();  // Cho phép đóng
}

void MainWindow::Display(std::map<std::string, std::shared_ptr<Product>> list)
{
    ui->tbw->clearContents();
    int numRows = list.size();
    ui->tbw->setRowCount(numRows);

    int row = 0;
    for(const auto& pair : list)
    {
        int col = 0;
        QTableWidgetItem *checkItem = new QTableWidgetItem();
        checkItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable); // cho phép tích
        checkItem->setCheckState(Qt::Unchecked);  // mặc định chưa được tích

        ui->tbw->setItem(row, col++, checkItem);
        ui->tbw->setItem(row, col++, new QTableWidgetItem(pair.first.c_str()));
        ui->tbw->setItem(row, col++, new QTableWidgetItem(pair.second->GetName()));
        ui->tbw->setItem(row, col++, new QTableWidgetItem(pair.second->GetBrand()));
        ui->tbw->setItem(row, col++, new QTableWidgetItem(QString::number(pair.second->GetQuantity())));
        ui->tbw->setItem(row, col, new QTableWidgetItem(QString::number(pair.second->GetPrice())));
        row++;
    }
}
//===============================================================

void MainWindow::ActionUndo()
{
    if(cmdManager->Undo())
    {
        //hiện bảng log
    }
}

void MainWindow::ActionRedo()
{
    if(cmdManager->Redo())
    {
        //hiện bảng log
    }
}

void MainWindow::ActionMenu()
{
    sidebarStatus = !sidebarStatus;
    ui->sidebar->setHidden(sidebarStatus);
}

//===============================================================

void MainWindow::on_btn_addPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    on_cbb_type_activated(0);
}


void MainWindow::on_btn_removePage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lbl_title->setText("Remove Product");
    ui->cbb_type_2->hide();
    ui->btn_confirm->show();
    ui->btn_cancel->show();
    on_cbb_type_2_activated(0);
    ui->tbw->setColumnHidden(0, false);
}


void MainWindow::on_btn_showPage_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->lbl_title->setText("Show Product by");
    ui->cbb_type_2->show();
    ui->btn_confirm->hide();
    ui->btn_cancel->hide();
    on_cbb_type_2_activated(0);
    ui->tbw->setColumnHidden(0, true);
}


//=========================================================

void MainWindow::on_cbb_type_activated(int index)
{
    switch(index)
    {
    case 1:
        ui->lbl_extra1->show();
        ui->lbl_extra1->setText("Tiêu chuẩn an toàn :");
        ui->le_extra1->show();
        ui->lbl_extra2->show();
        ui->lbl_extra2->setText("Oát :");
        ui->le_extra2->show();
        ui->lbl_extra3->show();
        ui->le_extra3->show();
        break;
    case 2:
        ui->lbl_extra1->show();
        ui->lbl_extra1->setText("Chất liệu :");
        ui->le_extra1->show();
        ui->lbl_extra2->show();
        ui->lbl_extra2->setText("Độ chịu nhiệt :");
        ui->le_extra2->show();
        ui->lbl_extra3->hide();
        ui->le_extra3->hide();
        break;
    default:
        ui->lbl_extra1->hide();
        ui->le_extra1->hide();
        ui->lbl_extra2->hide();
        ui->le_extra2->hide();
        ui->lbl_extra3->hide();
        ui->le_extra3->hide();
        break;
    }

    if(index == 0)
    {
        ui->btn_addProduct->hide();
    }
    else
    {
        ui->btn_addProduct->show();
    }
}


void MainWindow::on_btn_addProduct_clicked()
{
    if(ui->le_name->text().isEmpty() || ui->le_brand->text().isEmpty() ||
       ui->le_price->text().isEmpty() || ui->le_quantity->text().isEmpty())
    {
        QMessageBox::warning(this, "Lỗi",
            "Các thông tin cơ bản như tên, hãng,\nsố lượng, giá không được để trống!");
        return;
    }

    ProductParam productInfo;
    productInfo.name = ui->le_name->text();
    productInfo.brand = ui->le_brand->text();
    productInfo.quantity = ui->le_quantity->text().toInt();
    productInfo.price = ui->le_price->text().toDouble();
    if(!ui->le_extra1->text().isEmpty())
        productInfo.extra1 = ui->le_extra1->text();
    if(!ui->le_extra2->text().isEmpty())
        productInfo.extra2 = ui->le_extra2->text().toDouble();
    if(!ui->le_extra3->text().isEmpty())
        productInfo.extra3 = ui->le_extra3->text().toDouble();

    auto cmd = new AddCommand(DetermineType(ui->cbb_type->currentIndex())
                                            , productInfo);
    if(cmdManager->ExecuteCommand(cmd))
    {
        QMessageBox::information(this, "Thành công", "Thêm sản phẩm thành công!");
    }
    else
    {
        QMessageBox::warning(this, "Thất bại", "Thêm sản phẩm thất bại\n"
                                 "hãy kiểm tra lại thông tin sản phẩm hoặc\n"
                                 "xem sản phẩm đã tồn tại trước đó chưa?");
    }

    // delete cmd;
    ui->le_name->clear();
    ui->le_brand->clear();
    ui->le_quantity->clear();
    ui->le_price->clear();
    ui->le_extra1->clear();
    ui->le_extra2->clear();
    ui->le_extra3->clear();

    setWindowModified(true);
}

//==================================================================


void MainWindow::on_btn_confirm_clicked()
{
    if(productIds.empty())
    {
        QMessageBox::information(this, "Thông báo", "Không có sản phẩm nào được chọn!");
        return;
    }

    if(QMessageBox::question(this, "Xác nhận", "Bạn có chắc chắn muốn xóa "
                             + QString::number(productIds.size()) + " sản phẩm ???") == QMessageBox::Yes)
    {
        for(const auto& ID : productIds)
        {
            auto cmd = new DeleteCommand(ID);
            cmdManager->ExecuteCommand(cmd);
            // delete cmd;
        }
        productIds.clear();

        on_cbb_type_2_activated(0);
    }

    setWindowModified(true);
}


void MainWindow::on_btn_cancel_clicked()
{
    on_cbb_type_2_activated(0);
    productIds.clear();
}


//==================================================================

void MainWindow::on_le_search_textChanged(const QString &arg1)
{
    std::map<std::string, std::shared_ptr<Product>> list
        = db->GetProduct(arg1.toStdString());

    Display(list);
}

//==================================================================


void MainWindow::on_cbb_type_2_activated(int index)
{
    switch(index)
    {
    case 1:
        on_le_search_textChanged("E");
        break;
    case 2:
        on_le_search_textChanged("W");
        break;
    case 3:
        on_le_search_textChanged("O");
        break;
    default:
        on_le_search_textChanged("");
        break;
    }
}


//==================================================================

void MainWindow::on_tbw_itemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidgetItem *checkItem = ui->tbw->item(item->row(), 0);
    if(!item || !checkItem)
    {
        return;
    }

    std::string ID = ui->tbw->item(item->row(), 1)->text().toStdString();
    if(!ui->tbw->isColumnHidden(0)) //kiểm tra đang ở chế độ remove hay show
    {
        //chế độ remove cho phép
        if(checkItem->checkState() == Qt::Unchecked)
        {
            checkItem->setCheckState(Qt::Checked);
            productIds.push_back(ID);
        }
        else
        {
            checkItem->setCheckState(Qt::Unchecked);
            auto it = std::find(productIds.begin(), productIds.end(), ID);
            if (it != productIds.end())
            {
                productIds.erase(it);
            }
        }
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(3);
        std::map<std::string, std::shared_ptr<Product>> product =
            db->GetProduct(ID);
        ui->le_id_2->setText(QString::fromStdString(ID));
        if(ID[0] == 'E')
        {
            ui->cbb_type_3->setCurrentIndex(0);
            ui->lbl_extra1_2->show();
            ui->lbl_extra1_2->setText("Tiêu chuẩn an toàn :");
            ui->le_extra1_2->show();
            ui->le_extra1_2->setText(product[ID]->GetExtraData1());
            ui->lbl_extra2_2->show();
            ui->lbl_extra2_2->setText("Oát :");
            ui->le_extra2_2->show();
            ui->le_extra2_2->setText(QString::number(product[ID]->GetExtraData2()));
            ui->lbl_extra3_2->show();
            ui->le_extra3_2->show();
            ui->le_extra3_2->setText(QString::number(product[ID]->GetExtraData3()));

        }
        else if(ID[0] == 'W')
        {
            ui->cbb_type_3->setCurrentIndex(1);
            ui->lbl_extra1_2->show();
            ui->lbl_extra1_2->setText("Chất liệu :");
            ui->le_extra1_2->show();
            ui->le_extra1_2->setText(product[ID]->GetExtraData1());
            ui->lbl_extra2_2->show();
            ui->lbl_extra2_2->setText("Độ chịu nhiệt :");
            ui->le_extra2_2->show();
            ui->le_extra2_2->setText(QString::number(product[ID]->GetExtraData2()));
            ui->lbl_extra3_2->hide();
            ui->le_extra3_2->hide();
        }
        else
        {
            ui->cbb_type_3->setCurrentIndex(2);
            ui->lbl_extra1_2->hide();
            ui->le_extra1_2->hide();
            ui->lbl_extra2_2->hide();
            ui->le_extra2_2->hide();
            ui->lbl_extra3_2->hide();
            ui->le_extra3_2->hide();
        }
        ui->cbb_type_3->setEnabled(false);
        ui->le_name_2->setText(product[ID]->GetName());
        ui->le_brand_2->setText(product[ID]->GetBrand());
        ui->le_quantity_2->setText(QString::number(product[ID]->GetQuantity()));
        ui->le_price_2->setText(QString::number(product[ID]->GetPrice()));
        ui->sidebar->hide();
    }
}

//==================================================================

void MainWindow::on_btn_modify_clicked()
{
    std::string oldId = ui->le_id_2->text().toStdString();

    ProductParam newInfo;
    newInfo.name = ui->le_name_2->text();
    newInfo.brand = ui->le_brand_2->text();
    newInfo.quantity = ui->le_quantity_2->text().toInt();
    newInfo.price = ui->le_price_2->text().toDouble();
    if(!ui->le_extra1_2->text().isEmpty())
        newInfo.extra1 = ui->le_extra1_2->text();
    if(!ui->le_extra2_2->text().isEmpty())
        newInfo.extra2 = ui->le_extra2_2->text().toDouble();
    if(!ui->le_extra3_2->text().isEmpty())
        newInfo.extra3 = ui->le_extra3_2->text().toDouble();

    auto cmd = new EditCommand(oldId, newInfo);
    if(cmdManager->ExecuteCommand(cmd))
    {
        QMessageBox::information(this, "Thông báo", "Sửa đổi thành công !!!");
        on_btn_showPage_clicked();
    }
    else
    {
        QMessageBox::information(this, "Thông báo", "Không thể sửa đổi thông tin sản phẩm này");
    }

    // delete cmd;
    setWindowModified(true);
}


void MainWindow::on_btn_outModify_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}
