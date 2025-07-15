#include "product.h"

//------------------- PRODUCT CLASS -----------------------

Product::Product(const QString& name, const QString& brand,
                 const std::string& id, int quantity, double price)
    : name(name), brand(brand), id(id), quantity(quantity), price(price) {}

QString Product::GetName()
{
    return name;
}

QString Product::GetBrand()
{
    return brand;
}

std::string Product::GetId()
{
    return id;
}

int Product::GetQuantity()
{
    return quantity;
}

double Product::GetPrice()
{
    return price;
}

void Product::SetName(const QString& name)
{
    this->name = name;
}
void Product::SetBrand(const QString& brand)
{
    this->brand = brand;
}

void Product::SetId(std::string id)
{
    this->id = id;
}

void Product::SetQuantity(int n)
{
    quantity = n;
}

void Product::SetPrice(double p)
{
    price = p;
}


//-------------------ELECTRIC PRODUCT CLASS -----------------------

ElectricProduct::ElectricProduct(const QString& name, const QString& brand,
                const std::string& id, const int& quantity,const double& price,
                const QString& safetyStandard,const double& power, const double& voltage)
    : Product(name, brand, id, quantity, price), safetyStandard(safetyStandard),
    power(power), voltage(voltage)
{}

ProductType ElectricProduct::GetType() const
{
    return ProductType::Electric;
}

QString ElectricProduct::GetExtraData1() const
{
    return safetyStandard;
}

double ElectricProduct::GetExtraData2() const
{
    return power;
}

double ElectricProduct::GetExtraData3() const
{
    return voltage;
}



//------------------- WATER PRODUCT CLASS -----------------------

WaterProduct::WaterProduct(const QString& name, const QString& brand,
             const std::string& id, const int& quantity, const double& price,
             const QString& material, const double& temperatureLimit)
    : Product(name, brand, id, quantity, price), material(material),
      temperatureLimit(temperatureLimit)
{}

ProductType WaterProduct::GetType() const
{
    return ProductType::Water;
}

QString WaterProduct::GetExtraData1() const
{
    return material;
}

double WaterProduct::GetExtraData2() const
{
    return temperatureLimit;
}

double WaterProduct::GetExtraData3() const
{
    return 0;
}



//------------------- OTHER PRODUCT CLASS -----------------------

OtherProduct::OtherProduct(const QString& name, const QString& brand,
             const std::string& id, const int& quantity, const double& price)
    : Product(name, brand, id, quantity, price)
{}

ProductType OtherProduct::GetType() const
{
    return ProductType::Other;
}

QString OtherProduct::GetExtraData1() const
{
    return "";
}

double OtherProduct::GetExtraData2() const
{
    return 0;
}

double OtherProduct::GetExtraData3() const
{
    return 0;
}
