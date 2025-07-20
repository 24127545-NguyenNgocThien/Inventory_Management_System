#include "productfactory.h"
#include "electricproduct.h"
#include "waterproduct.h"
#include "otherproduct.h"

#include <stdexcept>

//---------------------Creator-----------------
std::shared_ptr<Product> ProductFactory::CreateProduct
    (ProductType type,const ProductParam& productInfo)
{
    switch(type)
    {
    case ProductType::Electric:
        return std::make_shared<ElectricProduct>(productInfo);

    case ProductType::Water:
        return std::make_shared<WaterProduct>(productInfo);

    case ProductType::Other:
        return std::make_shared<OtherProduct>(productInfo);

    default:
        throw std::invalid_argument("Invalid Product Type!");
    }
}
