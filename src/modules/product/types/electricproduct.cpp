#include "electricproduct.h"

ElectricProduct::ElectricProduct(const ProductParam& productInfo)
    : Product(productInfo), safetyStandard(productInfo.extra1),
      power(productInfo.extra2), voltage(productInfo.extra3)
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

void ElectricProduct::SetExtraData1(const QString& extra1)
{
    safetyStandard = extra1;
}

void ElectricProduct::SetExtraData2(const double& extra)
{
    power = extra;
}

void ElectricProduct::SetExtraData3(const double& extra3)
{
    voltage = extra3;
}
