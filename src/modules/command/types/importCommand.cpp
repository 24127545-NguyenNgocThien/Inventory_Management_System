#include "importCommand.h"
#include <data.h>

ImportCommand::ImportCommand(const std::string& id, int quantity)
    :productId(id), amount(quantity), previousQuantity(0)
{}

bool ImportCommand::Execute()
{
    if (amount <= 0) return false;
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if (it == list.end() || !it->second) return false;

    previousQuantity = it->second->GetQuantity();
    it->second->SetQuantity(previousQuantity + amount);
    return true;
}

void ImportCommand::Undo()
{
    auto list = Database::GetInstance().GetProduct(productId);
    auto it = list.find(productId);
    if (it == list.end() || !it->second) return;
    it->second->SetQuantity(previousQuantity);
}
