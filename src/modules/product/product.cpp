#include "product.h"


Product::Product(const ProductParam& productInfo)
    : name(productInfo.name), brand(productInfo.brand), id(productInfo.id),
    quantity(productInfo.quantity), importPrice(productInfo.importPrice),
    salePrice(productInfo.salePrice)
{}

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

double Product::GetSalePrice()
{
    return salePrice;
}

double Product::GetImportPrice()
{
    return importPrice;
}

void Product::SetName(const QString& name)
{
    this->name = name;
}
void Product::SetBrand(const QString& brand)
{
    this->brand = brand;
}

void Product::SetId(const std::string& id)
{
    this->id = id;
}

void Product::SetQuantity(int quantity)
{
    this->quantity = quantity;
}

void Product::SetSalePrice(double price)
{
    this->salePrice = price;
}

void Product::SetImportPrice(double price)
{
    this->importPrice = price;
}
