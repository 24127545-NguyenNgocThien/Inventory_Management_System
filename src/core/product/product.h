#pragma once
#include <string>

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
    std::string name;
    std::string brand;
    std::string id;
    int quantity;
    double price;

public:
    Product(const std::string& name, const std::string& brand,
            const std::string& id, int quantity, double price);
    virtual ~Product() = default;

    //Getter
    std::string GetName();
    std::string GetBrand();
    std::string GetId();
    int GetQuantity();
    double GetPrice();
    virtual ProductType GetType() const = 0;
    virtual std::string GetExtraData1() const = 0;
    virtual double GetExtraData2() const = 0;
    virtual double GetExtraData3() const = 0;

    //Setter
    void SetName(std::string name);
    void SetBrand(std::string brand);
    void SetId(std::string id);
    void SetQuantity(int n);
    void SetPrice(double p);

    virtual void ShowInfo() const = 0;
};



//Các lớp con cụ thể của product 
class ElectricProduct : public Product
{
private:
    std::string safetyStandard;
    double power;
    double voltage; 

public:
    ElectricProduct(const std::string& name, const std::string& brand,
                    const std::string& id, const int& quantity,
                    const double& price, const std::string& safetyStandard,
                    const double& power, const double& voltage);

    ProductType GetType() const override;

    std::string GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;

    void ShowInfo() const override;
};


class WaterProduct : public Product
{
private:
    std::string material;
    double temperatureLimit;

public:
    WaterProduct(const std::string& name, const std::string& brand,
                 const std::string& id, const int& quantity, const double& price,
                 const std::string& material, const double& temperatureLimit);

    ProductType GetType() const override;

    std::string GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;

    void ShowInfo() const override;
};


class OtherProduct : public Product
{
public:
    OtherProduct(const std::string& name, const std::string& brand,
                 const std::string& id, const int& quantity, const double& price);

    ProductType GetType() const override;

    std::string GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;

    void ShowInfo() const override;
};
