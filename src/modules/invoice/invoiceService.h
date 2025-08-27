#ifndef INVOICESERVICE_H
#define INVOICESERVICE_H

#include "invoice.h"
#include "commandManager.h"

class InvoiceService
{
public:
    // Tạo hoá đơn: nếu có mặt hàng nào xuất kho thất bại (thiếu hàng) thì rollback.
    static bool CreateInvoice(const std::vector<InvoiceItem>& items,
                              CommandManager& cmdManager,
                              Invoice& outInvoice);
};

#endif // INVOICESERVICE_H
