#include "importCommand.h"
#include <data.h>
#include "log.h"

ImportCommand::ImportCommand(const std::string& id, int quantity)
    :productId(id), amount(quantity), previousQuantity(0)
{
    Log::Info(QString("Khởi tạo ImportCommand: id=%1, amount=%2")
                  .arg(QString::fromStdString(productId))
                  .arg(amount));
}

bool ImportCommand::Execute()
{
    if (amount <= 0)
    {
        Log::Warning(QString("Nhập kho thất bại: số lượng không hợp lệ (id=%1, amount=%2)")
                         .arg(QString::fromStdString(productId))
                         .arg(amount));
        return false;
    }
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if (it == list.end() || !it->second)
    {
        Log::Error(QString("Nhập kho thất bại: không tìm thấy sản phẩm id=%1")
                       .arg(QString::fromStdString(productId)));
        return false;
    }

    previousQuantity = it->second->GetQuantity();
    it->second->SetQuantity(previousQuantity + amount);

    Log::Info(QString("Nhập kho thành công: id=%1, thêm=%2, tổng=%3")
                  .arg(QString::fromStdString(productId))
                  .arg(amount)
                  .arg(previousQuantity + amount));
    return true;
}

void ImportCommand::Undo()
{
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if (it == list.end() || !it->second)
    {
        Log::Error(QString("Undo Import thất bại: không tìm thấy sản phẩm id=%1")
                       .arg(QString::fromStdString(productId)));
        return;
    }

    it->second->SetQuantity(previousQuantity);
    Log::Info(QString("Undo Import: khôi phục số lượng sản phẩm id=%1 về %2")
                  .arg(QString::fromStdString(productId))
                  .arg(previousQuantity));
}
