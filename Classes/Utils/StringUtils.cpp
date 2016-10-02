//
//  StringUtils.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/10/12.
//
//

#include "StringUtils.h"
#include "Models/StopWatch.h"

// 文字列を置換する
string LastSupper::StringUtils::strReplace(const string& pattern, const string& replacement, string target)
{
    std::string::size_type Pos(target.find(pattern));
    while( Pos != std::string::npos )
    {
        target.replace( Pos, pattern.length(), replacement);
        Pos = target.find( pattern, Pos + replacement.length() );
    }
    return target;
}

// sprinfでformatした文字列を取得
string LastSupper::StringUtils::getSprintf(const string& format, const string& str){
    char c[100];
    sprintf(c, format.c_str(), str.c_str());
    return string(c);
}

// 文字数ごとに改行で区切る (現状使えない)
string LastSupper::StringUtils::splitLineBreak(const string &target, const int line_limit)
{
    string str {""};
    int line_count = floor(target.size() / line_limit) + 1;
    int break_count = 0;
    for (int i = 0; i < line_count; i++)
    {
        cout << target[i] << " - " << break_count << endl;
        if (break_count < line_limit)
        {
            str += target[i];
            break_count++;
        }
        else
        {
            str += "\n";
            break_count = 0;
        }
    }
    return str;
}

// ランダムな文字列を取得
string LastSupper::StringUtils::getRandomString(const int length)
{
    // 現在の時間を取得
    StopWatch* watch {StopWatch::create(0)};
    int runum = floor(watch->getSecMs());
    string randomString = to_string(runum) + "/";
    
    // ランダムに文字列を追加
    static string alphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int alphLength {static_cast<int>(alphs.length())};
    for (int i = static_cast<int>(randomString.length()); i < length; i++) {
        randomString += alphs[rand() % alphLength];
    }
    return randomString;
}

// 時間(秒)を表示用にに
string LastSupper::StringUtils::getTimeForDisplay(const int sec)
{
    int min = floor(sec / 60);
    int hour = floor(min / 60);
    string display = LastSupper::StringUtils::getSprintf("%02s", to_string(hour)) + "h";
    display += LastSupper::StringUtils::getSprintf("%02s", to_string(min % 60)) + "m";
    display += LastSupper::StringUtils::getSprintf("%02s", to_string(sec % 60))+ "s";
    return display;
}

// XOR暗号・複合化
void LastSupper::StringUtils::encryptXor(string& str)
{
    for(int i = 0; i < strlen(str.c_str()); i++)
    {
        str[i] ^= C_KEY;
    }
}

// intからchar*に変換
char* LastSupper::StringUtils::setCharsFromInt(char *charas, const int num)
{
    sprintf(charas, "%d", num);
    return charas;
}

// 文字列が数字かどうか判定
bool LastSupper::StringUtils::isNumericString(const string &target)
{
    for (int i = 0; i < strlen(target.c_str()); i++) {
        if (!isdigit(target[i])) {
            return false;
        }
    }
    return true;
}

// char*が一致するかチェック
bool LastSupper::StringUtils::areMatchChars(const char *str1, const char *str2)
{
    return strcmp(str1, str2) == 0;
}
