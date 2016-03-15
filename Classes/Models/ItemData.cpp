//
//  ItemData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/15.
//
//

#include "Models/ItemData.h"

// 初期化
bool ItemData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename("csv/item.csv");
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// アイテム名の取得
string ItemData::getItemName(const int item_id)
{
    return (item_id >= 0) ? this->data[item_id][etoi(ItemData::Keys::NAME)] : "アイテムがありません";
}

// アイテム説明の取得
string ItemData::getItemDiscription(const int item_id)
{
    return (item_id >= 0) ? this->data[item_id][etoi(ItemData::Keys::DISCRIPTION)] : "アイテムがありません";
}
