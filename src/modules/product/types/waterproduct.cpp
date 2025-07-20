#include "waterproduct.h"

WaterProduct::WaterProduct(const ProductParam& productInfo)
    : Product(productInfo), material(productInfo.extra1),
      temperatureLimit(productInfo.extra2)
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
    return NAN;
}

void WaterProduct::SetExtraData1(const QString& extra1)
{
    material = extra1;
}

void WaterProduct::SetExtraData2(const double& extra2)
{
    temperatureLimit = extra2;
}

void WaterProduct::SetExtraData3(const double& extra3)
{
    return;
}
