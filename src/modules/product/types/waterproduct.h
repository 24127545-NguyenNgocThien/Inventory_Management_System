#ifndef WATERPRODUCT_H
#define WATERPRODUCT_H

#include "product.h"

class WaterProduct : public Product
{
private:
    QString material;
    double temperatureLimit;

public:
    WaterProduct(const ProductParam& productInfo);

    ProductType GetType() const override;

    QString GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;

    void SetExtraData1(const QString& extra1) override;
    void SetExtraData2(const double& extra2) override;
    void SetExtraData3(const double& extra3) override;
};

#endif // WATERPRODUCT_H
