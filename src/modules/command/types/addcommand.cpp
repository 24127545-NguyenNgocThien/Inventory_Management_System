#include "addcommand.h"
#include "productfactory.h"
#include "otherutils.h"
#include "data.h"

AddCommand::AddCommand(const ProductType& tp, const ProductParam& pro)
    : type(tp), productInfo(pro)
{
    auto list = Database::GetInstance().GetProduct("");
    productInfo.id = CreateId(type, productInfo.name, productInfo.brand, list);
}

bool AddCommand::Execute()
{
    auto product = ProductFactory::CreateProduct(type, productInfo);

    if(product->GetQuantity() < 0 || product->GetSalePrice() <= 0 || product->GetImportPrice() <= 0)
    {
        return false;
    }

    Database::GetInstance().AddProduct(product);

    return true;
}

void AddCommand::Undo()
{
    Database::GetInstance().DeleteProduct(productInfo.id);
}
