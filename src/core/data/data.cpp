#include "data.h"
#include <QJsonArray>
#include "productfactory.h"

std::unique_ptr<Database> Database::instance;
std::mutex Database::mtx;

Database::Database() : fileName("inventory.json")
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

bool Database::Save()
{
    QJsonObject rootObject;
    QJsonArray productArray;
    for(const auto& pair : productsList)
    {
        QJsonObject product;
        product["id"] = QString::fromStdString(pair.first);
        product["name"] = pair.second->GetName();
        product["brand"] = pair.second->GetBrand();
        product["quantity"] = pair.second->GetQuantity();
        product["price"] = pair.second->GetPrice();
        if(!pair.second->GetExtraData1().isEmpty())
            product["extra1"] = pair.second->GetExtraData1();
        if(!std::isnan(pair.second->GetExtraData2()))
            product["extra2"] = pair.second->GetExtraData2();
        if(!std::isnan(pair.second->GetExtraData3()))
            product["extra3"] = pair.second->GetExtraData3();

        productArray.append(product);
    }

    rootObject["product list"] = productArray;
    QJsonDocument jsonDoc(rootObject);
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Không thể mở file để ghi:" << fileName;
        return false;
    }
    file.write(jsonDoc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Đã lưu dữ liệu vào file:" << fileName;
    return true;
}

void Database::Load()
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Không thể mở file để đọc:" << fileName;
        return;
    }

    QByteArray jsonData = file.readAll();
    file.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData);
    if (jsonDoc.isNull())
    {
        qWarning() << "Dữ liệu JSON không hợp lệ";
        return;
    }

    QJsonObject rootObject = jsonDoc.object();
    if (!rootObject.contains("product list") || !rootObject["product list"].isArray())
    {
        qWarning() << "Không tìm thấy mảng sản phẩm hoặc định dạng không đúng";
        return;
    }

    QJsonArray productArray = rootObject["product list"].toArray();
    for (const QJsonValue &productValue : productArray)
    {
        if (!productValue.isObject())
        {
            qWarning() << "Sản phẩm không phải là đối tượng JSON hợp lệ";
            continue;
        }

        QJsonObject product = productValue.toObject();
        ProductParam info;
        std::shared_ptr<Product> pro;
        info.id = product["id"].toString().toStdString();
        info.name = product["name"].toString();
        info.brand = product["brand"].toString();
        info.quantity = product["quantity"].toInt();
        info.price = product["price"].toDouble();
        if (product.contains("extra1"))
            info.extra1 = product["extra1"].toString();
        if (product.contains("extra2"))
            info.extra2 = product["extra2"].toDouble();
        if (product.contains("extra3"))
            info.extra3 = product["extra3"].toDouble();
        if(info.id[0] == 'E')
            pro = ProductFactory::CreateProduct(ProductType::Electric, info);
        else if(info.id[0] == 'W')
            pro = ProductFactory::CreateProduct(ProductType::Water, info);
        else if(info.id[0] == 'O')
            pro = ProductFactory::CreateProduct(ProductType::Other, info);

        AddProduct(pro);
    }
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
