#include "exportCommand.h"
#include <data.h>
ExportCommand::ExportCommand(const std::string& id, int quantity)
    :productId(id), amount(quantity), previousQuantity(0)
{}

bool ExportCommand::Execute()
{
    if(amount <= 0) return false;
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if(it == list.end() || !it->second) return false;

    previousQuantity = it->second->GetQuantity();
    if (previousQuantity < amount) return false; // không đủ hàng

    it->second->SetQuantity(previousQuantity - amount);
    return true;
}

void ExportCommand::Undo()
{
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if(it == list.end() || !it->second) return;
    it->second->SetQuantity(previousQuantity);
}
