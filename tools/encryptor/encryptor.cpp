// default
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// rapidjson
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/writer.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/error/en.h"

#include "../../Classes/env.h"

// namespace
using namespace std;
using namespace rapidjson;

// header
void encryptXor(string& str);
void writeJsonFile(const string& path, const rapidjson::Document& doc);
void writeJsonCrypt(const string &path, const rapidjson::Document &doc);
rapidjson::Document readJsonFile(const string& path);
rapidjson::Document readJsonCrypted(const string& path);
void encryptCsvFile(const string& path);
void encryptListJson(string& listName);
void encryptListCsv(string& listName);
vector<string> readTextFile(string& listName);

// main
int main(int argc, char *argv[])
{
    string dirs[] = {"event", "config", "save", "csv"};
    string path = "tmp/root_path.txt";
    string rootPath = readTextFile(path).front();
    printf("%s\n", rootPath.c_str());

    // リストから暗号化
    for (string dir : dirs) {
        path = "tmp/" + dir + ".list";
        vector<string> fileNames = readTextFile(path);
        printf("%s\n", dir.c_str());
        for (string file : fileNames) {
            file = rootPath + dir + "/" + file;
            printf("%s\n", file.c_str());
            if (dir == "csv") {
                encryptCsvFile(file);
            } else {
                writeJsonCrypt(file);
            }
        }
    }

    return 0;
}

void encryptListJson(string& listName)
{

}

void encryptListCsv(string& listName)
{

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

// CSVファイルをXOR暗号化して書き出し
void encryptCsvFile(const string& path)
{
    // ファイル読み込み
    ifstream ifs(path);

    // 文字列を暗号化
    string str;
    getline(ifs, str);
    ofstream ofs;
    ofs.open(path);
    while(getline(ifs, str))
    {
        encryptXor(str);
        ofs << str << endl;
    }
    ifs.close();
    ofs.close();
}

// 暗号化
void encryptXor(string& str)
{
    for (int i = 0; i < strlen(str.c_str()); i++) {
        str[i] ^= C_KEY;
    }
}
