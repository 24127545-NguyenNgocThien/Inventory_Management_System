#ifndef INVOICE_H
#define INVOICE_H

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <vector>
#include <string>

struct InvoiceItem
{
    std::string productId;
    int quantity;
    double salePrice;  // giá bán tại thời điểm lập hóa đơn
};

class Invoice
{
private:
    std::string id;
    QDateTime createdAt;
    std::vector<InvoiceItem> items;

public:
    Invoice();
    explicit Invoice(const std::string& invoiceId);

    const std::string& GetId() const;
    const QDateTime& GetCreatedAt() const;
    const std::vector<InvoiceItem>& GetItems() const;

    void SetCreatedAt(const QDateTime& dt);
    void AddItem(const std::string& productId, int quantity, double salePrice);
    double Total() const;

    QJsonObject ToJson() const;
    static Invoice FromJson(const QJsonObject& obj);
};

#endif // INVOICE_H
