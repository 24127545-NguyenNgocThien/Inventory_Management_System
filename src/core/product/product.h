#pragma once
#include <string>
#include <QString>

enum class ProductType
{
    Electric,
    Water,
    Other
};

//-----------------

class Product
{
protected:
    QString name;
    QString brand;
    std::string id;
    int quantity;
    double price;

public:
    Product(const QString& name, const QString& brand,
            const std::string& id, int quantity, double price);
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
    void SetId(std::string id);
    void SetQuantity(int n);
    void SetPrice(double p);
};



//Các lớp con cụ thể của product 
class ElectricProduct : public Product
{
private:
    QString safetyStandard;
    double power;
    double voltage; 

public:
    ElectricProduct(const QString& name, const QString& brand,
                    const std::string& id, const int& quantity,
                    const double& price, const QString& safetyStandard,
                    const double& power, const double& voltage);

    ProductType GetType() const override;

    QString GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;

};


class WaterProduct : public Product
{
private:
    QString material;
    double temperatureLimit;

public:
    WaterProduct(const QString& name, const QString& brand,
                 const std::string& id, const int& quantity, const double& price,
                 const QString& material, const double& temperatureLimit);

    ProductType GetType() const override;

    QString GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;
};


class OtherProduct : public Product
{
public:
    OtherProduct(const QString& name, const QString& brand,
                 const std::string& id, const int& quantity, const double& price);

    ProductType GetType() const override;

    QString GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;
};
