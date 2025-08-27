#ifndef REPORT_H
#define REPORT_H

#include <QDate>
#include <QMap>
#include <QString>
#include "invoice.h"

class Report
{
public:
    // Doanh thu theo ngày (key = yyyy-MM-dd)
    static QMap<QString, double> RevenueByDay(const std::vector<Invoice>& invoices);

    // Doanh thu theo tháng (key = yyyy-MM)
    static QMap<QString, double> RevenueByMonth(const std::vector<Invoice>& invoices);

    // Lợi nhuận tổng = Σ (giá bán - giá nhập) * quantity
    static double ProfitTotal(const std::vector<Invoice>& invoices);

    // Sản phẩm bán chạy nhất (trả về <productId, quantitySold>)
    static std::pair<std::string,int> BestSellingProduct(const std::vector<Invoice>& invoices);
};

#endif // REPORT_H
