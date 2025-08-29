#include "productDialog.h"
#include <QVBoxLayout>
#include <QLabel>

ProductDialog::ProductDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Thêm/Sửa sản phẩm");
    setModal(true);

    // --- Form chính ---
    baseForm = new QFormLayout;

    categoryBox = new QComboBox(this);
    categoryBox->addItems({"Điện", "Nước", "Khác"});
    connect(categoryBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ProductDialog::onCategoryChanged);

    nameEdit = new QLineEdit(this);
    brandEdit = new QLineEdit(this);

    importPriceSpin = new QDoubleSpinBox(this);
    importPriceSpin->setRange(0, 1e12);
    importPriceSpin->setDecimals(2);

    salePriceSpin = new QDoubleSpinBox(this);
    salePriceSpin->setRange(0, 1e12);
    salePriceSpin->setDecimals(2);

    quantitySpin = new QSpinBox(this);
    quantitySpin->setRange(0, 1000000000);

    baseForm->addRow("Loại:", categoryBox);
    baseForm->addRow("Tên:", nameEdit);
    baseForm->addRow("Thương hiệu:", brandEdit);
    baseForm->addRow("Giá nhập:", importPriceSpin);
    baseForm->addRow("Giá bán:", salePriceSpin);
    baseForm->addRow("Số lượng:", quantitySpin);

    // --- Vùng extras riêng ---
    extrasPanel = new QWidget(this);
    extrasForm  = new QFormLayout(extrasPanel);
    extrasPanel->setLayout(extrasForm);

    // Khởi tạo extras theo loại mặc định
    onCategoryChanged(categoryBox->currentIndex());

    // --- Buttons ---
    QDialogButtonBox *buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ProductDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &ProductDialog::reject);

    // --- Layout tổng ---
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(baseForm);
    mainLayout->addWidget(extrasPanel);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);
}

ProductDialog::~ProductDialog() {
    clearExtras();
}






void ProductDialog::addExtraLine(const QString &label, const QString &key) {
    auto *e = new QLineEdit(this);
    extrasEdits.insert(key, e);
    extrasForm->addRow(label + ":", e);
}

// Xóa SẠCH vùng extras mà không đụng vào form chính
void ProductDialog::clearExtras() {
    // for (int i = extrasForm->rowCount() - 1; i >= 0; --i) {
    //     if (auto *li = extrasForm->itemAt(i, QFormLayout::LabelRole)) {
    //         if (auto *w = li->widget()) { extrasForm->removeWidget(w); w->deleteLater(); }
    //         else delete li;
    //     }
    //     if (auto *fi = extrasForm->itemAt(i, QFormLayout::FieldRole)) {
    //         if (auto *w = fi->widget()) { extrasForm->removeWidget(w); w->deleteLater(); }
    //         else delete fi;
    //     }
    // }
    // extrasEdits.clear();
    for (int i = extrasForm->rowCount() - 1; i >= 0; --i) {
        extrasForm->removeRow(i);   // gỡ cả label và field
    }
    extrasEdits.clear();
}

void ProductDialog::onCategoryChanged(int index) {
    clearExtras();

    switch (index) {
    case 0: // Điện
        addExtraLine("Tiêu chuẩn an toàn", "extra1");
        addExtraLine("Điện năng (W)",      "extra2");
        addExtraLine("Vôn (V)",            "extra3");
        break;
    case 1: // Nước
        addExtraLine("Chất liệu",    "extra1");
        addExtraLine("Độ chịu nhiệt (C)","extra2");
        break;
    case 2: // Khác
    default:
        // Không có extras
        break;
    }
}






// --- Getters ---
int     ProductDialog::getCategory() const    { return categoryBox->currentIndex(); }
QString ProductDialog::getName() const        { return nameEdit->text(); }
QString ProductDialog::getBrand() const       { return brandEdit->text(); }
double  ProductDialog::getImportPrice() const { return importPriceSpin->value(); }
double  ProductDialog::getSalePrice() const   { return salePriceSpin->value(); }
int     ProductDialog::getQuantity() const    { return quantitySpin->value(); }
QMap<QString, QString> ProductDialog::getExtras() const {
    QMap<QString, QString> out;
    for (auto it = extrasEdits.constBegin(); it != extrasEdits.constEnd(); ++it) {
        if (it.value()) out.insert(it.key(), it.value()->text());
    }
    return out;
}






void ProductDialog::setCategoryLocked(bool locked) {
    categoryBox->setEnabled(!locked);
}

void ProductDialog::setProductData(const QString &name,
                                   const QString &brand,
                                   double importPrice,
                                   double salePrice,
                                   int quantity,
                                   int categoryIndex,
                                   const QMap<QString, QString> &extraValues,
                                   bool lockCategory)
{
    nameEdit->setText(name);
    brandEdit->setText(brand);
    importPriceSpin->setValue(importPrice);
    salePriceSpin->setValue(salePrice);
    quantitySpin->setValue(quantity);

    // Đặt loại -> Qt tự emit signal gọi onCategoryChanged để dựng lại extras
    categoryBox->setCurrentIndex(categoryIndex);
    setCategoryLocked(lockCategory); // khóa loại khi edit

    // Gán giá trị extras
    for (auto it = extraValues.constBegin(); it != extraValues.constEnd(); ++it) {
        if (extrasEdits.contains(it.key()) && extrasEdits[it.key()])
            extrasEdits[it.key()]->setText(it.value());
    }
}
