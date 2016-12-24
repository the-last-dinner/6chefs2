// default
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>

// rapidjson
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/error/en.h"
#include "rapidjson/rapidjson.h"

#include "../../Classes/env.h"

// namespace
using namespace std;
using namespace rapidjson;
typedef GenericDocument< UTF8<> > Document;
typedef GenericValue< UTF8<> > Value;
typedef GenericStringStream< UTF8<> > StringStream;
typedef GenericStringBuffer< UTF8<> > StringBuffer;
using CsvMap = map<int, vector<string> >;

// header
void encryptXor(string& str);
void writeJsonFile(const string& path, const rapidjson::Document& doc);
void writeJsonCrypt(const string &path, const rapidjson::Document &doc);
rapidjson::Document readJsonFile(const string& path);
rapidjson::Document readJsonCrypted(const string& path);
rapidjson::Document csvMapToJson(CsvMap& csvMap);
CsvMap readCsvFile(const string& path);
vector<string> readTextFile(string& listName);
string strReplace(const string& pattern, const string& replacement, string target);

// main
int main(int argc, char *argv[])
{
    // シェルで必要ファイル生成
    system("sh prepare.sh");

    // 変数準備
    string dirs[] = {
        "event",
        "config",
        "save",
        "csv",
    };
    string path = "tmp/root_path.txt";
    string rootPath = readTextFile(path).front();
    printf("%s\n", rootPath.c_str());

    // リストから暗号化
    for (string dir:dirs) {
        path = "tmp/" + dir + ".list";
        vector<string> fileNames = readTextFile(path);
        printf("%s\n", dir.c_str());
        for (string file:fileNames) {
            file = rootPath + dir + "/" + file;
            printf("%s\n", file.c_str());
            if (dir == "csv") {
                CsvMap csvMap = readCsvFile(file);
                rapidjson::Document doc = csvMapToJson(csvMap);
                writeJsonCrypt(file, doc);
            } else {
                rapidjson::Document doc = readJsonFile(file);
                writeJsonCrypt(file, doc);
            }
        }
    }

    // 暗号化が終わった証拠を作成
    string makeFileCommand = "touch " + rootPath + "config/IsEncrypted.json";
    system(makeFileCommand.c_str());

    return 0;
}

// 暗号化するファイルのリストを読み込む
vector<string> readTextFile(string& path)
{
    vector<string> files;
    ifstream ifs(path);
    string str;
    while(getline(ifs, str)) {
        files.push_back(str);
    }
    ifs.close();
    return files;
}

// JSONファイルの読み込み
rapidjson::Document readJsonFile(const string& path)
{
    const char* cpath = path.c_str();
    rapidjson::Document doc;
    FILE* fp;
    char buf[512];

    // ファイル操作
    fp = fopen(cpath, "rb");
    FileReadStream rs(fp, buf, sizeof(buf));
    doc.ParseStream(rs);
    fclose(fp);

    return doc;
}

// 暗号化されたJSONファイルの読み込み
rapidjson::Document readJsonCrypted(const string &path)
{
    rapidjson::Document doc;

    // ファイル読み込み
    ifstream ifs(path);

    // 文字列を複合化
    string jsonStr;
    getline(ifs, jsonStr);
    encryptXor(jsonStr);
    doc.Parse(jsonStr.c_str());
    ifs.close();

    return doc;
}

// JSONファイルの書き出し
void writeJsonFile(const string& path, const rapidjson::Document& doc)
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

// 暗号化されたJSONファイルの書き出し
void writeJsonCrypt(const string &path, const rapidjson::Document &doc)
{
    // 平文書き込み
    writeJsonFile(path, doc);

    // ファイル読み込み
    ifstream ifs(path);
    if (ifs.fail()) {
        printf("%s is missing.", path.c_str());
        return;
    }

    // 文字列を暗号化
    string jsonStr;
    getline(ifs, jsonStr);
    encryptXor(jsonStr);
    ifs.close();

    // ファイル書き出し
    ofstream ofs;
    ofs.open(path);
    ofs << jsonStr << flush;
    ofs.close();
}

// csvファイルの読み込み
CsvMap readCsvFile(const string& path)
{
    // ファイル読み込み
    ifstream file(path);
    CsvMap values;
    string str;
    int p, i, dataId;
    int headerCount = 1;

    // 1行目のheaderの要素数をカウント
    string header;
    getline(file, header);
    while( (p = header.find(",")) != header.npos ) {
        header = header.substr(p+1);
        ++headerCount;
    }

    // csvデータ格納
    while(getline(file, str)) {
        vector<string> inner;
        i = 1;
        // キャリッジリターンを削除
        str = strReplace("\r", "", str);
        // カンマがあるかを探し、そこまでをvaluesに格納
        while( (p = str.find(",")) != str.npos ) {
            if (i == 1) {
                dataId = stoi(str.substr(0, p));
            }
            inner.push_back(str.substr(0, p));
            // strの中身は","の1文字を飛ばす
            str = str.substr(p+1);
            i++;
        }
        inner.push_back(str);

        // 要素数チェック
        if (i != headerCount) {
            string fileName = path;
            while ((p = fileName.find("/")) != fileName.npos) {
                fileName = fileName.substr(p+1);
            }
            fileName = fileName.substr(p+1);
            return values;
        }
        values[dataId] = inner;
    }
    return values;
}

// csvMapをjsonオブジェクトに変換
rapidjson::Document csvMapToJson(CsvMap& csvMap)
{
    rapidjson::Document json;
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();

    for (auto itr:csvMap) {
        rapidjson::Value idStr (kStringType);
        const char* idChar = to_string(itr.first).c_str();
        writer.Key(idChar);
        writer.StartArray();
        for (string val:itr.second) {
            writer.String(val.c_str());
        }
        writer.EndArray();
    }
    writer.EndObject();

    json.Parse(s.GetString());
    return json;
}

// 暗号化
void encryptXor(string& str)
{
    for (int i = 0; i < strlen(str.c_str()); i++) {
        str[i] ^= C_KEY;
    }
}

// 文字列の置換
string strReplace(const string& pattern, const string& replacement, string target)
{
    std::string::size_type Pos(target.find(pattern));
    while( Pos != std::string::npos )
    {
        target.replace( Pos, pattern.length(), replacement);
        Pos = target.find( pattern, Pos + replacement.length() );
    }
    return target;
}
