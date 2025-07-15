#pragma once

#include "product.h"

#include <memory>
#include <mutex>
#include <map>

//lớp App đc tạo theo kiểu singleton để chắc chắn rằng chỉ
//có duy nhất một instance có quyền thay đổi các thông tin của kho hàng
class App
{
private:
    static std::unique_ptr<App> instance;
    static std::mutex mtx;

    std::map<std::string, std::shared_ptr<Product>> manager;

    App();

public:
    App(const App&) = delete;
    App& operator = (const App&) = delete;

    static App& GetInstance();

    int GetNumberProduct();

    bool isProductExists(ProductType type, const QString& name, const QString& brand);
    bool AddProduct(ProductType type, const QString name,const QString brand,
                    const int& quantity, const double& price, const QString& extra1 =""
                    , const double& extra2 = 0, const double& extra3 = 0);
    bool RemoveProduct(const std::string& ID);
    bool ModifyProduct(std::shared_ptr<Product> product);

    void Undo(); // chỉ áp dụng với 3 thao tác add, remove, modify
    void Redo(); // chỉ áp dụng với 3 thao tác add, remove, modify

    std::map<std::string, std::shared_ptr<Product>> GetProductById(const std::string& ID);
    std::shared_ptr<Product> GetProductBySTT(int stt); //lấy phần tử theo số thứ tự đc lưu trong map

    void Save();
    void Load();
};
