#include "addcommand.h"
#include "productfactory.h"
#include "otherutils.h"
#include "data.h"
#include "log.h"

AddCommand::AddCommand(const ProductType& tp, const ProductParam& pro)
    : type(tp), productInfo(pro)
{
    auto list = Database::GetInstance().GetProduct("");
    productInfo.id = CreateId(type, productInfo.name, productInfo.brand, list);

    Log::Info(QString("Khởi tạo AddCommand cho sản phẩm: %1 (brand: %2)")
                  .arg(productInfo.name)
                  .arg(productInfo.brand));
}

bool AddCommand::Execute()
{
    auto product = ProductFactory::CreateProduct(type, productInfo);

    if(product->GetQuantity() < 0 || product->GetSalePrice() <= 0 || product->GetImportPrice() <= 0)
    {
        Log::Warning(QString("Thêm sản phẩm thất bại: dữ liệu không hợp lệ (id=%1)")
                         .arg(QString::fromStdString(productInfo.id)));
        return false;
    }

    Database::GetInstance().AddProduct(product);

    Log::Info(QString("Đã thêm sản phẩm thành công: %1 (số lượng: %2, giá bán: %3)")
                  .arg(QString::fromStdString(productInfo.id))
                  .arg(product->GetQuantity())
                  .arg(product->GetSalePrice()));

    return true;
}

void AddCommand::Undo()
{
    Database::GetInstance().DeleteProduct(productInfo.id);
    Log::Info(QString("Undo thêm sản phẩm, đã xóa sản phẩm: %1")
                  .arg(QString::fromStdString(productInfo.id)));
}
