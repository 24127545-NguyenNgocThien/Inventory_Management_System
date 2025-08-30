#pragma once

#include <QDialog>
#include <QComboBox>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QList>
#include <QPushButton>

/**
 * Struct để chứa thông tin cần hiển thị cho mỗi sản phẩm.
 * Bạn có thể chuyển dữ liệu từ model của bạn sang struct này khi gọi loadData(...)
 */
struct SelectedProduct {
    QString type;       // ví dụ: "Điện", "Nước", "Khác" — nên do bạn set khi load
    QString id;
    QString name;
    QString brand;
    double importPrice = 0.0;
    double salePrice = 0.0;
    int stock = 0;      // tồn kho
    int quantity = 0;   // quantity được chọn (khi trả về)
};

class SelectionDialog : public QDialog {
    Q_OBJECT
public:
    explicit SelectionDialog(QWidget *parent = nullptr, bool flag = false);
    ~SelectionDialog() override = default;

    // Nạp dữ liệu (chuyển từ DB sang danh sách này trước khi gọi)
    void loadData(const QList<SelectedProduct> &products);

    // Lấy kết quả (các sản phẩm có quantity > 0)
    QList<SelectedProduct> getSelections() const;

private slots:
    void onTypeChanged(int idx);
    void onNameChanged(int idx);
    void onBrandChanged(int idx);
    void onSearchTextChanged(const QString &txt);

private:
    // Phân biệt lệnh import(false) hay export(true)
    bool _flag;

    // Widgets
    QComboBox *comboType;
    QComboBox *comboName;
    QComboBox *comboBrand;
    QLineEdit  *editSearch;
    QTableWidget *table;
    QDialogButtonBox *buttonBox;

    // Dữ liệu
    QList<SelectedProduct> allProducts;
    QList<SelectedProduct> filteredProducts;

    // Helpers
    void applyFilters();
    void populateTable();
    void rebuildFilterCombosFrom(const QList<SelectedProduct> &source);
};
