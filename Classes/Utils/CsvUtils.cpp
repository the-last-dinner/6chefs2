//
//  CsvUtils.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/15.
//
//

#include "Utils/CsvUtils.h"
#include "Utils/StringUtils.h"
#include "Managers/DebugManager.h"

CsvUtils::CsvMap CsvUtils::readCsvFile(const string& file_name)
{
    //ファイル読み込み
    ifstream file(file_name);
    CsvUtils::CsvMap values;
    string str;
    int p, i, data_id;
    
    //ファイル読み込み失敗時
    if(file.fail())
    {
        CCLOG("Reading csv file of %s is failed.", file_name.c_str());
        return values;
    }
    //csvデータ格納
    while(getline(file, str)){
        // 複合化
        if (!DebugManager::getInstance()->isPlainData())
        {
            LastSupper::StringUtils::encryptXor(str);
        }
        
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
