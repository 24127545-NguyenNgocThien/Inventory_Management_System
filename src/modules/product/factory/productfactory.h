#ifndef PRODUCTFACTORY_H
#define PRODUCTFACTORY_H

#include "product.h"

#include <memory>

class ProductFactory
{
public:
    //creator
    static std::shared_ptr<Product> CreateProduct
        (ProductType type, const ProductParam& productInfo);
};

#endif // PRODUCTFACTORY_H
