#include "app.h"
#include "productFactory.h"

#include <QDebug>
#include <QString>

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
bool App::isProductExists(ProductType type, const QString& name, const QString& brand)
{
    for (const auto& pair : manager)
    {
        if (pair.second->GetType() == type &&
            pair.second->GetName() == name &&
            pair.second->GetBrand() == brand)
        {
            return true;
        }
    }
    return false;
}

//khai báo ban đầu cho các hàm hỗ trợ việc tạo ra newID không trùng lặp
std::string getFirstLetters(const std::string& input);
std::string CreateId(ProductType type, QString name, QString brand,
                     const std::map<std::string, std::shared_ptr<Product>>& manager);

//hàm thêm mới
bool App::AddProduct(ProductType type, const QString name,const QString brand,
                    const int& quantity, const double& price, const QString& extra1
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
std::map<std::string, std::shared_ptr<Product>> App::GetProductById(const std::string& ID)
{
    if(ID == "")
    {
        return manager;
    }

    std::map<std::string, std::shared_ptr<Product>> ret;

    auto it = manager.lower_bound(ID);

    while(it != manager.end() && !it->first.compare(0, ID.length(), ID))
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
QString removeDiacritics(const QString &text)
{
    QString normalized = text.normalized(QString::NormalizationForm_D); // Tách dấu ra khỏi chữ cái
    QString result;

    for (const QChar &ch : normalized)
    {
        if (ch.category() != QChar::Mark_NonSpacing)
        { // Loại bỏ ký tự dấu (Unicode)
            result.append(ch);
        }
    }

    // Thay thế các ký tự đặc biệt (nếu cần)
    result.replace("đ", "d").replace("Đ", "D");
    return result;
}

std::string getFirstLetters(const QString& input)
{
    QString noAccentText = removeDiacritics(input); // Bỏ dấu trước
    QStringList words = noAccentText.split(" ", Qt::SkipEmptyParts);
    QString result;

    for (const QString &word : words) {
        if (!word.isEmpty()) {
            result += word.at(0).toUpper(); // Lấy chữ cái đầu và viết hoa
        }
    }

    return result.toStdString();
}

std::string CreateId(ProductType type, QString name, QString brand,
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
