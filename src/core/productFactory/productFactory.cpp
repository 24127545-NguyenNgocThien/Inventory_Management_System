#include "productFactory.h"

#include <stdexcept>

//---------------------Creator-----------------
std::shared_ptr<Product> ProductFactory::CreateProduct(ProductType type,
                        const std::string name, const std::string brand,
                        const std::string id, const int& quantity,
                        const double& price, const std::string& extra1,
                        const double& extra2, const double& extra3)
{
    switch(type)
    {
        case ProductType::Electric:
            return std::make_shared<ElectricProduct>(name, brand, id, quantity, price,
                                                    extra1, extra2, extra3);

        case ProductType::Water:
            return std::make_shared<WaterProduct>(name, brand, id, quantity, price,
                                                    extra1, extra2);

        case ProductType::Other:
            return std::make_shared<OtherProduct>(name, brand, id, quantity, price);

        default:
            throw std::invalid_argument("Invalid Product Type!");
    }
}
