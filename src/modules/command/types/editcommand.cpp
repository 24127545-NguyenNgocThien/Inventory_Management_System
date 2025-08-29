#include "editcommand.h"
#include "data.h"
#include "otherutils.h"
#include "productfactory.h"
#include "log.h"

EditCommand::EditCommand(const std::string& proId, const ProductParam& info)
    : oldProductId(proId), newInfo(info)
{
    auto list = Database::GetInstance().GetProduct(oldProductId);
    oldProduct = list[oldProductId];

    if(!oldProduct) {
        Log::Warning(QString("Khởi tạo EditCommand thất bại: không tìm thấy sản phẩm id=%1")
                         .arg(QString::fromStdString(oldProductId)));
        return;
    }

    list = Database::GetInstance().GetProduct("");
    newInfo.id = CreateId(oldProduct->GetType(), newInfo.name, newInfo.brand, list);

    Log::Info(QString("Khởi tạo EditCommand cho sản phẩm id=%1 (sẽ đổi thành id=%2)")
                  .arg(QString::fromStdString(oldProductId))
                  .arg(QString::fromStdString(newInfo.id)));
}

bool EditCommand::Execute()
{
    if(newInfo.quantity < 0 || newInfo.salePrice <= 0 || newInfo.importPrice <= 0)
    {
        Log::Warning(QString("Chỉnh sửa thất bại: thông tin không hợp lệ (id=%1)")
                         .arg(QString::fromStdString(oldProductId)));
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

        Log::Info(QString("Chỉnh sửa sản phẩm id=%1 → tạo mới id=%2")
                      .arg(QString::fromStdString(oldProductId))
                      .arg(QString::fromStdString(newInfo.id)));
    }

    Database::GetInstance().EditProduct(oldProduct, newInfo);

    Log::Info(QString("Đã chỉnh sửa sản phẩm thành công: id=%1, name=%2, brand=%3")
                  .arg(QString::fromStdString(newInfo.id))
                  .arg(newInfo.name)
                  .arg(newInfo.brand));

    return true;
}

void EditCommand::Undo()
{
    if(oldProduct->GetId() != newInfo.id)
    {
        Database::GetInstance().AddProduct(oldProduct);
        Database::GetInstance().DeleteProduct(newInfo.id);

        Log::Info(QString("Undo chỉnh sửa: khôi phục id=%1, xóa id mới=%2")
                      .arg(QString::fromStdString(oldProductId))
                      .arg(QString::fromStdString(newInfo.id)));
    }

    Database::GetInstance().EditProduct(oldProduct, newInfo);

    Log::Info(QString("Undo chỉnh sửa: phục hồi dữ liệu cũ cho sản phẩm id=%1")
                  .arg(QString::fromStdString(oldProductId)));
}
