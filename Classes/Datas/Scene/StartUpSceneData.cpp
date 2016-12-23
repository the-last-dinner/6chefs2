//
//  StartUpSceneData.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/13.
//
//

#include "Datas/Scene/StartUpSceneData.h"
#include "Utils/JsonUtils.h"

bool StartUpSceneData::init()
{
    string basePath;
    int length;
    const rapidjson::Document json = LastSupper::JsonUtils::readJsonCrypted(Resource::ConfigFiles::PRELOAD_LIST);
    for (rapidjson::Value::ConstMemberIterator itr = json.MemberBegin(); itr != json.MemberEnd(); itr++)
    {
        basePath = itr->name.GetString();
        basePath += "/";
        length = itr->value.Size();
        for (int i = 0; i < length; i++)
        {
            this->textureFilePaths.push_back(basePath + itr->value[i].GetString());
        }
    }
    
    this->soundFilePaths = {};
    
    return true;
}
