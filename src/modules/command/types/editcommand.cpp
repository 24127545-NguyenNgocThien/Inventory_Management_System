#include "editcommand.h"
#include "data.h"
#include "otherutils.h"
#include "productfactory.h"

EditCommand::EditCommand(const std::string& proId, const ProductParam& info)
    : oldProductId(proId), newInfo(info)
{
    auto list = Database::GetInstance().GetProduct(oldProductId);
    oldProduct = list[oldProductId];

    list = Database::GetInstance().GetProduct("");
    newInfo.id = CreateId(oldProduct->GetType(), newInfo.name, newInfo.brand, list);
}

bool EditCommand::Execute()
{
    if(newInfo.quantity < 0 || newInfo.salePrice <= 0 || newInfo.importPrice <= 0)
    {
        return false;
    }

    //app ko cho người dùng edit id và type nhưng id có thể bị thay đổi
    //thông qua name & brand.
    //Nếu vào th đó thì buộc phải tạo mới và xóa cũ
    if(oldProduct->GetId() != newInfo.id)
    {
        auto newProduct = ProductFactory::CreateProduct(oldProduct->GetType(), newInfo);
        Database::GetInstance().AddProduct(newProduct);
        Database::GetInstance().DeleteProduct(oldProduct->GetId());
    }

    Database::GetInstance().EditProduct(oldProduct, newInfo);

    return true;
}

void EditCommand::Undo()
{
    if(oldProduct->GetId() != newInfo.id)
    {
        Database::GetInstance().AddProduct(oldProduct);
        Database::GetInstance().DeleteProduct(newInfo.id);
    }

    Database::GetInstance().EditProduct(oldProduct, newInfo);
}
