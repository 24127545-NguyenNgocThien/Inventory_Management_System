#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H

#include "command.h"
#include "product.h"
#include <memory>

class DeleteCommand : public Command
{
private:
    std::string id;
    std::shared_ptr<Product> deletedProduct;

public:
    DeleteCommand(const std::string&);

    bool Execute() override;
    void Undo() override;
};

#endif // DELETECOMMAND_H
