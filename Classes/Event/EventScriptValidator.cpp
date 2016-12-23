//
//  EventScriptValidator.cpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/08/29.
//
//

#include "Event/EventScriptValidator.h"

#include "Helpers/AssertHelper.h"
#include "Utils/JsonUtils.h"
#include "Utils/AssertUtils.h"
#include "Utils/StringUtils.h"

const string EventScriptValidator::TYPE {"type"};
const string EventScriptValidator::REQUIRE {"require"};
const string EventScriptValidator::MEMBER {"member"};
const string EventScriptValidator::MEMBERS {"members"};
const string EventScriptValidator::ENUMS {"enums"};
const string EventScriptValidator::STRING {"string"};
const string EventScriptValidator::RECURSIVE_OBJECT {"recursive_object"};
const string EventScriptValidator::OBJECT {"object"};
const string EventScriptValidator::RECURSIVE_ARRAY {"recursive_array"};
const string EventScriptValidator::ARRAY {"array"};
const string EventScriptValidator::NORMAL {"normal"};
const string EventScriptValidator::INT {"int"};
const string EventScriptValidator::DOUBLE {"double"};
const string EventScriptValidator::STOI {"stoi"};
const string EventScriptValidator::ENUM {"enum"};
const string EventScriptValidator::BOOL {"bool"};

const map<rapidjson::Type, string> EventScriptValidator::TYPE_TO_VALIDATE_STRING = {
    {rapidjson::kObjectType, EventScriptValidator::RECURSIVE_OBJECT},
    {rapidjson::kArrayType, EventScriptValidator::RECURSIVE_ARRAY},
    {rapidjson::kStringType, EventScriptValidator::NORMAL},
};

rapidjson::Document EventScriptValidator::VALIDATE_CONFIG { rapidjson::Document() };

#pragma mark -
#pragma mark PublicMethods

// 初期化
bool EventScriptValidator::init(const string& mapName, const string& eventName)
{
    if (EventScriptValidator::VALIDATE_CONFIG == rapidjson::Document()) {
        string path = FileUtils::getInstance()->fullPathForFilename(Resource::ConfigFiles::EVENT_SCRIPT_VALIDATOR);
        if (path == "") return false;
        EventScriptValidator::VALIDATE_CONFIG = LastSupper::JsonUtils::readJsonCrypted(path);
    }
    
    _assertHelper = AssertHelper::create();
    if (!_assertHelper) return false;
    CC_SAFE_RETAIN(_assertHelper);
    
    _assertHelper->pushTextLineKeyValue("MapName", mapName)->pushTextLineKeyValue("EventID", eventName);
    
    map<string, function<bool(const rapidjson::Value&, const rapidjson::Value&)>> typeToValidateFunc {
        {EventScriptValidator::RECURSIVE_OBJECT, CC_CALLBACK_2(EventScriptValidator::checkObject, this)},
        {EventScriptValidator::RECURSIVE_ARRAY, CC_CALLBACK_2(EventScriptValidator::checkArray, this)},
        {EventScriptValidator::OBJECT, CC_CALLBACK_2(EventScriptValidator::isObject, this)},
        {EventScriptValidator::ARRAY, CC_CALLBACK_2(EventScriptValidator::isArray, this)},
        {EventScriptValidator::INT, CC_CALLBACK_2(EventScriptValidator::isInt, this)},
        {EventScriptValidator::DOUBLE, CC_CALLBACK_2(EventScriptValidator::isDouble, this)},
        {EventScriptValidator::STRING, CC_CALLBACK_2(EventScriptValidator::isString, this)},
        {EventScriptValidator::STOI, CC_CALLBACK_2(EventScriptValidator::isStoi, this)},
        {EventScriptValidator::ENUM, CC_CALLBACK_2(EventScriptValidator::isEnum, this)},
        {EventScriptValidator::BOOL, CC_CALLBACK_2(EventScriptValidator::isBool, this)},
    };
    
    _typeToValidateFunc = typeToValidateFunc;
    
    return true;
}

EventScriptValidator::~EventScriptValidator()
{
    FUNCLOG
    CC_SAFE_RELEASE_NULL(_assertHelper);
}

// メインのバリデートメソッド
bool EventScriptValidator::validate(const rapidjson::Value& targetEvent)
{
    // タイプ存在チェック
    const char* type = EventScriptValidator::TYPE.c_str();
    if (!targetEvent.HasMember(type)) {
        _assertHelper->pushTextLine("\"" + EventScriptValidator::TYPE + "\" is missing.")
                     ->fatalAssert("EventScriptSyntaxError");
        return false;
    }
    
    // タイプ名取得
    string typeName = targetEvent[type].GetString();
    const char* typeNameChar = typeName.c_str();
    // cout << "ValidateEventName: " << this->typeName << endl;
    
    // タイプがstringかチェック
    if (!targetEvent[type].IsString()) {
        _assertHelper->pushTextLine("Type must be string!")
                     ->fatalAssert("EventScriptSyntaxError");
        return false;
    }
    
    _assertHelper->pushTextLineKeyValue("TypeName", typeName);
    
    // イベントの存在チェック
    if (!VALIDATE_CONFIG["events"].HasMember(typeNameChar)) {
        _assertHelper->pushTextLine("Validation config of this type is missing.")
                          ->warningAssert("EventValidationConfigError");
        return false;
    }

    // チェック
    const rapidjson::Value& rule = VALIDATE_CONFIG["events"][typeNameChar];
    return this->checkObject(targetEvent, rule);
}

#pragma mark -
#pragma mark CheckObject

bool EventScriptValidator::checkObject(const rapidjson::Value& targetEvent, const rapidjson::Value& rule)
{
    // Requireチェック
    bool isOk = true;
    if (rule.HasMember(EventScriptValidator::REQUIRE.c_str())) {
        const rapidjson::Value& requires = rule[EventScriptValidator::REQUIRE.c_str()];
        isOk = this->checkRequire(targetEvent, requires);
    }
    if (!isOk) {
        _assertHelper->warningAssert("EventRequireError");
        return false;
    }
    
    // Memberチェック
    isOk = true;
    if (rule.HasMember(EventScriptValidator::MEMBER.c_str())) {
        const rapidjson::Value& members = rule[EventScriptValidator::MEMBER.c_str()];
        isOk = this->checkMember(targetEvent, members);
    }
    if (!isOk) {
        _assertHelper->warningAssert("EventMemberTypeError");
        return false;
    }
    
    return true;
}

#pragma mark -
#pragma mark CheckRequire

bool EventScriptValidator::checkRequire(const rapidjson::Value& targetEvent, const rapidjson::Value& requires)
{
    bool isOr = requires[0].IsArray();
    if (isOr) {
        return this->checkRequireOr(targetEvent, requires);
    } else {
        return this->checkRequireAnd(targetEvent, requires);
    }
}

bool EventScriptValidator::checkRequireOr(const rapidjson::Value& targetEvent, const rapidjson::Value& requireDoubleArray)
{
    int doubleArraySize = requireDoubleArray.Size();
    for (int i = 0; i < doubleArraySize; i++) {
        if (this->checkRequireAnd(targetEvent, requireDoubleArray[i])) {
            _assertHelper->popTextLines(i);
            return true;
        }
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
    // cout << "require: " << targetMemberName.GetString() << endl;
    if (!targetEvent.HasMember(targetMemberName.GetString())) {
        string memberNameString = targetMemberName.GetString();
        _assertHelper->pushTextLine(memberNameString + " is require!");
        return false;
    }
    return true;
}

#pragma mark -
#pragma mark CheckMember

bool EventScriptValidator::checkMember(const rapidjson::Value& targetEvent, const rapidjson::Value& members)
{
    for (rapidjson::Value::ConstMemberIterator itr = targetEvent.MemberBegin(); itr != targetEvent.MemberEnd(); itr++) {
        
        string targetMemberName = itr->name.GetString();
        
        // typeの場合はスルー
        if (targetMemberName == EventScriptValidator::TYPE) continue;
        // cout << "checkMember: " << targetMemberName;
        
        // メンバー存在チェック
        if (!members.HasMember(targetMemberName.c_str())) {
            _assertHelper->pushTextLine("\"" + targetMemberName + "\" is not exists!");
            return false;
        }
        
        const rapidjson::Value& targetRule = members[targetMemberName.c_str()];
        
        // Validator文法ミス
        if (!targetRule.IsArray()) {
            _assertHelper->pushTextLine("\"members\" must be array")
                              ->fatalAssert("EventValidationConfigError");
            return true; // バリデーションエラーは回避
        }
        
        // メンバー名をAssertに追加
        _assertHelper->pushTextLineKeyValue("MemberName", targetMemberName);
        
        // メンバーの型が配列のどれかに当てはまるようにチェック
        bool isOk = false;
        int ruleSize = targetRule.Size();
        for (int i = 0; i < ruleSize; i++) {
            isOk = this->checkMemberType(itr->value, targetRule[i]);
            if (isOk) {
                _assertHelper->popTextLines(i);
                break;
            }
        }
        
        // 当てはまらなかった場合
        if (!isOk) return false;
        
        // メンバ名をAssertから削除
        _assertHelper->popTextLine();
    }
    return true;
}

bool EventScriptValidator::checkMemberType(const rapidjson::Value& targetMember, const rapidjson::Value& checkType)
{
    string funcKey = this->getValidateFuncKey(checkType);
    
    // cout << " is " << funcKey << endl;
    
    // バリデート関数が存在するかチェック
    if (_typeToValidateFunc.count(funcKey) == 0) {
        _assertHelper->pushTextLine("Validate type \"" + funcKey + "\" is invalid")
                          ->fatalAssert("EventValidationConfigError");
        return true; // バリデーションエラーは回避
    }
    
    if (!_typeToValidateFunc.at(funcKey)(targetMember, checkType)) {
        _assertHelper->pushTextLine("Type did not match \"" + funcKey + "\".");
        return false;
    }
    return true;
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
        _assertHelper->pushTextLine("Array length did not match.");
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

bool EventScriptValidator::isEnum(const rapidjson::Value &target, const rapidjson::Value &enumName)
{
    string enumType = enumName.GetString();

    // コンフィグ文法チェック
    if (!VALIDATE_CONFIG[EventScriptValidator::ENUMS.c_str()][enumType.c_str()].IsArray()) {
        _assertHelper->pushTextLine("Enums definition is not array.")
                          ->fatalAssert("EventValidationConfigError");
        return true; // バリデーションはスルー
    }
    const rapidjson::Value& enumArray = VALIDATE_CONFIG[EventScriptValidator::ENUMS.c_str()][enumType.c_str()];
    
    // 対象が文字列かどうか
    if (!target.IsString()) {
        _assertHelper->pushTextLine("Enum member must be string!");
        return false;
    }
    string targetString = target.GetString();
    
    // enumの中に定義されている文字列かどうか
    int enumSize = enumArray.Size();
    for (int i = 0; i < enumSize; i++) {
        string targetEnum = enumArray[i].GetString();
        if (targetString == targetEnum) return true;
    }
    
    _assertHelper->pushTextLine("\"" + targetString + "\" is not exists in enum." + enumType + ".");
    return false;
}

bool EventScriptValidator::isBool(const rapidjson::Value &target, const rapidjson::Value& checkType)
{
    return target.IsBool();
}

#pragma mark -
#pragma mark Utils

// バリデート関数ポインタへのキーを取得
string EventScriptValidator::getValidateFuncKey(const rapidjson::Value &checkType)
{
    rapidjson::Type validateType = checkType.GetType();
    if (EventScriptValidator::TYPE_TO_VALIDATE_STRING.count(validateType) == 0) {
        // Assert
        return "ERROR";
    }
    
    string funcKey = EventScriptValidator::TYPE_TO_VALIDATE_STRING.at(validateType);
    if (funcKey == EventScriptValidator::NORMAL) {
        funcKey = checkType.GetString();
        if (VALIDATE_CONFIG[EventScriptValidator::ENUMS.c_str()].HasMember(funcKey.c_str())) {
            funcKey = EventScriptValidator::ENUM;
        }
        
    }
    
    return funcKey;
}
