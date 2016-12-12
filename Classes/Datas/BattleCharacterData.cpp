//
//  BattleCharacterData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/11/21.
//
//

#include "Datas/BattleCharacterData.h"

#include "Helpers/AssertHelper.h"
#include "Utils/StringUtils.h"
#include "Utils/JsonUtils.h"

const char* BattleCharacterData::HIT_POINT {"hit_point"};
const char* BattleCharacterData::SPEED_RATIO {"speed_ratio"};
const char* BattleCharacterData::ATTACKS {"attacks"};

rapidjson::Document BattleCharacterData::BATTLE_CHARACTER_DATA { rapidjson::Document() };

bool BattleCharacterData::init(const string &charaId)
{
    _assertHelper = AssertHelper::create()->pushTextLineKeyValue("charaID", charaId);
    _charaId = etos(CharacterID::UNDIFINED);
    
    // configFileがセットされていなければセット
    if (BattleCharacterData::BATTLE_CHARACTER_DATA == rapidjson::Document()) {
        string path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::BATTLE_CHARACTER);
        if (path == "") {
            _assertHelper->fatalAssert(Resource::ConfigFiles::BATTLE_CHARACTER + "is missing");
            return false;
        }
        BattleCharacterData::BATTLE_CHARACTER_DATA = LastSupper::JsonUtils::readJsonFile(path);
    }
    
    // charaIDに対応するデータを取得
    if (!BattleCharacterData::BATTLE_CHARACTER_DATA.HasMember(charaId.c_str())) return false;
    
    rapidjson::Value& battleCharacterData {BattleCharacterData::BATTLE_CHARACTER_DATA[charaId.c_str()]};
    
    // 固有データのセット
    if (!this->setHitPoint(battleCharacterData)) return false;
    if (!this->setSpeedRatio(battleCharacterData)) return false;
    if (!this->setAttacks(battleCharacterData)) return false;
    
    _charaId = charaId;
    
    return true;
}

bool BattleCharacterData::init(const int charaId)
{
    string charaIdStr {to_string(charaId)};
    
    if (!LastSupper::StringUtils::isNumericString(charaIdStr)) return false;
    
    return this->init(charaIdStr);
}

#pragma mark -
#pragma mark setter

bool BattleCharacterData::setHitPoint(const rapidjson::Value &battleCharacterData)
{
    if (!battleCharacterData.HasMember(BattleCharacterData::HIT_POINT)) {
        _assertHelper->fatalAssert("hit_point is missing.");
        return false;
    }
    
    if (!battleCharacterData[BattleCharacterData::HIT_POINT].IsInt()) {
        _assertHelper->fatalAssert("Type of hit_point should be int.");
        return false;
    }
    
    _hitPoint = battleCharacterData[BattleCharacterData::HIT_POINT].GetInt();
    return true;
}

bool BattleCharacterData::setSpeedRatio(const rapidjson::Value &battleCharacterData)
{
    if (!battleCharacterData.HasMember(BattleCharacterData::SPEED_RATIO)) {
        _assertHelper->fatalAssert("speed_ratio is missing.");
        return false;
    }
    
    if (!battleCharacterData[BattleCharacterData::SPEED_RATIO].IsDouble()) {
        _assertHelper->fatalAssert("Type of speed_ratio should be float.");
        return false;
    }
    
    _speedRatio = battleCharacterData[BattleCharacterData::SPEED_RATIO].GetDouble();
    return true;
}

bool BattleCharacterData::setAttacks(const rapidjson::Value &battleCharacterData)
{
    if (!battleCharacterData.HasMember(BattleCharacterData::ATTACKS)) {
        _assertHelper->fatalAssert("attacks is missing.");
        return false;
    }
    
    if (!battleCharacterData[BattleCharacterData::ATTACKS].IsObject()) {
        _assertHelper->fatalAssert("Type of speed_ratio should be Object.");
        return false;
    }
    
    const rapidjson::Value& attacks = battleCharacterData[BattleCharacterData::ATTACKS];
    for (rapidjson::Value::ConstMemberIterator itr = attacks.MemberBegin();itr != attacks.MemberEnd(); itr++)
    {
        string attackName = itr->name.GetString();
        if (!itr->value.IsInt()) {
            _assertHelper->pushTextLineKeyValue("attack_name", attackName)
                            ->fatalAssert("Type of atack_point should be int.");
            return false;
        }
        _attacks[attackName] = itr->value.GetInt();
    }
    return true;
}

#pragma mark -
#pragma mark getter

int BattleCharacterData::getHitPoint() const
{
    return _hitPoint;
}

float BattleCharacterData::getSpeedRatio() const
{
    return _speedRatio;
}

int BattleCharacterData::getAttackPoint(const string &attackName) const
{
    if (_attacks.count(attackName) == 0) {
        _assertHelper->pushTextLineKeyValue("attack_name", attackName)
                            ->fatalAssert("attack_point is missing.");
        return 0;
    }
    
    return _attacks.at(attackName);
}
