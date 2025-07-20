#include "deletecommand.h"
#include "data.h"

DeleteCommand::DeleteCommand(const std::string& Id)
    : id(Id)
{}

bool DeleteCommand::Execute()
{
    auto list = Database::GetInstance().GetProduct(id);
    deletedProduct = list[id];
    if(deletedProduct == nullptr)
    {
        return false;
    }

    Database::GetInstance().DeleteProduct(id);

    return true;
}

void DeleteCommand::Undo()
{
    Database::GetInstance().AddProduct(deletedProduct);
}
