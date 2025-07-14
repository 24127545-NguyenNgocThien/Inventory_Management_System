#include "app.h"
#include "productFactory.h"

#include <sstream>
#include <QDebug>

//----------------LOGIC TẠO INSTANCE DUY NHẤT-------------------
std::unique_ptr<App> App::instance;
std::mutex App::mtx;

App::App() {};

App& App::GetInstance()
{
    std::lock_guard<std::mutex> lock(mtx);
    if(instance == nullptr)
    {
        instance = std::unique_ptr<App>(new App());
    }
    return *instance;
}

//----------------Số lượng sản phẩm trong kho---------------------
int App::GetNumberProduct()
{
    return manager.size();
}

//----------------------------LOGIC THÊM SẢN PHẨM ----------------------------
//check sản phẩm đã tồn tại chx
bool App::isProductExists(ProductType type, const std::string& name, const std::string& brand)
{
    for (const auto& pair : manager)
    {
        if (pair.second->GetType() == type &&
            QString::fromStdString(pair.second->GetName()) == name &&
            QString::fromStdString(pair.second->GetBrand()) == brand)
        {
            return true;
        }
    }
    return false;
}

//khai báo ban đầu cho các hàm hỗ trợ việc tạo ra newID không trùng lặp
std::string getFirstLetters(const std::string& input);
std::string CreateId(ProductType type, std::string name, std::string brand,
                     const std::map<std::string, std::shared_ptr<Product>>& manager);

//hàm thêm mới
bool App::AddProduct(ProductType type, const std::string name,const std::string brand,
                    const int& quantity, const double& price, const std::string& extra1
                    , const double& extra2, const double& extra3)
{
    // Kiểm tra trùng thông tin trước
    if (isProductExists(type, name, brand)) {
        return false;
    }

    std::string id = CreateId(type, name, brand, manager);

    manager[id] = ProductFactory::CreateProduct(type, name, brand, id, quantity,
                                                price, extra1, extra2, extra3);

    return true;
}

//------------------------------XÓA SẢN PHẨM TỒN TẠI--------------------------------
bool App::RemoveProduct(const std::string& ID)
{
    auto target = manager.find(ID);
    if(target == manager.end())
    {
        return false;
    }

    manager.erase(target);

    return true;
}


//-----------------------------HÀM LẤY SẢN PHẨM THEO ID / STT----------------------
std::map<std::string, std::shared_ptr<Product>> App::GetProductById(const std::string& id)
{
    if(id == "")
    {
        return manager;
    }

    std::map<std::string, std::shared_ptr<Product>> ret;

    auto it = manager.lower_bound(id);

    while(it != manager.end() && !it->first.compare(0, id.length(), id))
    {
        ret.insert(*it);
        it++;
    }

    return ret;
}

std::shared_ptr<Product> App::GetProductBySTT(int stt)
{
    if (stt < 0 || stt >= GetNumberProduct())
    {
        return nullptr;
    }

    auto it = manager.begin();
    std::advance(it, stt);

    if(it != manager.end())
    {
        return it->second;
    }

    return nullptr;
}

//CÁC HÀM HỖ TRỢ ĐÃ KHAI BÁO Ở TRÊN
std::string getFirstLetters(const std::string& input)
{
    std::istringstream iss(input);
    std::string word;
    std::string result;
    while (iss >> word)
    {
        if (!word.empty())
        {
            result += toupper(word[0]);
        }
    }

    return result;
}

std::string CreateId(ProductType type, std::string name, std::string brand,
                    const std::map<std::string, std::shared_ptr<Product>>& manager)
{
    std::string baseId;
    switch(type)
    {
    case ProductType::Electric: baseId = "E"; break;
    case ProductType::Water: baseId = "W"; break;
    case ProductType::Other: baseId = "O"; break;
    }

    baseId += getFirstLetters(name) + getFirstLetters(brand);

    int idx  = 1;
    std::string newId;
    do
    {
        newId = baseId + std::to_string(idx);
        idx++;
    } while (manager.find(newId) != manager.end());

    return newId;
}
