#ifndef IMPORTCOMMAND_H
#define IMPORTCOMMAND_H

#include "command.h"
#include <string>

class ImportCommand : public Command
{
private:
    std::string productId;
    int amount;
    int previousQuantity;

public:
    ImportCommand(const std::string& id, int quantity);

    bool Execute() override;
    void Undo() override;
};

#endif // IMPORTCOMMAND_H
