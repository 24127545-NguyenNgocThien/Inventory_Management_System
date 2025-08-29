/*
#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMap>

class ProductDialog : public QDialog
{
    Q_OBJECT
public:
    ProductDialog(QWidget *parent = nullptr);

    QString getName() const;
    QString getBrand() const;
    int getCategory() const;
    double getImportPrice() const;
    double getSalePrice() const;
    int getQuantity() const;
    QMap<QString, QString> getExtras() const; // Thuộc tính mở rộng

private slots:
    void onCategoryChanged(int index);

private:
    QFormLayout *formLayout;

    QLineEdit *nameEdit, *brandEdit;
    QComboBox *categoryBox;
    QDoubleSpinBox *importPriceSpin;
    QDoubleSpinBox *salePriceSpin;
    QSpinBox *quantitySpin;

    // QWidget *extraWidget;
    // QFormLayout *extraLayout;
    // Lưu các extra field, key = "extra1", "extra2", ...
    QMap<QString, QLineEdit*> extras;

    void clearExtras();
};

#endif // PRODUCTDIALOG_H
*/

#ifndef PRODUCTDIALOG_H
#define PRODUCTDIALOG_H

#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QMap>
#include <QLabel>

class ProductDialog : public QDialog {
    Q_OBJECT

public:
    explicit ProductDialog(QWidget *parent = nullptr);
    ~ProductDialog();

    // Getters
    QString getName() const;
    QString getBrand() const;
    int getCategory() const;
    double getImportPrice() const;
    double getSalePrice() const;
    int getQuantity() const;
    QMap<QString, QString> getExtras() const;

    // Helpers for edit mode:
    // Đặt dữ liệu sẵn có (khi mở dialog ở chế độ Edit)
    void setProductData(const QString &name,
                        const QString &brand,
                        double importPrice,
                        double salePrice,
                        int quantity,
                        int categoryIndex,
                        const QMap<QString, QString> &extraValues = QMap<QString, QString>(),
                        bool lockCategory = false);

    // Khóa/ mở combobox loại (dùng khi edit để không cho đổi loại)
    void setCategoryLocked(bool locked);

private slots:
    void onCategoryChanged(int index);

private:
    QFormLayout *formLayout;

    QComboBox *categoryBox;
    QLineEdit *nameEdit;
    QLineEdit *brandEdit;
    QDoubleSpinBox *importPriceSpin;
    QDoubleSpinBox *salePriceSpin;
    QSpinBox *quantitySpin;

    // Lưu các extra field, key = "extra1", "extra2", ...
    QMap<QString, QLineEdit*> extras;

    // Xóa an toàn cả label và widget của các extra
    void clearExtras();
};

#endif // PRODUCTDIALOG_H
