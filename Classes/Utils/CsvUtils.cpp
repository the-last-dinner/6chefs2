//
//  CsvUtils.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/15.
//
//

#include "Utils/CsvUtils.h"
#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"
#include "Managers/DebugManager.h"

CsvUtils::CsvMap CsvUtils::readCsvFile(const string& path)
{
    // ファイル読み込み
    ifstream file(path);
    CsvUtils::CsvMap values;
    string str;
    int p, i, data_id;
    
    // ファイル読み込み失敗時
    if(file.fail())
    {
        CCLOG("Reading csv file of %s is failed.", path.c_str());
        return values;
    }
    
    // jsonで暗号化されている場合の複合化
    if (!DebugManager::getInstance()->isPlainData())
    {
        return CsvUtils::readJsonForCsv(path);
    }
    
    // csvデータ格納
    while(getline(file, str)){
        // 複合化
        // if (!DebugManager::getInstance()->isPlainData())
        // {
        //     LastSupper::StringUtils::encryptXor(str);
        // }
        
        //コメント箇所は除く
        if( (p = str.find("//")) != str.npos ) continue;
        vector<string> inner;
        i = 0;
        //カンマがあるかを探し、そこまでをvaluesに格納
        while( (p = str.find(",")) != str.npos ){
            if (i == 0)
            {
                data_id = stoi(str.substr(0, p));
            }
            inner.push_back(str.substr(0, p));
            //strの中身は","の1文字を飛ばす
            str = str.substr(p+1);
            i++;
        }
        inner.push_back(str);
        values[data_id] = inner;
    }
    return values;
}

// CSVファイルをXOR暗号化して書き出し
bool CsvUtils::encryptCsvFile(const string& path)
{
    // ファイル読み込み
    ifstream ifs(path);
    if (ifs.fail())
    {
        CCLOG("%s is missing.", path.c_str());
        return false;
    }
    
    // 文字列を暗号化
    string str;
    getline(ifs, str);
    ofstream ofs;
    ofs.open(path);
    while(getline(ifs, str))
    {
        LastSupper::StringUtils::encryptXor(str);
        ofs << str << endl;
    }
    ifs.close();
    ofs.close();
    return true;
}

// csvMapをjsonオブジェクトに変換
rapidjson::Document CsvUtils::csvMapToJson(CsvUtils::CsvMap& csvMap)
{
    rapidjson::Document json;
    json.SetObject();
    for (auto itr:csvMap)
    {
        rapidjson::Value idStr (kStringType);
        const char* idChar = to_string(itr.first).c_str();
        idStr.SetString(idChar, strlen(idChar), json.GetAllocator());
        json.AddMember(idStr, rapidjson::Value(), json.GetAllocator());
        json[idChar].SetArray();
        for (string val:itr.second)
        {
            rapidjson::Value strVal (kStringType);
            strVal.SetString(val.c_str(), strlen(val.c_str()), json.GetAllocator());
            json[idChar].PushBack(strVal, json.GetAllocator());
        }
    }
    return json;
}

// csvをjsonに変換して暗号化
void CsvUtils::encryptCsvToJson(const string &path)
{
    CsvUtils::CsvMap csvMap {CsvUtils::readCsvFile(path)};
    rapidjson::Document doc {CsvUtils::csvMapToJson(csvMap)};
    LastSupper::JsonUtils::writeJsonCrypt(path, doc);
}

// csvをjson化したものを複合化して取得
CsvUtils::CsvMap CsvUtils::readJsonForCsv(const string &path)
{
    CsvUtils::CsvMap csvMap;
    vector<string> line;
    rapidjson::Document json {LastSupper::JsonUtils::readJsonCrypted(path)};
    for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); itr++)
    {
        int lineSize {static_cast<int>(itr->value.Size())};
        for (int j = 0; j < lineSize; j++)
        {
            line.push_back(itr->value[j].GetString());
        }
        csvMap[stoi(itr->name.GetString())] = line;
        line.clear();
    }
    return csvMap;
}
