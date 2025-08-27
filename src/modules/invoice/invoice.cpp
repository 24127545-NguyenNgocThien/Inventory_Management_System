#include "invoice.h"
#include "otherutils.h"

Invoice::Invoice()
    : id(GenerateInvoiceId())
    , createdAt(QDateTime::currentDateTime())
{}

Invoice::Invoice(const std::string& invoiceId)
    : id(invoiceId)
    , createdAt(QDateTime::currentDateTime())
{}

const std::string& Invoice::GetId() const
{
    return id;
}

const QDateTime& Invoice::GetCreatedAt() const
{
    return createdAt;
}

const std::vector<InvoiceItem>& Invoice::GetItems() const
{
    return items;
}

void Invoice::SetCreatedAt(const QDateTime& dt)
{
    createdAt = dt;
}

void Invoice::AddItem(const std::string& productId, int quantity, double salePrice)
{
    items.push_back(InvoiceItem{productId, quantity, salePrice});
}

double Invoice::Total() const
{
    double total = 0.0;
    for(const auto& it : items)
    {
        total += it.salePrice * it.quantity;
    }
    return total;
}


QJsonObject Invoice::ToJson() const
{
    QJsonObject obj;
    obj["id"] = QString::fromStdString(id);
    obj["createdAt"] = createdAt.toString(Qt::ISODate);
    QJsonArray arr;
    for(const auto& it : items)
    {
        QJsonObject o;
        o["productId"] = QString::fromStdString(it.productId);
        o["quantity"]  = it.quantity;
        o["salePrice"] = it.salePrice;
        arr.append(o);
    }
    obj["items"] = arr;
    return obj;
}

Invoice Invoice::FromJson(const QJsonObject& obj)
{
    Invoice invoice;
    if(obj.contains("id"))
        invoice = Invoice(obj["id"].toString().toStdString());
    if(obj.contains("createdAt"))
        invoice.SetCreatedAt(QDateTime::fromString(obj["createdAt"].toString(), Qt::ISODate));

    if (obj.contains("items") && obj["items"].isArray())
    {
        QJsonArray arr = obj["items"].toArray();
        for(const auto& v : arr)
        {
            if(!v.isObject()) continue;
            QJsonObject o = v.toObject();
            invoice.AddItem(
                o["productId"].toString().toStdString(),
                o["quantity"].toInt(),
                o["salePrice"].toDouble()
                );
        }
    }
    return invoice;
}
