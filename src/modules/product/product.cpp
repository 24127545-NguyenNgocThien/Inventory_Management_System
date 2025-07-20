#include "product.h"


Product::Product(const ProductParam& productInfo)
    : name(productInfo.name), brand(productInfo.brand), id(productInfo.id),
      quantity(productInfo.quantity), price(productInfo.price)
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

void Product::SetId(const std::string& id)
{
    this->id = id;
}

void Product::SetQuantity(int quantity)
{
    this->quantity = quantity;
}

void Product::SetPrice(double price)
{
    this->price = price;
}
