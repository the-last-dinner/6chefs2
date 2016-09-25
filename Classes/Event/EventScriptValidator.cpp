//
//  EventScriptValidator.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/29.
//
//

#include "Event/EventScriptValidator.h"

#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"
#include "Utils/StringUtils.h"

const string EventScriptValidator::REQUIRE {"require"};
const string EventScriptValidator::MEMBER {"member"};
const string EventScriptValidator::STRING {"string"};
const string EventScriptValidator::RECURSIVE_OBJECT {"recursive_object"};
const string EventScriptValidator::OBJECT {"object"};
const string EventScriptValidator::RECURSIVE_ARRAY {"recursive_array"};
const string EventScriptValidator::ARRAY {"array"};
const string EventScriptValidator::NORMAL {"normal"};
const string EventScriptValidator::INT {"int"};
const string EventScriptValidator::DOUBLE {"double"};
const string EventScriptValidator::STOI {"stoi"};

const map<rapidjson::Type, string> EventScriptValidator::typeToValidateString = {
    {rapidjson::kObjectType, EventScriptValidator::RECURSIVE_OBJECT},
    {rapidjson::kArrayType, EventScriptValidator::RECURSIVE_ARRAY},
    {rapidjson::kStringType, EventScriptValidator::NORMAL},
};

rapidjson::Document EventScriptValidator::validateConfig { rapidjson::Document() };

#pragma mark -
#pragma mark PublicMethods

// 初期化
bool EventScriptValidator::init()
{
    if (EventScriptValidator::validateConfig == rapidjson::Document()) {
        string path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::EVENT_SCRIPT_VALIDATOR);
        if (path == "") return false;
        EventScriptValidator::validateConfig = LastSupper::JsonUtils::readJsonFile(path);
    }
    
    this->typeToValidateFunc = {
        {EventScriptValidator::RECURSIVE_OBJECT, CC_CALLBACK_2(EventScriptValidator::checkObject, this)},
        {EventScriptValidator::RECURSIVE_ARRAY, CC_CALLBACK_2(EventScriptValidator::checkArray, this)},
        {EventScriptValidator::OBJECT, CC_CALLBACK_2(EventScriptValidator::isObject, this)},
        {EventScriptValidator::ARRAY, CC_CALLBACK_2(EventScriptValidator::isArray, this)},
        {EventScriptValidator::INT, CC_CALLBACK_2(EventScriptValidator::isInt, this)},
        {EventScriptValidator::DOUBLE, CC_CALLBACK_2(EventScriptValidator::isDouble, this)},
        {EventScriptValidator::STRING, CC_CALLBACK_2(EventScriptValidator::isString, this)},
        {EventScriptValidator::STOI, CC_CALLBACK_2(EventScriptValidator::isStoi, this)},
    };
    
    return true;
}

// メインのバリデートメソッド
bool EventScriptValidator::validate(const rapidjson::Value& targetEvent)
{
    // タイプ存在チェック
    if (!targetEvent.HasMember("type")) {
        // Assert
        return false;
    }
    
    // タイプがstringかチェック
    if (!targetEvent["type"].IsString()) {
        // Assert
        return false;
    }
    
    // タイプ名取得
    this->typeName = targetEvent["type"].GetString();
    const char* typeNameChar = this->typeName.c_str();
    cout << "ValidateEventName: " << this->typeName << endl;
    
    // イベントの存在チェック
    if (!this->validateConfig.HasMember(typeNameChar)) {
        LastSupper::AssertUtils::warningAssert("EventScriptError\n" + this->typeName + "なんてイベントのバリデーションはないずら〜");
        return false;
    }

    // チェック
    const rapidjson::Value& rule = this->validateConfig[typeNameChar];
    return this->checkObject(targetEvent, rule);
}

#pragma mark -
#pragma mark CheckObject

bool EventScriptValidator::checkObject(const rapidjson::Value& targetEvent, const rapidjson::Value& rule)
{
    bool hasError = false;
    if (rule.HasMember(EventScriptValidator::REQUIRE.c_str())) {
        const rapidjson::Value& requires = rule[EventScriptValidator::REQUIRE.c_str()];
        hasError = this->checkRequire(targetEvent, requires);
    }
    
    if (rule.HasMember(EventScriptValidator::MEMBER.c_str())) {
        const rapidjson::Value& members = rule[EventScriptValidator::MEMBER.c_str()];
        hasError = this->checkMember(targetEvent, members);
    }
    
    return true;
}

#pragma mark -
#pragma mark CheckRequire

bool EventScriptValidator::checkRequire(const rapidjson::Value& targetEvent, const rapidjson::Value& requires)
{
    bool isOr = requires[0].IsArray();
    bool isOk = false;
    if (isOr) {
        isOk = this->checkRequireOr(targetEvent, requires);
    } else {
        isOk = this->checkRequireAnd(targetEvent, requires);
    }
    if (!isOk) {
        // Assert
        LastSupper::AssertUtils::fatalAssert("Event::" + this->typeName);
        return false;
    }
    return true;
}

bool EventScriptValidator::checkRequireOr(const rapidjson::Value& targetEvent, const rapidjson::Value& requireDoubleArray)
{
    int doubleArraySize = requireDoubleArray.Size();
    for (int i = 0; i < doubleArraySize; i++) {
        if (this->checkRequireAnd(targetEvent, requireDoubleArray[i])) return true;
    }
    return false;
}

bool EventScriptValidator::checkRequireAnd(const rapidjson::Value& targetEvent, const rapidjson::Value& requireArray)
{
    int requireSize = requireArray.Size();
    for (int i = 0; i < requireSize; i++) {
        if (!this->checkRequireChild(targetEvent, requireArray[i])) {
            return false;
        }
    }
    return true;
}

bool EventScriptValidator::checkRequireChild(const rapidjson::Value& targetEvent, const rapidjson::Value& targetMemberName)
{
    cout << "require: " << targetMemberName.GetString() << endl;
    if (!targetEvent.HasMember(targetMemberName.GetString())) return false;
    return true;
}

#pragma mark -
#pragma mark CheckMember

bool EventScriptValidator::checkMember(const rapidjson::Value& targetEvent, const rapidjson::Value& members)
{
    for (rapidjson::Value::ConstMemberIterator itr = members.MemberBegin(); itr != members.MemberEnd(); itr++) {
        // メンバがない場合はチェックOK
        string targetMemberName = itr->name.GetString();
        
        if (!targetEvent.HasMember(targetMemberName.c_str())) continue;
        cout << "checkType: " << targetMemberName;
        
        // Validator文法ミス
        if (!itr->value.IsArray()) {
            // Assert
            return false;
        }
        
        // メンバーの型が配列のどれかに当てはまるようにチェック
        bool isOk = false;
        int targetSize = itr->value.Size();
        for (int i = 0; i < targetSize; i++) {
            isOk = this->checkMemberType(targetEvent[targetMemberName.c_str()], itr->value[i]);
            cout << isOk << endl;
            if (isOk) break;
        }
        
        // 当てはまらなかった場合
        if (!isOk) {
            LastSupper::AssertUtils::warningAssert("タイプエラー");
            // Assert
            return false;
        }
    }
    return true;
}

bool EventScriptValidator::checkMemberType(const rapidjson::Value& targetMember, const rapidjson::Value& checkType)
{
    string funcKey = this->getValidateFuncKey(checkType);
    
    cout << " is " << funcKey << endl;
    // バリデート関数が存在するかチェック
    if (this->typeToValidateFunc.count(funcKey) == 0) {
        // Assert
        cout << "error" << endl;
        return false;
    }
    return this->typeToValidateFunc.at(funcKey)(targetMember, checkType);
}

#pragma mark -
#pragma mark CheckArray

bool EventScriptValidator::checkArray(const rapidjson::Value& targetMember, const rapidjson::Value& checkTypes)
{
    if (!this->isArray(checkTypes)) return false;
    if (!this->isArray(targetMember)) {
        // Assert
        return false;
    }
    
    int arraySize = checkTypes.Size();
    
    if (arraySize == 1) {
        return this->checkArraySimpleContents(targetMember, checkTypes[0]);
    } else {
        return this->checkArrayMultipleContents(targetMember, checkTypes);
    }
}

bool EventScriptValidator::checkArraySimpleContents(const rapidjson::Value &targetMember, const rapidjson::Value &checkType)
{
    bool isOk = false;
    int targetSize = targetMember.Size();
    for (int i = 0; i < targetSize; i++) {
        isOk = this->checkMemberType(targetMember[i], checkType);
        if (!isOk) {
            return false;
        }
    }
    return true;
}

bool EventScriptValidator::checkArrayMultipleContents(const rapidjson::Value &targetMember, const rapidjson::Value &checkTypes)
{
    bool isOk = false;
    int targetSize = targetMember.Size();
    int typeSize = checkTypes.Size();
    
    if (targetSize != typeSize) {
        // Assert
        return false;
    }
    
    for (int i = 0; i < targetSize; i++) {
        isOk = this->checkMemberType(targetMember[i], checkTypes[i]);
        if (!isOk) {
            return false;
        }
    }
    
    return true;
}

#pragma mark -
#pragma mark CheckType

bool EventScriptValidator::isObject(const rapidjson::Value &target, const rapidjson::Value& checkType)
{
    return target.IsObject();
}

bool EventScriptValidator::isArray(const rapidjson::Value &target, const rapidjson::Value& checkType)
{
    return target.IsArray();
}

bool EventScriptValidator::isInt(const rapidjson::Value &target, const rapidjson::Value& checkType)
{
    return target.IsInt();
}

bool EventScriptValidator::isDouble(const rapidjson::Value& target, const rapidjson::Value& checkType)
{
    return target.IsDouble();
}

bool EventScriptValidator::isString(const rapidjson::Value &target, const rapidjson::Value& checkType)
{
    return target.IsString();
}

bool EventScriptValidator::isStoi(const rapidjson::Value &target, const rapidjson::Value& checkType)
{
    if (!target.IsString()) return false;
    
    if (!LastSupper::StringUtils::isNumericString(target.GetString())) return false;
    
    return true;
}

#pragma mark -
#pragma mark Utils

// バリデート関数ポインタへのキーを取得
string EventScriptValidator::getValidateFuncKey(const rapidjson::Value &checkType)
{
    rapidjson::Type validateType = checkType.GetType();
    if (EventScriptValidator::typeToValidateString.count(validateType) == 0) {
        // Assert
        return "ERROR";
    }
    
    string funcKey = EventScriptValidator::typeToValidateString.at(validateType);
    if (funcKey == EventScriptValidator::NORMAL) {
        funcKey = checkType.GetString();
    }
    
    return funcKey;
}
