//
//  BattleCharacterData.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/11/21.
//
//

#include "Datas/BattleCharacterData.h"

#include "Datas/AttackData.h"
#include "Helpers/AssertHelper.h"
#include "Utils/StringUtils.h"
#include "Utils/JsonUtils.h"

const char* BattleCharacterData::HIT_POINT {"hit_point"};
const char* BattleCharacterData::SPEED_RATIO {"speed_ratio"};
const char* BattleCharacterData::ATTACKS {"attacks"};
const char* BattleCharacterData::STEP_INTERVAL_TIME {"step_interval_time"};
const char* BattleCharacterData::STEP_STAMINA {"step_stamina"};
const char* BattleCharacterData::POWER {"power"};
const char* BattleCharacterData::STAMINA {"stamina"};
const char* BattleCharacterData::INTERVAL_TIME {"interval_time"};
const char* BattleCharacterData::MOTION_SOUND {"motion_sound"};
const char* BattleCharacterData::MOTION_SOUND_VOLUME {"motion_sound_volume"};
const char* BattleCharacterData::HIT_SOUND {"hit_sound"};
const char* BattleCharacterData::HIT_SOUND_VOLUME {"hit_sound_volume"};


rapidjson::Document BattleCharacterData::BATTLE_CHARACTER_DATA { rapidjson::Document() };

BattleCharacterData::~BattleCharacterData()
{
    for (auto attack : _attacks) {
        CC_SAFE_RELEASE_NULL(attack.second);
    }
}

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
        BattleCharacterData::BATTLE_CHARACTER_DATA = LastSupper::JsonUtils::readJsonCrypted(path);
    }
    
    // charaIDに対応するデータを取得
    if (!BattleCharacterData::BATTLE_CHARACTER_DATA.HasMember(charaId.c_str())) return false;
    
    rapidjson::Value& battleCharacterData {BattleCharacterData::BATTLE_CHARACTER_DATA[charaId.c_str()]};
    
    // 固有データのセット
    if (!this->setHitPoint(battleCharacterData)) return false;
    if (!this->setSpeedRatio(battleCharacterData)) return false;
    if (!this->setAttacks(battleCharacterData)) return false;
    if (!this->setStepStamina(battleCharacterData)) return false;
    if (!this->setStepIntervalTime(battleCharacterData)) return false;
    
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
        AttackData*
        attackData { AttackData::create(attackName) };
        _assertHelper->pushTextLineKeyValue("attack_name", attackName);
        
        // power
        if (!itr->value.HasMember(BattleCharacterData::POWER)) {
            _assertHelper->fatalAssert("power is missing.");
            return false;
        }
        if (!itr->value[BattleCharacterData::POWER].IsInt()) {
            _assertHelper->fatalAssert("Type of power should be int");
            return false;
        }
        attackData->power = itr->value[BattleCharacterData::POWER].GetInt();
        
        // stamina
        if (!itr->value.HasMember(BattleCharacterData::STAMINA)) {
            _assertHelper->fatalAssert("stamina is missing.");
            return false;
        }
        if (!itr->value[BattleCharacterData::STAMINA].IsDouble()) {
            _assertHelper->fatalAssert("Type of stamina should be double");
            return false;
        }
        attackData->stamina = itr->value[BattleCharacterData::STAMINA].GetDouble();
        
        // interval time
        if (!itr->value.HasMember(BattleCharacterData::INTERVAL_TIME)) {
            _assertHelper->fatalAssert("interval_time is missing.");
            return false;
        }
        if (!itr->value[BattleCharacterData::INTERVAL_TIME].IsDouble()) {
            _assertHelper->fatalAssert("Type of interval_time should be double");
            return false;
        }
        attackData->intervalTime = itr->value[BattleCharacterData::INTERVAL_TIME].GetDouble();
        
        // motion sound
        if (itr->value.HasMember(BattleCharacterData::MOTION_SOUND)) {
            if (!itr->value[BattleCharacterData::MOTION_SOUND].IsString()) {
                _assertHelper->fatalAssert("Type of motion sound should be string.");
                return false;
            }
            attackData->motionSound = itr->value[BattleCharacterData::MOTION_SOUND].GetString();
        } else {
            attackData->motionSound = "";
        }
        
        if (itr->value.HasMember(BattleCharacterData::MOTION_SOUND_VOLUME)) {
            if (!itr->value[BattleCharacterData::MOTION_SOUND_VOLUME].IsDouble()) {
                _assertHelper->fatalAssert("Type of motion sound volume should be string.");
                return false;
            }
            attackData->motionSoundVolume = itr->value[BattleCharacterData::MOTION_SOUND_VOLUME].GetDouble();
        }
        
        // hit sound
        if (itr->value.HasMember(BattleCharacterData::HIT_SOUND)) {
            if (!itr->value[BattleCharacterData::HIT_SOUND].IsString()) {
                _assertHelper->fatalAssert("Type of motion sound should be string.");
                return false;
            }
            attackData->hitSound = itr->value[BattleCharacterData::HIT_SOUND].GetString();
        } else {
            attackData->hitSound = "";
        }
        
        if (itr->value.HasMember(BattleCharacterData::HIT_SOUND_VOLUME)) {
            if (!itr->value[BattleCharacterData::HIT_SOUND_VOLUME].IsDouble()) {
                _assertHelper->fatalAssert("Type of motion sound volume should be string.");
                return false;
            }
            attackData->hitSoundVolume = itr->value[BattleCharacterData::HIT_SOUND_VOLUME].GetDouble();
        }
        
        CC_SAFE_RETAIN(attackData);
        _attacks[attackName] = attackData;
        _assertHelper->popTextLine();
    }
    return true;
}

bool BattleCharacterData::setStepStamina(const rapidjson::Value &battleCharacterData)
{
    if (!battleCharacterData.HasMember(BattleCharacterData::STEP_STAMINA)) {
        _assertHelper->fatalAssert("step_stamina is missing.");
        return false;
    }
    
    if (!battleCharacterData[STEP_STAMINA].IsDouble()) {
        _assertHelper->fatalAssert("Type of step_stamina should be double.");
        return false;
    }
    
    _stepStamina = battleCharacterData[STEP_STAMINA].GetDouble();
    return true;
}

bool BattleCharacterData::setStepIntervalTime(const rapidjson::Value &battleCharacterData)
{
    if (!battleCharacterData.HasMember(BattleCharacterData::STEP_INTERVAL_TIME)) {
        _assertHelper->fatalAssert("step_interval_time is missing.");
        return false;
    }
    
    if (!battleCharacterData[STEP_INTERVAL_TIME].IsDouble()) {
        _assertHelper->fatalAssert("Type of step_interval_time should be double.");
        return false;
    }
    
    _stepIntervalTime = battleCharacterData[STEP_INTERVAL_TIME].GetDouble();
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

float BattleCharacterData::getStepStamina() const
{
    return _stepStamina;
}

float BattleCharacterData::getStepIntervalTime() const
{
    return _stepIntervalTime;
}

AttackData* BattleCharacterData::getAttackData(const string &attackName) const
{
    if (_attacks.count(attackName) == 0) {
        // ここでエラー出しちゃうと都合が悪いので取得する側で判断する
        //_assertHelper->pushTextLineKeyValue("attack_name", attackName)
        //->fatalAssert("attack data is missing.");
        return nullptr;
    }
    return _attacks.at(attackName);
}
