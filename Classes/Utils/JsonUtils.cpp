//
//  JsonUtils.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"

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
        size_t offset = doc.GetErrorOffset();
        ParseErrorCode code = doc.GetParseError();
        const char* msg = GetParseError_En(code);
        CCLOG("GlobalTemplate JSON Parse Error : %d:%d(%s)\n", static_cast<int>(offset), code, msg);
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
    fclose(fp);
    
    return;
}

// JSONファイルの読み込み
rapidjson::Document LastSupper::JsonUtils::readJsonCrypted(const string &path)
{
    rapidjson::Document doc {nullptr};
    
    // ファイル読み込み
    ifstream ifs(path);
    if (ifs.fail())
    {
        CCLOG("%s is missing.", path.c_str());
        return nullptr;
    }
    
    // 文字列を複合化
    string jsonStr;
    getline(ifs, jsonStr);
    LastSupper::StringUtils::encryptXor(jsonStr);
//    for(int i = 0; i < strlen(jsonStr.c_str()); i++)
//    {
//        jsonStr[i] ^= C_KEY;
//    }
    doc.Parse(jsonStr.c_str());
    ifs.close();
    
    //構文エラー判定
    bool error = doc.HasParseError();
    if(error){
        size_t offset = doc.GetErrorOffset();
        ParseErrorCode code = doc.GetParseError();
        const char* msg = GetParseError_En(code);
        CCLOG("GlobalTemplate JSON Parse Error : %d:%d(%s)\n", static_cast<int>(offset), code, msg);
        return nullptr;
    }
    
    return doc;
}

void LastSupper::JsonUtils::writeJsonCrypt(const string &path, const rapidjson::Document &doc)
{
    LastSupper::JsonUtils::writeJsonFile(path, doc);
    // ファイル読み込み
    ifstream ifs(path);
    if (ifs.fail())
    {
        CCLOG("%s is missing.", path.c_str());
        return;
    }
    
    // 文字列を複合化
    string jsonStr;
    getline(ifs, jsonStr);
    LastSupper::StringUtils::encryptXor(jsonStr);
//    for(int i = 0; i < strlen(jsonStr.c_str()); i++)
//    {
//        jsonStr[i] ^= C_KEY;
//    }
    ifs.close();
    
    // ファイル書き出し
    ofstream ofs;
    ofs.open(path);
    ofs << jsonStr << endl;
    ofs.close();
}

// 平文ファイルを暗号化
void LastSupper::JsonUtils::enctyptJsonFile(const string& path)
{
    LastSupper::JsonUtils::writeJsonCrypt(path, LastSupper::JsonUtils::readJsonFile(path));
}