//
//  JsonUtils.hpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#ifndef JsonUtils_h
#define JsonUtils_h

#include "define.h"
namespace LastSupper
{
    namespace JsonUtils
    {
        rapidjson::Document readJsonFile(const string& path);
        void writeJsonFile(const string& path, const rapidjson::Document& doc);
        rapidjson::Document readJsonCrypted(const string& path);
        void writeJsonCrypt(const string& path, const rapidjson::Document& doc);
        void enctyptJsonFile(const string& path);
    }
}

#endif /* JsonUtils_hpp */
