#include "data.h"

std::unique_ptr<Database> Database::instance;
std::mutex Database::mtx;

Database::Database() : filePath("inventory.json")
{}

Database& Database::GetInstance()
{
    std::lock_guard<std::mutex> lock(mtx);
    if(instance == nullptr)
    {
        instance = std::unique_ptr<Database>(new Database());
    }
    return *instance;
}

//=========================================================================

int Database::GetNumberProduct()
{
    return productsList.size();
}

//=========================================================================

void Database::AddProduct(std::shared_ptr<Product> product)
{
    productsList[product->GetId()] = product;
}

void Database::DeleteProduct(const std::string& id)
{
    auto target = productsList.find(id);
    if(target == productsList.end())
    {
        return;
    }

    productsList.erase(target);

    return;
}

void Database::EditProduct(std::shared_ptr<Product> oldProduct, const ProductParam& newInfo)
{
    if(oldProduct->GetName() != newInfo.name)
        oldProduct->SetName(newInfo.name);
    if(oldProduct->GetBrand() != newInfo.brand)
        oldProduct->SetBrand(newInfo.brand);
    if(oldProduct->GetQuantity() != newInfo.quantity)
        oldProduct->SetQuantity(newInfo.quantity);
    if(oldProduct->GetPrice() != newInfo.price)
        oldProduct->SetPrice(newInfo.price);

    if(oldProduct->GetExtraData1() != newInfo.extra1)
        oldProduct->SetExtraData1(newInfo.extra1);
    if(oldProduct->GetExtraData2() != newInfo.extra2)
        oldProduct->SetExtraData2(newInfo.extra2);
    if(oldProduct->GetExtraData3() != newInfo.extra3)
        oldProduct->SetExtraData3(newInfo.extra3);
}

std::map<std::string, std::shared_ptr<Product>> Database::GetProduct(const std::string& id)
{
    if(id.empty())
    {
        return productsList;
    }

    //vì id là do chương trình tự tạo và gán cho mỗi product nên
    //nếu người dùng ko nhớ rõ id cụ thể thì họ chỉ có thể tìm
    //bằng bằng thông tin: type, name, brand thôi
    std::map<std::string, std::shared_ptr<Product>> ret;

    auto it = productsList.lower_bound(id);

    while(it != productsList.end() && !it->first.compare(0, id.length(), id))
    {
        ret.insert(*it);
        it++;
    }

    return ret;
}
