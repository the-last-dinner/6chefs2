//
//  MapData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#include "Models/CsvData/MapData.h"

// 初期化
bool MapData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename(Resource::CsvFiles::MAP);
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// マップ名を取得
string MapData::getName(const int map_id)
{
    return this->data[map_id][etoi(MapData::Keys::NAME)];
}

// マップのファイル名を取得
string MapData::getFileName(const int map_id)
{
    return this->data[map_id][etoi(MapData::Keys::FILE_NAME)];
}

// マップのファイル名を全取得
vector<string> MapData::getFileNameAll()
{
    vector<string> fileNames {};
    for (auto itr:this->data)
    {
        fileNames.push_back(itr.second[etoi(MapData::Keys::FILE_NAME)]);
    }
    return fileNames;
}