#include "exportCommand.h"
#include <data.h>
#include "log.h"

ExportCommand::ExportCommand(const std::string& id, int quantity)
    :productId(id), amount(quantity), previousQuantity(0)
{
    Log::Info(QString("Khởi tạo ExportCommand: id=%1, amount=%2")
                  .arg(QString::fromStdString(productId))
                  .arg(amount));
}

bool ExportCommand::Execute()
{
    if(amount <= 0)
    {
        Log::Warning(QString("Xuất kho thất bại: số lượng không hợp lệ (id=%1, amount=%2)")
                         .arg(QString::fromStdString(productId))
                         .arg(amount));
        return false;
    }
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if(it == list.end() || !it->second) return false;

    previousQuantity = it->second->GetQuantity();
    if (previousQuantity < amount)
    {
        Log::Warning(QString("Xuất kho thất bại: không đủ hàng (id=%1, tồn=%2, yêu cầu=%3)")
                         .arg(QString::fromStdString(productId))
                         .arg(previousQuantity)
                         .arg(amount));
        return false;
    }

    it->second->SetQuantity(previousQuantity - amount);
    Log::Info(QString("Xuất kho thành công: id=%1, trừ=%2, còn lại=%3")
                  .arg(QString::fromStdString(productId))
                  .arg(amount)
                  .arg(previousQuantity - amount));
    return true;
}

void ExportCommand::Undo()
{
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if(it == list.end() || !it->second)
    {
        Log::Error(QString("Undo Export thất bại: không tìm thấy sản phẩm id=%1")
                       .arg(QString::fromStdString(productId)));
        return;
    }
    it->second->SetQuantity(previousQuantity);

    Log::Info(QString("Undo Export: khôi phục số lượng sản phẩm id=%1 về %2")
                  .arg(QString::fromStdString(productId))
                  .arg(previousQuantity));
}
