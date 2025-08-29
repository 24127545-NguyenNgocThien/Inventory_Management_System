#include "deletecommand.h"
#include "data.h"
#include "log.h"

DeleteCommand::DeleteCommand(const std::string& Id)
    : id(Id)
{
    Log::Info(QString("Khởi tạo DeleteCommand cho sản phẩm id=%1")
                  .arg(QString::fromStdString(id)));
}

bool DeleteCommand::Execute()
{
    auto list = Database::GetInstance().GetProduct(id);
    deletedProduct = list[id];
    if(deletedProduct == nullptr)
    {
        Log::Warning(QString("Xóa sản phẩm thất bại: không tìm thấy id=%1")
                         .arg(QString::fromStdString(id)));
        return false;
    }

    Database::GetInstance().DeleteProduct(id);

    Log::Info(QString("Đã xóa sản phẩm thành công: %1 (name=%2, brand=%3)")
                  .arg(QString::fromStdString(id))
                  .arg(deletedProduct->GetName())
                  .arg(deletedProduct->GetBrand()));

    return true;
}

void DeleteCommand::Undo()
{
    Database::GetInstance().AddProduct(deletedProduct);
    Log::Info(QString("Undo xóa sản phẩm: %1 (đã khôi phục lại)")
                  .arg(QString::fromStdString(id)));
}
