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
    string ranst = to_string(runum) + "/";
    
    // ランダムに文字列を追加
    string alphs = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int alphs_length = alphs.length();
    int ranum;
    for (int i = ranst.length(); i < length; i++)
    {
        ranum = rand() % alphs_length;
        ranst += alphs[ranum];
    }
    return ranst;
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