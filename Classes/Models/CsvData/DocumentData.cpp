//
//  DocumentData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/15.
//
//

#include "Models/CsvData/DocumentData.h"

// 初期化
bool DocumentData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename(Resource::CsvFiles::DOCUMENT);
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// 資料名の取得
string DocumentData::getName(const int docId)
{
    return this->data[docId][etoi(DocumentData::Keys::NAME)];
}

// 画像ファイル名の取得
string DocumentData::getImageFileName(const int docId)
{
    return this->data[docId][etoi(DocumentData::Keys::IMAGE)];
}

// キャラクターの説明をレベルごとに取得
// @param int level 0 ~ 2
string DocumentData::getDiscription(const int docId, const int level)
{
    return this->data[docId][level + etoi(DocumentData::Keys::PROFILE0)];
}

// 資料の全要素を取得
vector<int> DocumentData::getDocumentDataIds()
{
    vector<int> ids = {};
    for (auto itr : this->data)
    {
        ids.push_back(itr.first);
    }
    return ids;
}

// 資料の要素数を取得
int DocumentData::getDocumentCount()
{
    int count = this->data.size();
    return count;
}