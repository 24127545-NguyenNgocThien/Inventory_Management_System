#pragma once
#include "product.h"

#include <memory>

class ProductFactory
{
public:
    //creator
    static std::shared_ptr<Product> CreateProduct(ProductType type, const std::string name,
                                        const std::string brand, const std::string id,
                                        const int& quantity, const double& price,
                                        const std::string& extra1,
                                        const double& extra2,const double& extra3);
};
