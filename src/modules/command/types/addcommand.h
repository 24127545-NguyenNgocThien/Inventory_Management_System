#ifndef ADDCOMMAND_H
#define ADDCOMMAND_H

#include "command.h"
#include "product.h"

class AddCommand : public Command
{
private:
    ProductType type;
    ProductParam productInfo;

public:
    AddCommand(const ProductType&, const ProductParam&);

    bool Execute() override;
    void Undo() override;
};

#endif // ADDCOMMAND_H
