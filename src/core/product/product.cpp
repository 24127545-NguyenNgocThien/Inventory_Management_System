#include "product.h"

#include <iostream>

//------------------- PRODUCT CLASS -----------------------

Product::Product(const std::string& name, const std::string& brand,
                 const std::string& id, int quantity, double price)
    : name(name), brand(brand), id(id), quantity(quantity), price(price) {}

std::string Product::GetName()
{
    return name;
}

std::string Product::GetBrand()
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

void Product::SetName(std::string name)
{
    this->name = name;
}
void Product::SetBrand(std::string brand)
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

ElectricProduct::ElectricProduct(const std::string& name, const std::string& brand,
                const std::string& id, const int& quantity,const double& price,
                const std::string& safetyStandard,const double& power, const double& voltage)
    : Product(name, brand, id, quantity, price), safetyStandard(safetyStandard),
    power(power), voltage(voltage)
{}

ProductType ElectricProduct::GetType() const
{
    return ProductType::Electric;
}

std::string ElectricProduct::GetExtraData1() const
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

void ElectricProduct::ShowInfo() const
{
    std::cout << "Ma san pham           : " << id << std::endl
              << "   -Ten san pham      : " << name << std::endl
              << "   -Thuong hieu       : " << brand << std::endl
              << "   -Von               : " << voltage << std::endl
              << "   -Cong suat tieu thu: " << power << std::endl
              << "   -Ton kho           : " << quantity << std::endl
              << "   -Don gia           : " << price << std::endl;
}


//------------------- WATER PRODUCT CLASS -----------------------

WaterProduct::WaterProduct(const std::string& name, const std::string& brand,
             const std::string& id, const int& quantity, const double& price,
             const std::string& material, const double& temperatureLimit)
    : Product(name, brand, id, quantity, price), material(material),
      temperatureLimit(temperatureLimit)
{}

ProductType WaterProduct::GetType() const
{
    return ProductType::Water;
}

std::string WaterProduct::GetExtraData1() const
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

void WaterProduct::ShowInfo() const
{
    std::cout << "Ma san pham            : " << id << std::endl
              << "   -Ten san pham       : " << name << std::endl
              << "   -Thuong hieu        : " << brand << std::endl
              << "   -Chat lieu          : " << material << std::endl
              << "   -Gioi han chiu nhiet: " << temperatureLimit << std::endl
              << "   -Ton kho            : " << quantity << std::endl
              << "   -Don gia            : " << price << std::endl;
}


//------------------- OTHER PRODUCT CLASS -----------------------

OtherProduct::OtherProduct(const std::string& name, const std::string& brand,
             const std::string& id, const int& quantity, const double& price)
    : Product(name, brand, id, quantity, price)
{}

ProductType OtherProduct::GetType() const
{
    return ProductType::Other;
}

std::string OtherProduct::GetExtraData1() const
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

void OtherProduct::ShowInfo() const
{
    std::cout << "Ma san pham           : " << id << std::endl
              << "   -Ten san pham      : " << name << std::endl
              << "   -Thuong hieu       : " << brand << std::endl
              << "   -Ton kho           : " << quantity << std::endl
              << "   -Don gia           : " << price << std::endl;
}

