// /*
// #include "productDialog.h"
// #include <QFormLayout>
// #include <QHBoxLayout>
// #include <QDialogButtonBox>

// ProductDialog::ProductDialog(QWidget *parent)
//     : QDialog(parent)
// {
//     setWindowTitle("Thông tin sản phẩm");

//     categoryBox = new QComboBox(this);
//     categoryBox->addItems({"Điện", "Nước", "Khác"});
//     nameEdit = new QLineEdit(this);
//     brandEdit = new QLineEdit(this);
//     importPriceSpin = new QDoubleSpinBox(this);
//     importPriceSpin->setMaximum(1e9);
//     salePriceSpin = new QDoubleSpinBox(this);
//     salePriceSpin->setMaximum(1e9);
//     quantitySpin = new QSpinBox(this);
//     quantitySpin->setMaximum(1000000);

//     // Layout form chính
//     QFormLayout *formLayout = new QFormLayout;
//     formLayout->addRow("Loại:", categoryBox);
//     formLayout->addRow("Tên:", nameEdit);
//     formLayout->addRow("Tên:", brandEdit);
//     formLayout->addRow("Giá nhập:", importPriceSpin);
//     formLayout->addRow("Giá bán:", salePriceSpin);
//     formLayout->addRow("Số lượng:", quantitySpin);

//     // Gọi ngay để hiển thị đúng "Điện" lúc mở
//     onCategoryChanged(categoryBox->currentIndex());

//     // Extra fields container
//     extraWidget = new QWidget(this);
//     extraLayout = new QFormLayout(extraWidget);
//     formLayout->addRow(extraWidget);

//     // Buttons
//     QDialogButtonBox *buttonBox =
//         new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
//     connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
//     connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

//     // Layout chính
//     QVBoxLayout *mainLayout = new QVBoxLayout(this);
//     mainLayout->addLayout(formLayout);
//     mainLayout->addWidget(buttonBox);
//     setLayout(mainLayout);

//     connect(categoryBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
//             this, &ProductDialog::onCategoryChanged);
// }

// QString ProductDialog::getName() const
// {
//     return nameEdit->text();
// }

// QString ProductDialog::getBrand() const
// {
//     return brandEdit->text();
// }

// int ProductDialog::getCategory() const
// {
//     return categoryBox->currentIndex();
// }

// double ProductDialog::getImportPrice() const
// {
//     return importPriceSpin->value();
// }

// double ProductDialog::getSalePrice() const
// {
//     return salePriceSpin->value();
// }

// int ProductDialog::getQuantity() const
// {
//     return quantitySpin->value();
// }

// QMap<QString, QString> ProductDialog::getExtraFields() const
// {
//     QMap<QString, QString> fields;
//     QList<QLineEdit*> edits = extraWidget->findChildren<QLineEdit*>();
//     for (QLineEdit *edit : edits) {
//         fields.insert(edit->objectName(), edit->text());
//     }
//     return fields;
// }

// void ProductDialog::onCategoryChanged(int index) {
//     // Xóa field cũ
//     QLayoutItem *item;
//     while ((item = extraLayout->takeAt(0)) != nullptr) {
//         if (item->widget()) item->widget()->deleteLater();
//         delete item;
//     }

//     if (index == 0) { // Điện
//         QLineEdit *safety = new QLineEdit(this);
//         safety->setObjectName("extra1");
//         QLineEdit *power = new QLineEdit(this);
//         power->setObjectName("extra2");
//         QLineEdit *voltage = new QLineEdit(this);
//         voltage->setObjectName("extra3");

//         extraLayout->addRow("Tiêu chuẩn an toàn:", safety);
//         extraLayout->addRow("Công suất (W):", power);
//         extraLayout->addRow("Điện áp (V):", voltage);
//     }
//     else if (index == 1) { // Nước
//         QLineEdit *material = new QLineEdit(this);
//         material->setObjectName("extra1");
//         QLineEdit *temperatureLimit = new QLineEdit(this);
//         temperatureLimit->setObjectName("extra2");

//         extraLayout->addRow("Chất liệu:", material);
//         extraLayout->addRow("Độ chịu nhiệt:", temperatureLimit);
//     }
//     // index == 2 (Khác) => để trống
// }
// */

// #include "ProductDialog.h"
// #include <QVBoxLayout>
// #include <QDebug>
// #include <QSignalBlocker>

// ProductDialog::ProductDialog(QWidget *parent)
//     : QDialog(parent)
// {
//     setWindowTitle("Thêm/Sửa sản phẩm");
//     setModal(true);

//     formLayout = new QFormLayout;

//     // --- Category ---
//     categoryBox = new QComboBox(this);
//     categoryBox->addItems({"Điện", "Nước", "Khác"});
//     connect(categoryBox, &QComboBox::currentIndexChanged, this, &ProductDialog::onCategoryChanged);

//     // --- Các trường cơ bản ---
//     nameEdit = new QLineEdit(this);
//     brandEdit = new QLineEdit(this);

//     importPriceSpin = new QDoubleSpinBox(this);
//     importPriceSpin->setRange(0, 1e9);
//     importPriceSpin->setDecimals(2);
//     importPriceSpin->setSingleStep(1000.0);

//     salePriceSpin = new QDoubleSpinBox(this);
//     salePriceSpin->setRange(0, 1e9);
//     salePriceSpin->setDecimals(2);
//     salePriceSpin->setSingleStep(1000.0);

//     quantitySpin = new QSpinBox(this);
//     quantitySpin->setRange(0, 1e9);

//     // --- Thêm vào form layout ---
//     formLayout->addRow("Loại:", categoryBox);
//     formLayout->addRow("Tên:", nameEdit);
//     formLayout->addRow("Thương hiệu:", brandEdit);
//     formLayout->addRow("Giá nhập:", importPriceSpin);
//     formLayout->addRow("Giá bán:", salePriceSpin);
//     formLayout->addRow("Số lượng:", quantitySpin);

//     // --- ButtonBox (OK/Cancel) ---
//     QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
//     connect(buttonBox, &QDialogButtonBox::accepted, this, &ProductDialog::accept);
//     connect(buttonBox, &QDialogButtonBox::rejected, this, &ProductDialog::reject);

//     QVBoxLayout *mainLayout = new QVBoxLayout;
//     mainLayout->addLayout(formLayout);
//     mainLayout->addWidget(buttonBox);

//     setLayout(mainLayout);

//     // Gọi thủ công khi mở lần đầu -> đảm bảo extras được tạo
//     onCategoryChanged(categoryBox->currentIndex());
// }

// ProductDialog::~ProductDialog()
// {
//     clearExtras();
// }

// /**
//  * clearExtras:
//  *  - Xóa an toàn cả widget (ví dụ QLineEdit) và QLabel tương ứng
//  *  - Lý do: formLayout->addRow("Label", widget) tạo thêm một QLabel cho Label; nếu
//  *    chỉ delete widget thì label vẫn còn trong layout => gây lỗi logic hiển thị.
//  */
// void ProductDialog::clearExtras() {
//     // // Xóa tất cả field bổ sung trước đó
//     // for (QLineEdit *w : extras) {
//     //     // lấy label gắn với field này
//     //     QWidget *labelWidget = formLayout->labelForField(w);
//     //     if (labelWidget) {
//     //         QLabel *lbl = qobject_cast<QLabel*>(labelWidget); // Ép kiểu an toàn
//     //         if (lbl) {
//     //             formLayout->removeWidget(lbl);
//     //             lbl->deleteLater();
//     //         }
//     //     }
//     //     formLayout->removeWidget(w);
//     //     w->deleteLater();
//     // }
//     // extras.clear();
//     // duyệt tất cả các item từ dưới lên
//     // for (int i = formLayout->rowCount() - 1; i >= 0; --i) {
//     //     QWidget *field = formLayout->itemAt(i, QFormLayout::FieldRole) ?
//     //                          formLayout->itemAt(i, QFormLayout::FieldRole)->widget() : nullptr;
//     //     QWidget *label = formLayout->itemAt(i, QFormLayout::LabelRole) ?
//     //                          formLayout->itemAt(i, QFormLayout::LabelRole)->widget() : nullptr;

//     //     if (field && extras.values().contains(static_cast<QLineEdit*>(field))) {
//     //         formLayout->removeRow(i);
//     //         field->deleteLater();
//     //         if (label) label->deleteLater();
//     //     }
//     // }
//     // extras.clear();
//     for (auto it = extras.begin(); it != extras.end(); ++it) {
//         QWidget *field = it.value();
//         if (!field) continue;

//         // Xóa label đi kèm field này
//         QWidget *label = formLayout->labelForField(field);
//         if (label) {
//             formLayout->removeWidget(label);
//             label->deleteLater();
//         }

//         // Xóa chính field
//         formLayout->removeWidget(field);
//         field->deleteLater();
//     }
//     extras.clear();
// }

// void ProductDialog::onCategoryChanged(int index)
// {
//     // // Trước hết xóa extras cũ (bao gồm label)
//     // clearExtras();

//     // // Tạo extra fields tùy loại
//     // if (index == 0) { // Điện -> 3 extra
//     //     QLineEdit *extra1 = new QLineEdit(this);
//     //     QLineEdit *extra2 = new QLineEdit(this);
//     //     QLineEdit *extra3 = new QLineEdit(this);
//     //     extra1->setObjectName("extra1");
//     //     extra2->setObjectName("extra2");
//     //     extra3->setObjectName("extra3");
//     //     extras["extra1"] = extra1;
//     //     extras["extra2"] = extra2;
//     //     extras["extra3"] = extra3;
//     //     formLayout->addRow("Tiêu chuẩn an toàn: ", extra1);
//     //     formLayout->addRow("Điện năng (W): ", extra2);
//     //     formLayout->addRow("Vôn (V): ", extra3);
//     // } else if (index == 1) { // Nước -> 2 extra
//     //     QLineEdit *extra1 = new QLineEdit(this);
//     //     QLineEdit *extra2 = new QLineEdit(this);
//     //     extra1->setObjectName("extra1");
//     //     extra2->setObjectName("extra2");
//     //     extras["extra1"] = extra1;
//     //     extras["extra2"] = extra2;
//     //     formLayout->addRow("Chất liệu: ", extra1);
//     //     formLayout->addRow("Độ chịu nhiệt (C): ", extra2);
//     // } else {
//     //     // Khác: không có extra
//     // }
//     clearExtras(); // luôn xóa sạch trước

//     if (index == 0) { // Điện
//         auto e1 = new QLineEdit(this);
//         auto e2 = new QLineEdit(this);
//         auto e3 = new QLineEdit(this);
//         formLayout->addRow("Tiêu chuẩn an toàn:", e1);
//         formLayout->addRow("Điện năng (W):", e2);
//         formLayout->addRow("Vôn (V):", e3);
//         extras["extra1"] = e1;
//         extras["extra2"] = e2;
//         extras["extra3"] = e3;
//     }
//     else if (index == 1) { // Nước
//         auto e1 = new QLineEdit(this);
//         auto e2 = new QLineEdit(this);
//         formLayout->addRow("Chất liệu:", e1);
//         formLayout->addRow("Dung tích (L):", e2);
//         extras["extra1"] = e1;
//         extras["extra2"] = e2;
//     }
//     else {
//         // loại "Khác" -> không có extras
//     }
// }

// /*** Getters ***/
// QString ProductDialog::getName() const {
//     return nameEdit->text();
// }

// QString ProductDialog::getBrand() const {
//     return brandEdit->text();
// }

// int ProductDialog::getCategory() const {
//     return categoryBox->currentIndex();
// }

// double ProductDialog::getImportPrice() const {
//     return importPriceSpin->value();
// }

// double ProductDialog::getSalePrice() const {
//     return salePriceSpin->value();
// }

// int ProductDialog::getQuantity() const {
//     return quantitySpin->value();
// }

// QMap<QString, QString> ProductDialog::getExtras() const {
//     QMap<QString, QString> result;
//     for (auto it = extras.begin(); it != extras.end(); ++it) {
//         if (it.value())
//             result[it.key()] = it.value()->text();
//     }
//     return result;
// }

// /*** Helpers cho Edit mode ***/
// void ProductDialog::setCategoryLocked(bool locked)
// {
//     categoryBox->setEnabled(!locked);
// }

// void ProductDialog::setProductData(const QString &name,
//                                    const QString &brand,
//                                    double importPrice,
//                                    double salePrice,
//                                    int quantity,
//                                    int categoryIndex,
//                                    const QMap<QString, QString> &extraValues,
//                                    bool lockCategory)
// {
//     nameEdit->setText(name);
//     brandEdit->setText(brand);
//     importPriceSpin->setValue(importPrice);
//     salePriceSpin->setValue(salePrice);
//     quantitySpin->setValue(quantity);

//     // Gọi explicit để đảm bảo extras đã tạo
//     onCategoryChanged(categoryIndex);

//     // Gán giá trị extra nếu có
//     for (auto it = extraValues.begin(); it != extraValues.end(); ++it) {
//         if (extras.contains(it.key()) && extras[it.key()])
//             extras[it.key()]->setText(it.value());
//     }

//     setCategoryLocked(lockCategory);
// }
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
    for (int i = extrasForm->rowCount() - 1; i >= 0; --i) {
        if (auto *li = extrasForm->itemAt(i, QFormLayout::LabelRole)) {
            if (auto *w = li->widget()) { extrasForm->removeWidget(w); w->deleteLater(); }
            else delete li;
        }
        if (auto *fi = extrasForm->itemAt(i, QFormLayout::FieldRole)) {
            if (auto *w = fi->widget()) { extrasForm->removeWidget(w); w->deleteLater(); }
            else delete fi;
        }
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
        addExtraLine("Dung tích (L)","extra2");
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

    // Gán giá trị extras
    for (auto it = extraValues.constBegin(); it != extraValues.constEnd(); ++it) {
        if (extrasEdits.contains(it.key()) && extrasEdits[it.key()])
            extrasEdits[it.key()]->setText(it.value());
    }

    setCategoryLocked(lockCategory); // khóa loại khi edit nếu cần
}
