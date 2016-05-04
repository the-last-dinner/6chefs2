//
//  CsvUtils.h
//  6chefs2
//
//  Created by Ryoya Ino on 2016/03/15.
//
//

#ifndef CsvUtils_h
#define CsvUtils_h

#include "define.h"

namespace CsvUtils
{
    using CsvMap = map<int, vector<string>>;
    CsvMap readCsvFile(const string& path);
    CsvMap readJsonForCsv(const string& path);
    bool encryptCsvFile(const string& path);
    void encryptCsvToJson(const string& path);
    rapidjson::Document csvMapToJson(CsvMap& csvMap);
}

#endif /* CsvUtils_hpp */
