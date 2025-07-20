#ifndef OTHERUTILS_H
#define OTHERUTILS_H
#pragma once

#include <QString>
#include <QStringList>
#include <string>
#include "product.h"
#include <map>
#include <memory>


//=========================hàm bỏ dấu=============================
inline QString removeDiacritics(const QString &text)
{
    QString normalized = text.normalized(QString::NormalizationForm_D); // Tách dấu ra khỏi chữ cái
    QString result;
    result.reserve(normalized.size());

    for (QChar ch : std::as_const(normalized))
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

//=========================hàm lấy chữ cái đầu mỗi từ=============================
inline std::string getFirstLetters(const QString& input)
{
    QString noAccentText = removeDiacritics(input); // Bỏ dấu trước
    QStringList words = noAccentText.split(" ", Qt::SkipEmptyParts);
    QString result;
    result.reserve(words.size()); // Tối ưu hóa

    // Sửa cảnh báo deprecated qAsConst bằng std::as_const
    for (const QString &word : std::as_const(words))
    {
        if (!word.isEmpty()) {
            result += word.at(0).toUpper(); // Lấy chữ cái đầu và viết hoa
        }
    }

    return result.toStdString();
}



//========================hàm tạo id=========================
inline std::string CreateId(ProductType type, QString name, QString brand,
                            const std::map<std::string, std::shared_ptr<Product>>& list)
{
    std::string baseId;
    switch(type)
    {
    case ProductType::Electric: baseId = "E"; break;
    case ProductType::Water: baseId = "W"; break;
    case ProductType::Other: baseId = "O"; break;
    default: break;
    }

    baseId += getFirstLetters(name) + getFirstLetters(brand);

    int idx  = 1;
    std::string newId;
    do
    {
        newId = baseId + std::to_string(idx);
        idx++;
    } while (list.find(newId) != list.end());

    return newId;
}


//==================hàm xác định kiểu product===================
inline ProductType DetermineType(int index)
{
    switch(index)
    {
    case 1: return ProductType::Electric;
    case 2: return ProductType::Water;
    case 3: return ProductType::Other;
    default: return ProductType::None;
    }
}

#endif // OTHERUTILS_H
