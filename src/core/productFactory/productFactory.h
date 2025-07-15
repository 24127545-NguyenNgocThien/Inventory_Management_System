#pragma once
#include "product.h"

#include <memory>

class ProductFactory
{
public:
    //creator
    static std::shared_ptr<Product> CreateProduct(ProductType type, const QString name,
                                        const QString brand, const std::string id,
                                        const int& quantity, const double& price,
                                        const QString& extra1,
                                        const double& extra2,const double& extra3);
};
