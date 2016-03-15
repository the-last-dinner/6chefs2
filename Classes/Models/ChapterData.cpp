//
//  ChapterData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/16.
//
//

#include "ChapterData.h"

// 初期化
bool ChapterData::init()
{
    string path = FileUtils::getInstance()->fullPathForFilename("csv/chapter.csv");
    if (path == "") return false;
    this->data = CsvUtils::readCsvFile(path);
    return true;
}

// チャプター名を取得
string ChapterData::getName(const int chapter_id)
{
    return this->data[chapter_id][etoi(ChapterData::Keys::NAME)];
}

// チャプターのタイトルを取得
string ChapterData::getTitle(const int chapter_id)
{
    return this->data[chapter_id][etoi(ChapterData::Keys::TITLE)];
}