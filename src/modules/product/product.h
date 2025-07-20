#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <cmath>

//===============================
enum class ProductType
{
    None,
    Electric,
    Water,
    Other
};


//==============hỗ trợ clean code=============
struct ProductParam
{
    QString name;
    QString brand;
    std::string id;
    int quantity;
    double price;
    QString extra1 = "";
    double extra2 = NAN;
    double extra3 = NAN;
};


//===============================
class Product
{
protected:
    QString name;
    QString brand;
    std::string id;
    int quantity;
    double price;

public:
    Product(const ProductParam& productInfo);
    virtual ~Product() = default;

    //Getter
    QString GetName();
    QString GetBrand();
    std::string GetId();
    int GetQuantity();
    double GetPrice();
    virtual ProductType GetType() const = 0;
    virtual QString GetExtraData1() const = 0;
    virtual double GetExtraData2() const = 0;
    virtual double GetExtraData3() const = 0;

    //Setter
    void SetName(const QString& name);
    void SetBrand(const QString& brand);
    void SetId(const std::string& id);
    void SetQuantity(int quantity);
    void SetPrice(double price);
    virtual void SetExtraData1(const QString& extra1) = 0;
    virtual void SetExtraData2(const double& extra2) = 0;
    virtual void SetExtraData3(const double& extra3) = 0;
};

#endif // PRODUCT_H
