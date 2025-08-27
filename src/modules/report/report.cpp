#include "report.h"
#include <data.h>

QMap<QString, double> Report::RevenueByDay(const std::vector<Invoice>& invoices)
{
    QMap<QString, double> result;
    for (const auto& inv : invoices)
    {
        QString key = inv.GetCreatedAt().date().toString("yyyy-MM-dd");
        result[key] += inv.Total();
    }
    return result;
}

QMap<QString, double> Report::RevenueByMonth(const std::vector<Invoice>& invoices)
{
    QMap<QString, double> result;
    for (const auto& inv : invoices)
    {
        QString key = inv.GetCreatedAt().date().toString("yyyy-MM");
        result[key] += inv.Total();
    }
    return result;
}

double Report::ProfitTotal(const std::vector<Invoice>& invoices)
{
    double profit = 0.0;
    auto& db = Database::GetInstance();
    for (const auto& inv : invoices)
    {
        for (const auto& it : inv.GetItems())
        {
            auto map = db.GetProduct(it.productId);
            auto found = map.find(it.productId);
            if (found == map.end() || !found->second) continue;
            double importPrice = found->second->GetImportPrice();
            profit += (it.salePrice - importPrice) * it.quantity;
        }
    }
    return profit;
}

std::pair<std::string,int> Report::BestSellingProduct(const std::vector<Invoice>& invoices)
{
    std::map<std::string, int> sold;
    for (const auto& inv : invoices)
        for (const auto& it : inv.GetItems())
            sold[it.productId] += it.quantity;

    std::pair<std::string,int> best{"", 0};
    for (const auto& kv : sold)
        if (kv.second > best.second) best = kv;

    return best;
}
