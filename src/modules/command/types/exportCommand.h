#ifndef EXPORTCOMMAND_H
#define EXPORTCOMMAND_H

#include "command.h"
#include <string>

class ExportCommand : public Command
{
private:
    std::string productId;
    int amount;
    int previousQuantity;

public:
    ExportCommand(const std::string& id, int quantity);

    bool Execute() override;
    void Undo() override;
};

#endif // EXPORTCOMMAND_H
