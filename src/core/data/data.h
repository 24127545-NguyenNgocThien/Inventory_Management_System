#ifndef DATA_H
#define DATA_H

#include "product.h"
#include <map>
#include <memory>
#include <mutex>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include "invoice.h"

class Database
{
private:
    std::map<std::string, std::shared_ptr<Product>> productsList;
    const char* fileName;
    QString invoiceFileName;      // NEW
    std::vector<Invoice*> invoices; // NEW

    static std::unique_ptr<Database> instance;
    static std::mutex mtx;
    Database();

public:
    Database(const Database&) = delete;
    Database& operator = (const Database&) = delete;

    static Database& GetInstance();
    ~Database();

    bool Save();
    void Load();

    int GetNumberProduct();

    void AddProduct(std::shared_ptr<Product> product);
    void DeleteProduct(const std::string& id);
    void EditProduct(std::shared_ptr<Product> oldProduct, const ProductParam& newInfo);
    std::map<std::string, std::shared_ptr<Product>> GetProduct(const std::string& id) const;

    bool SaveInvoices(); // NEW
    void LoadInvoices(); // NEW
    void AddInvoice(Invoice* inv);         // NEW
    const std::vector<Invoice*>& GetInvoices() const; // NEW
    const std::vector<Invoice*> GetInvoicesInRange(const QDate& from, const QDate& to) const;
    bool ReturnInvoice(const std::string& invoiceId);

};

#endif // DATA_H
