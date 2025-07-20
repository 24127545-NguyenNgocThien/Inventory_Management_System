#ifndef EDITCOMMAND_H
#define EDITCOMMAND_H

#include "command.h"
#include "product.h"

class EditCommand : public Command
{
private:
    std::string oldProductId;
    ProductParam newInfo;
    std::shared_ptr<Product> oldProduct;

public:
    EditCommand(const std::string&, const ProductParam&);

    bool Execute() override;
    void Undo() override;
};

#endif // EDITCOMMAND_H
