//
//  ItemData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/15.
//
//

#include "Models/CsvData/ItemData.h"

// 初期化
bool ItemData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename(Resource::CsvFiles::ITEM);
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// アイテム名の取得
string ItemData::getItemName(const int itemId)
{
    return (itemId > etoi(ItemID::UNDIFINED)) ? this->data[itemId][etoi(ItemData::Keys::NAME)] : "アイテムがありません";
}

// アイテム説明の取得
string ItemData::getItemDiscription(const int itemId)
{
    return (itemId > etoi(ItemID::UNDIFINED)) ? this->data[itemId][etoi(ItemData::Keys::DISCRIPTION)] : "アイテムがありません";
}

// アイテム装備時のイベント
string ItemData::getItemEquipInitEvent(const int itemId)
{
    return (itemId > etoi(ItemID::UNDIFINED)) ? this->data[itemId][etoi(ItemData::Keys::EQUIP_INIT_EVENT)] : "";
}

// アイテム装備中のマップ移動時のイベント
string ItemData::getItemEquipUpdateEvent(const int itemId)
{
    return (itemId > etoi(ItemID::UNDIFINED)) ? this->data[itemId][etoi(ItemData::Keys::EQUIP_UPDATE_EVENT)] : "";
}

// アイテム装備解除時イベント
string ItemData::getItemEquipEndEvent(const int itemId)
{
    return (itemId > etoi(ItemID::UNDIFINED)) ? this->data[itemId][etoi(ItemData::Keys::EQUIP_END_EVENT)] : "";
}
