#include "invoiceService.h"
#include "exportCommand.h"
#include "data.h"

bool InvoiceService::CreateInvoice(const std::vector<InvoiceItem>& items,
                                   CommandManager* cmdManager)
{
    Invoice* newInv = new Invoice();
    std::vector<Command*> executed; executed.reserve(items.size());
    for(const auto& it : items)
    {
        Command* cmd = new ExportCommand(it.productId, it.quantity);
        if (!cmdManager->ExecuteCommand(cmd))
        {
            // rollback các lệnh đã chạy
            for(auto it2 = executed.rbegin(); it2 != executed.rend(); ++it2)
                cmdManager->Undo();
            delete cmd;
            for(auto* c : executed) delete c;
            return false;
        }
        executed.push_back(cmd);
        newInv->AddItem(it.productId, it.quantity, it.salePrice);
    }

    Database::GetInstance().AddInvoice(newInv);
    Database::GetInstance().SaveInvoices();
    // Giữ commands trong undo stack để người dùng có thể Undo hoá đơn nếu muốn.
    return true;
}
