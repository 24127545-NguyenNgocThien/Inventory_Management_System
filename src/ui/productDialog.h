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
#include <QWidget>

class ProductDialog : public QDialog {
    Q_OBJECT
public:
    explicit ProductDialog(QWidget *parent = nullptr);
    ~ProductDialog();

    // Getters
    int getCategory() const;
    QString getName() const;
    QString getBrand() const;
    double getImportPrice() const;
    double getSalePrice() const;
    int getQuantity() const;
    QMap<QString, QString> getExtras() const;

    // Dùng cho chế độ Edit
    void setProductData(const QString &name,
                        const QString &brand,
                        double importPrice,
                        double salePrice,
                        int quantity,
                        int categoryIndex,
                        const QMap<QString, QString> &extraValues = {},
                        bool lockCategory = false);
    void setCategoryLocked(bool locked);

private slots:
    void onCategoryChanged(int index);

private:
    // Form cơ bản
    QFormLayout *baseForm;
    QComboBox *categoryBox;
    QLineEdit *nameEdit;
    QLineEdit *brandEdit;
    QDoubleSpinBox *importPriceSpin;
    QDoubleSpinBox *salePriceSpin;
    QSpinBox *quantitySpin;

    // Vùng extras tách riêng
    QWidget *extrasPanel;
    QFormLayout *extrasForm;
    QMap<QString, QLineEdit*> extrasEdits;

    void clearExtras();                // chỉ xóa trong extrasForm
    void addExtraLine(const QString &label, const QString &key);
};

#endif // PRODUCTDIALOG_H
