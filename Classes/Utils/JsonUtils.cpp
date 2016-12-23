//
//  JsonUtils.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#include "Utils/AssertUtils.h"

#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"
#include "Managers/ConfigDataManager.h"

// JSONファイルの読み込み
rapidjson::Document LastSupper::JsonUtils::readJsonFile(const string& path)
{
    const char* cpath = path.c_str();
    rapidjson::Document doc {nullptr};
    FILE* fp;
    char buf[512];
    
    // ファイル操作
    fp = fopen(cpath, "rb");
    FileReadStream rs(fp, buf, sizeof(buf));
    doc.ParseStream(rs);
    fclose(fp);
    
    //構文エラー判定
    bool error = doc.HasParseError();
    if(error){
        AssertUtils::fatalAssert("JSON Parse Error!\n" + path);
        return nullptr;
    }
    
    return doc;
}

// JSONファイルの書き出し
void LastSupper::JsonUtils::writeJsonFile(const string& path, const rapidjson::Document& doc)
{
    const char* cpath = path.c_str();
    FILE* fp;
    char buf[512];
    
    fp = fopen(cpath, "wb");
    FileWriteStream ws(fp, buf, sizeof(buf));
    Writer<FileWriteStream> writerf(ws);
    doc.Accept(writerf);
    fflush(fp);
    fclose(fp);
    
    return;
}

// 暗号化されたJSONファイルの読み込み
rapidjson::Document LastSupper::JsonUtils::readJsonCrypted(const string &path)
{
    // 暗号化の必要がない場合は通常の読み込み
    if (!IS_ENCRYPTED)
    {
        return LastSupper::JsonUtils::readJsonFile(path);
    }
    
    rapidjson::Document doc {nullptr};
    
    // ファイル読み込み
    ifstream ifs(path);
    if (ifs.fail())
    {
        AssertUtils::fatalAssert("Encrepted JSON is missing!\n" + path);
        return nullptr;
    }
    
    // 文字列を複合化
    string jsonStr;
    getline(ifs, jsonStr);
    LastSupper::StringUtils::encryptXor(jsonStr);
    doc.Parse(jsonStr.c_str());
    ifs.close();
    
    //構文エラー判定
    bool error = doc.HasParseError();
    if(error){
        AssertUtils::fatalAssert("Encrepted JSON Parse Error!\n" + path);
        return nullptr;
    }
    
    return doc;
}

// 暗号化されたJSONファイルの書き出し
void LastSupper::JsonUtils::writeJsonCrypt(const string &path, const rapidjson::Document &doc)
{
    // 平文でJSONファイル書き出し
    LastSupper::JsonUtils::writeJsonFile(path, doc);

    // 暗号化必須かチェック
    if (!IS_ENCRYPTED) return;
    
    // ファイル読み込み
    ifstream ifs(path);
    if (ifs.fail())
    {
        CCLOG("%s is missing.", path.c_str());
        return;
    }
    
    // 文字列を暗号化
    string jsonStr;
    getline(ifs, jsonStr);
    LastSupper::StringUtils::encryptXor(jsonStr);
    ifs.close();
    
    // ファイル書き出し
    ofstream ofs;
    ofs.open(path);
    ofs << jsonStr << flush;
    ofs.close();
}

// 平文ファイルを暗号化
void LastSupper::JsonUtils::enctyptJsonFile(const string& path)
{
    LastSupper::JsonUtils::writeJsonCrypt(path, LastSupper::JsonUtils::readJsonFile(path));
}
