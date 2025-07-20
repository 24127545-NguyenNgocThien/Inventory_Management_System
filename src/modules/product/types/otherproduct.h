#ifndef OTHERPRODUCT_H
#define OTHERPRODUCT_H

#include "product.h"

class OtherProduct : public Product
{
public:
    OtherProduct(const ProductParam& productInfo);

    ProductType GetType() const override;

    QString GetExtraData1() const override;
    double GetExtraData2() const override;
    double GetExtraData3() const override;

    void SetExtraData1(const QString& extra1) override;
    void SetExtraData2(const double& extra2) override;
    void SetExtraData3(const double& extra3) override;
};

#endif // OTHERPRODUCT_H
