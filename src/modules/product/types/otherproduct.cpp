#include "otherproduct.h"

OtherProduct::OtherProduct(const ProductParam& productInfo)
    : Product(productInfo)
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
    return NAN;
}

double OtherProduct::GetExtraData3() const
{
    return NAN;
}

void OtherProduct::SetExtraData1(const QString& data1)
{
    return;
}

void OtherProduct::SetExtraData2(const double& data2)
{
    return;
}

void OtherProduct::SetExtraData3(const double& data3)
{
    return;
}
