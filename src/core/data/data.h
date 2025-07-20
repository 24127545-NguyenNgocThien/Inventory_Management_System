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

class Database
{
private:
    std::map<std::string, std::shared_ptr<Product>> productsList;
    const char* fileName;

    static std::unique_ptr<Database> instance;
    static std::mutex mtx;
    Database();

public:
    Database(const Database&) = delete;
    Database& operator = (const Database&) = delete;

    static Database& GetInstance();
    ~Database() {};

    bool Save();
    void Load();

    int GetNumberProduct();

    void AddProduct(std::shared_ptr<Product> product);
    void DeleteProduct(const std::string& id);
    void EditProduct(std::shared_ptr<Product> oldProduct, const ProductParam& newInfo);
    std::map<std::string, std::shared_ptr<Product>> GetProduct(const std::string& id);
};

#endif // DATA_H
