#include "selectiondialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QSpinBox>
#include <QSet>

SelectionDialog::SelectionDialog(QWidget *parent, bool flag)
    : QDialog(parent), _flag(flag)
{
    setWindowTitle("Chọn sản phẩm");
    resize(900, 520);

    // Filter widgets
    comboType  = new QComboBox(this);
    comboName  = new QComboBox(this);
    comboBrand = new QComboBox(this);
    editSearch = new QLineEdit(this);
    editSearch->setPlaceholderText("Tìm nhanh theo tên / id...");

    comboType->addItem("Tất cả");
    comboName->addItem("Tất cả");
    comboBrand->addItem("Tất cả");

    // Layout filter (gọn gàng trong 1 group)
    QHBoxLayout *hl = new QHBoxLayout;
    hl->addWidget(new QLabel("Loại:"));
    hl->addWidget(comboType, 1);
    hl->addSpacing(8);
    hl->addWidget(new QLabel("Tên:"));
    hl->addWidget(comboName, 2);
    hl->addSpacing(8);
    hl->addWidget(new QLabel("Thương hiệu:"));
    hl->addWidget(comboBrand, 2);
    hl->addSpacing(12);
    hl->addWidget(new QLabel("Tìm:"));
    hl->addWidget(editSearch, 3);

    // Table
    table = new QTableWidget(this);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"Mã", "Tên", "Thương hiệu", "Giá nhập", "Giá bán", "Tồn kho", "Số lượng"});
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);

    // Buttons
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SelectionDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SelectionDialog::reject);

    // Main layout
    QVBoxLayout *main = new QVBoxLayout(this);
    main->addLayout(hl);
    main->addWidget(table);
    main->addWidget(buttonBox);
    setLayout(main);

    // Signals
    connect(comboType,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SelectionDialog::onTypeChanged);
    connect(comboName,  QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SelectionDialog::onNameChanged);
    connect(comboBrand, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &SelectionDialog::onBrandChanged);
    connect(editSearch, &QLineEdit::textChanged, this, &SelectionDialog::onSearchTextChanged);
}

void SelectionDialog::loadData(const QList<SelectedProduct> &products)
{
    // copy toàn bộ dữ liệu nguồn
    allProducts = products;

    // build các combo theo toàn bộ tập dữ liệu
    rebuildFilterCombosFrom(allProducts);

    // áp dụng filter ban đầu
    applyFilters();
}

void SelectionDialog::rebuildFilterCombosFrom(const QList<SelectedProduct> &source)
{
    // Tạo danh sách unique cho mỗi filter
    QSet<QString> types;
    QSet<QString> names;
    QSet<QString> brands;
    for (const auto &p : source) {
        if (!p.type.isEmpty())  types.insert(p.type);
        if (!p.name.isEmpty())  names.insert(p.name);
        if (!p.brand.isEmpty()) brands.insert(p.brand);
    }

    // Block signals khi rebuild để tránh applyFilters nhiều lần
    comboType->blockSignals(true);
    comboName->blockSignals(true);
    comboBrand->blockSignals(true);

    comboType->clear();  comboType->addItem("Tất cả");
    for (const QString &t : types) comboType->addItem(t);

    comboName->clear();  comboName->addItem("Tất cả");
    for (const QString &n : names) comboName->addItem(n);

    comboBrand->clear(); comboBrand->addItem("Tất cả");
    for (const QString &b : brands) comboBrand->addItem(b);

    comboType->blockSignals(false);
    comboName->blockSignals(false);
    comboBrand->blockSignals(false);
}

void SelectionDialog::applyFilters()
{
    filteredProducts.clear();

    QString selType  = comboType->currentText();
    QString selName  = comboName->currentText();
    QString selBrand = comboBrand->currentText();
    QString search   = editSearch->text().trimmed();

    for (const auto &p : allProducts) {
        if (selType != "Tất cả" && !p.type.compare(selType, Qt::CaseInsensitive) == 0) {
            // note: use exact match for type; if type stored differently adapt accordingly
        }
        // We'll do comparisons in a simpler way to be case-insensitive and flexible:
        bool ok = true;
        if (selType != "Tất cả" && !p.type.compare(selType, Qt::CaseInsensitive) == 0) {
            ok = false;
        }
        if (ok && selName != "Tất cả" && !p.name.compare(selName, Qt::CaseInsensitive) == 0) {
            ok = false;
        }
        if (ok && selBrand != "Tất cả" && !p.brand.compare(selBrand, Qt::CaseInsensitive) == 0) {
            ok = false;
        }
        if (!ok) continue;

        // Search text filter: match id or name or brand (contains)
        if (!search.isEmpty()) {
            QString s = search.toLower();
            if (!(p.id.toLower().contains(s) || p.name.toLower().contains(s) || p.brand.toLower().contains(s))) {
                continue;
            }
        }

        filteredProducts.append(p);
    }

    // Update dependent combo boxes based on current filteredProducts:
    // Allow narrowing: if user picked Type, we want Name/Brand lists based on that type.
    // We'll compute available names/brands from the currently filteredProducts BEFORE search (so search shouldn't clear combos)
    // For simplicity, rebuild names/brands from filteredProducts:
    QSet<QString> namesSet, brandsSet;
    for (const auto &fp : filteredProducts) {
        namesSet.insert(fp.name);
        brandsSet.insert(fp.brand);
    }

    comboName->blockSignals(true);
    comboBrand->blockSignals(true);

    // Keep current selection if still present, otherwise reset to "Tất cả"
    QString curName  = comboName->currentText();
    QString curBrand = comboBrand->currentText();

    comboName->clear();
    comboName->addItem("Tất cả");
    for (const QString &n : namesSet) comboName->addItem(n);

    comboBrand->clear();
    comboBrand->addItem("Tất cả");
    for (const QString &b : brandsSet) comboBrand->addItem(b);

    // restore selection if it still exists
    int nameIndex = comboName->findText(curName);
    if (nameIndex >= 0) comboName->setCurrentIndex(nameIndex); // keep previous
    int brandIndex = comboBrand->findText(curBrand);
    if (brandIndex >= 0) comboBrand->setCurrentIndex(brandIndex);

    comboName->blockSignals(false);
    comboBrand->blockSignals(false);

    populateTable();
}

void SelectionDialog::populateTable()
{
    table->setRowCount(filteredProducts.size());
    for (int i = 0; i < filteredProducts.size(); ++i) {
        const SelectedProduct &p = filteredProducts.at(i);

        // ID
        QTableWidgetItem *itId = new QTableWidgetItem(p.id);
        itId->setData(Qt::UserRole, p.id);
        table->setItem(i, 0, itId);

        // Name
        table->setItem(i, 1, new QTableWidgetItem(p.name));
        // Brand
        table->setItem(i, 2, new QTableWidgetItem(p.brand));
        // Import price
        table->setItem(i, 3, new QTableWidgetItem(QString::number(p.importPrice)));
        // Sale price
        table->setItem(i, 4, new QTableWidgetItem(QString::number(p.salePrice)));
        // Stock
        table->setItem(i, 5, new QTableWidgetItem(QString::number(p.stock)));

        // Quantity - use QSpinBox (0..stock)
        QSpinBox *spin = new QSpinBox(this);
        if (_flag) {
            spin->setRange(0, qMax(0, p.stock));
        } else {
            spin->setRange(0, 1000000000);
        }
        spin->setValue(p.quantity); // if had previous selection
        spin->setMinimumWidth(80);
        table->setCellWidget(i, 6, spin);
    }

    // Optionally adjust row heights
    table->verticalHeader()->setDefaultSectionSize(28);
}

QList<SelectedProduct> SelectionDialog::getSelections() const
{
    QList<SelectedProduct> out;
    for (int i = 0; i < table->rowCount(); ++i) {
        QWidget *w = table->cellWidget(i, 6);
        QSpinBox *spin = qobject_cast<QSpinBox*>(w);
        int qty = spin ? spin->value() : 0;
        if (qty <= 0) continue; // only return selected > 0

        SelectedProduct p;
        p.id = table->item(i, 0)->text();
        p.name = table->item(i, 1)->text();
        p.brand = table->item(i, 2)->text();
        p.importPrice = table->item(i, 3)->text().toDouble();
        p.salePrice = table->item(i, 4)->text().toDouble();
        p.stock = table->item(i, 5)->text().toInt();
        p.quantity = qty;
        out.append(p);
    }
    return out;
}

/*** Slots ***/
void SelectionDialog::onTypeChanged(int /*idx*/)
{
    applyFilters();
}

void SelectionDialog::onNameChanged(int /*idx*/)
{
    applyFilters();
}
void SelectionDialog::onBrandChanged(int /*idx*/)
{
    applyFilters();
}
void SelectionDialog::onSearchTextChanged(const QString &)
{
    applyFilters();
}
