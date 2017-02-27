//
//  LocalPlayerData.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#include "Models/PlayerData/LocalPlayerData.h"

#include "Managers/ConfigDataManager.h"
#include "Models/PlayerData/GlobalPlayerData.h"
#include "Utils/AssertUtils.h"
#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"
#include "Datas/MapObject/CharacterData.h"

const char* LocalPlayerData::STATUS {"status"};
const char* LocalPlayerData::TOKEN {"token"};
const char* LocalPlayerData::SAVE_COUNT {"save_count"};
const char* LocalPlayerData::PLAY_TIME {"play_time"};
const char* LocalPlayerData::CHAPTER {"chapter"};
const char* LocalPlayerData::EQUIPMENT {"equipment"};
const char* LocalPlayerData::MAP_ID {"map_id"};
const char* LocalPlayerData::PARTY {"party"};
const char* LocalPlayerData::CHARA_ID {"chara_id"};
const char* LocalPlayerData::OBJ_ID {"obj_id"};
const char* LocalPlayerData::X {"x"};
const char* LocalPlayerData::Y {"y"};
const char* LocalPlayerData::DIRECTION {"direction"};
const char* LocalPlayerData::FRIENDSHIP {"friendship"};
const char* LocalPlayerData::EVENT {"event"};
const char* LocalPlayerData::CHARA {"chara"};
const char* LocalPlayerData::ITEM {"item"};
const char* LocalPlayerData::ITEMS {"items"};
const char* LocalPlayerData::BGM {"bgm"};
const char* LocalPlayerData::SPECIAL_ROOM {"special_room"};
const char* LocalPlayerData::INIT_EVENT_ID {"init_event_id"};

const int LocalPlayerData::MAX_COUNT {999};
const int LocalPlayerData::MAX_PLAY_TIME {35999};

#pragma mark LocalDataFile

// 初期化
bool LocalPlayerData::init(const int local_id)
{
    return this->loadLocalData(local_id);
}

// ローカルセーブデータの読み込み
bool LocalPlayerData::loadLocalData(const int local_id)
{
    string file = (local_id == 0) ? "save/local_template": "save/local" + to_string(local_id);
    file += SAVE_EXTENSION;
    string path = FileUtils::getInstance()->fullPathForFilename(file);
    if (path == "") return false;
    
    this->local_id = local_id;
    this->localData = LastSupper::JsonUtils::readJsonCrypted(path);
    if (this->localData[STATUS].GetInt() == 0) return false;
    return true;
}

// ローカルデータのセーブ
void LocalPlayerData::saveLocalData(const int local_id)
{
    // セーブ回数のインクリメント
    this->incrementSaveCount();

    string path = "save/local" + to_string(local_id) + SAVE_EXTENSION;
    LastSupper::JsonUtils::writeJsonCrypt(path, this->localData);
    this->local_id = local_id;
}

// ローカルセーブデータのIDを返す
int LocalPlayerData::getLocalId(){return this->local_id;}

// おまけ部屋かどうか
bool LocalPlayerData::isSpecialRoom()
{
    if (!this->localData.HasMember(LocalPlayerData::SPECIAL_ROOM)) return false;
    return this->localData[LocalPlayerData::SPECIAL_ROOM].GetBool();
}

#pragma mark - 
#pragma mark Token

// トークンをセットする
void LocalPlayerData::setToken()
{
    rapidjson::Value token(kStringType);
    string ranstr = LastSupper::StringUtils::getRandomString();
    this->localData[TOKEN].SetString(ranstr.c_str(), strlen(ranstr.c_str()), this->localData.GetAllocator());
}

// トークンを取得
string LocalPlayerData::getToken(){return this->localData[TOKEN].GetString();}

#pragma mark -
#pragma mark SaveCount

// セーブ回数をインクリメント
void LocalPlayerData::incrementSaveCount()
{
    // 現在のセーブ回数取得
    int save_count = this->getSaveCount();
    
    // 回数のインクリメント
    if (save_count < this->MAX_COUNT){
        save_count++;
        this->localData[SAVE_COUNT].SetInt(save_count);
    };
    
    // トロフィーチェック
    TrophyConfigData* trophyConfigData { ConfigDataManager::getInstance()->getTrophyConfigData() };
    int chikenSaveCount = trophyConfigData->getTrophyIntegerProperty(TrophyConfigData::CHIKEN_SAVE, TrophyConfigData::COUNT);
    int chikenSaveTrophyId = trophyConfigData->getTrophyId(TrophyConfigData::CHIKEN_SAVE);
    if (save_count >= chikenSaveCount) this->setTrophy(chikenSaveTrophyId);
}

// セーブ回数を取得
int LocalPlayerData::getSaveCount(){return this->localData[SAVE_COUNT].GetInt();}

#pragma mark -
#pragma mark PlayTime

// プレイ時間をセット
void LocalPlayerData::setPlayTime(const int secound)
{
    int playTime = secound;
    if (playTime > MAX_PLAY_TIME) playTime = MAX_PLAY_TIME;
    this->localData[PLAY_TIME].SetInt(playTime);
}

// プレイ秒数をintで取得
int LocalPlayerData::getPlayTime(){return this->localData[PLAY_TIME].GetInt();}

// プレイ時間を表示用stringで取得
string LocalPlayerData::getPlayTimeForDisplay(){return LastSupper::StringUtils::getTimeForDisplay(this->getPlayTime());}

#pragma mark -
#pragma mark Chapter

// チャプターIDのセット
void LocalPlayerData::setChapterId(const int chapter_id){this->localData[CHAPTER].SetInt(chapter_id);}

// チャプターIDを取得
int LocalPlayerData::getChapterId(){return this->localData[CHAPTER].GetInt();}

#pragma mark -
#pragma mark Friendship

// 友好度をセット
void LocalPlayerData::setFriendship(const int chara_id, const int level)
{
    char cidChar[10];
    sprintf(cidChar, "%d", chara_id);
    this->localData[FRIENDSHIP][cidChar].SetInt(level);
    if (level == 2 && this->setTrophy != nullptr) this->setTrophy(chara_id);
}

// 友好度を取得
int LocalPlayerData::getFriendship(const int chara_id)
{
    char cidChar[10];
    sprintf(cidChar, "%d", chara_id);
    rapidjson::Value::ConstMemberIterator itr = this->localData[FRIENDSHIP].FindMember(cidChar);
    return itr != this->localData[FRIENDSHIP].MemberEnd() ? this->localData[FRIENDSHIP][cidChar].GetInt() : -1;
}

// 友好度が指定の値と一致するかどうか
bool LocalPlayerData::checkFriendship(const int chara_id, const int level)
{
    int value = this->getFriendship(chara_id);
    return level == value ? true : false;
}

// 友好度がMAXの人数を取得
int LocalPlayerData::getMaxFriendshipCount()
{
    int maxCount {0};
    for (int i = 1; i <= 5; i++)
    {
        if (this->checkFriendship(i, 2)) maxCount++;
    }
    return maxCount;
}
#pragma mark -
#pragma mark Items

// charaIDに対してのitemのオブジェクトをセットする
void LocalPlayerData::setItemsObject(char * charaId)
{
    rapidjson::Value& items {this->localData[ITEMS]};
    if (!items.HasMember(charaId))
    {
        rapidjson::Value cidStr (kStringType);
        cidStr.SetString(charaId, strlen(charaId), this->localData.GetAllocator());
        
        items.AddMember(cidStr, rapidjson::Value(kObjectType), this->localData.GetAllocator());
        
        string equipmentLeft { JsonKey::getEquipmentKey(Direction::LEFT) };
        cidStr.SetString(equipmentLeft.c_str(), equipmentLeft.length(), this->localData.GetAllocator());
        items[charaId].AddMember(cidStr, rapidjson::Value(0), this->localData.GetAllocator());
        
        string equipmentRight { JsonKey::getEquipmentKey(Direction::RIGHT) };
        cidStr.SetString(equipmentRight.c_str(), equipmentRight.length(), this->localData.GetAllocator());
        items[charaId].AddMember(cidStr, rapidjson::Value(0), this->localData.GetAllocator());
        
        cidStr.SetString(ITEM, strlen(ITEM), this->localData.GetAllocator());
        items[charaId].AddMember(cidStr, rapidjson::Value(kArrayType), this->localData.GetAllocator());
    }
}

#pragma mark Equipment

// 装備をセット
void LocalPlayerData::setItemEquipment(const Direction direction, const int itemId)
{
    char cidChar[10];
    LastSupper::StringUtils::setCharsFromInt(cidChar, this->getMainCharaId());
    this->setItemsObject(cidChar);
    const string dir = JsonKey::getEquipmentKey(direction);
    this->localData[ITEMS][cidChar][dir.c_str()].SetInt(itemId);
}

// 装備してるアイテムIDを取得
int LocalPlayerData::getItemEquipment(const Direction direction)
{
    char cidChar[10];
    LastSupper::StringUtils::setCharsFromInt(cidChar, this->getMainCharaId());
    this->setItemsObject(cidChar);
    const string dir { JsonKey::getEquipmentKey(direction) };
    return this->localData[ITEMS][cidChar][dir.c_str()].GetInt();
}

// 指定のアイテムを装備しているかどうか
bool LocalPlayerData::isEquipedItem(const int itemId)
{
    int right = this->getItemEquipment(Direction::RIGHT);
    int left = this->getItemEquipment(Direction::LEFT);
    bool isEquiped  = (itemId == right || itemId == left) ? true : false;
    return isEquiped;
}

#pragma mark Item

// アイテムゲット時の処理
void LocalPlayerData::setItem(const int itemId)
{
    this->setItem(this->getMainCharaId(), itemId);
}

void LocalPlayerData::setItem(const int charaId, const int itemId)
{
    char iidChar[10];
    char cidChar[10];
    LastSupper::StringUtils::setCharsFromInt(cidChar, charaId);
    this->setItemsObject(cidChar);
    sprintf(iidChar, "%d", itemId);
    rapidjson::Value iid  (kStringType);
    iid.SetString(iidChar, strlen(iidChar), this->localData.GetAllocator());
    this->localData[ITEMS][cidChar][ITEM].PushBack(iid, this->localData.GetAllocator());
}

// アイテムを消費
bool LocalPlayerData::removeItem(const int itemId)
{
    return this->removeItem(this->getMainCharaId(), itemId);
}

bool LocalPlayerData::removeItem(const int charaId, const int itemId)
{
    bool isExist = false;
    char cidChar[10];
    LastSupper::StringUtils::setCharsFromInt(cidChar, charaId);
    this->setItemsObject(cidChar);
    vector<int> items {this->getItemAll()};
    this->localData[ITEMS][cidChar][ITEM].Clear();
    this->localData[ITEMS][cidChar][ITEM].SetArray();
    int itemCount = items.size();
    for(int i = 0; i < itemCount; i++)
    {
        if (items[i] == itemId && !isExist)
        {
            isExist = true;
            
            // 右手を確認
            if(itemId == this->getItemEquipment(Direction::RIGHT))
            {
                this->setItemEquipment(Direction::RIGHT, 0);
            }
            // 左手を確認
            if (itemId == this->getItemEquipment(Direction::LEFT))
            {
                this->setItemEquipment(Direction::LEFT, 0);
            }
        }
        else
        {
            char iid_char[10];
            sprintf(iid_char, "%d", items[i]);
            rapidjson::Value iid  (kStringType);
            iid.SetString(iid_char, strlen(iid_char), this->localData.GetAllocator());
            this->localData[ITEMS][cidChar][ITEM].PushBack(iid, this->localData.GetAllocator());
        } 
    }
    return isExist;
}

// 所持しているアイテムをすべて取得
vector<int> LocalPlayerData::getItemAll()
{
    return this->getItemAll(this->getMainCharaId());
}

vector<int> LocalPlayerData::getItemAll(const int charaId)
{
    vector<int> items {};
    char cidChar[10];
    LastSupper::StringUtils::setCharsFromInt(cidChar, charaId);
    this->setItemsObject(cidChar);
    rapidjson::Value& itemList = this->localData[ITEMS][cidChar][ITEM];
    int itemCount = itemList.Size();
    for(int i = 0; i < itemCount; i++)
    {
        items.push_back(stoi(itemList[i].GetString()));
    }
    return items;
} 

// アイテムを所持しているか確認
bool LocalPlayerData::hasItem(const int itemId)
{
    return this->hasItem(this->getMainCharaId(), itemId);
}

bool LocalPlayerData::hasItem(const int charaId, const int itemId)
{
    char cidChar[10];
    LastSupper::StringUtils::setCharsFromInt(cidChar, this->getMainCharaId());
    this->setItemsObject(cidChar);
    rapidjson::Value& itemList = this->localData[ITEMS][cidChar][ITEM];
    int itemCount = itemList.Size();
    for(int i = 0; i < itemCount; i++)
    {
        if (stoi(itemList[i].GetString()) == itemId) return true;
    }
    return false;
}

#pragma mark -
#pragma mark CharacterProfile

// キャラクタープロフィールの見れるレベルをセット
void LocalPlayerData::setCharacterProfile(const int chara_id, const int level)
{
    char cidChar[10];
    sprintf(cidChar, "%d", chara_id);
    rapidjson::Value::ConstMemberIterator itr = this->localData[CHARA].FindMember(cidChar);
    if (itr != this->localData[CHARA].MemberEnd())
    {
        // すでに追加されているキャラクター
        this->localData[CHARA][cidChar].SetInt(level);
    }
    else
    {
        // 初めて追加するキャラ
        rapidjson::Value id (kStringType);
        id.SetString(cidChar,strlen(cidChar),this->localData.GetAllocator());
        this->localData[CHARA].AddMember(id, rapidjson::Value(level), this->localData.GetAllocator());
    }
}

// キャラクターのプロフィール情報の見れるレベルを取得
int LocalPlayerData::getCharacterProfileLevel(const int chara_id)
{
    int level {-1}; // 存在しない場合は-1を返す
    char cidChar[10];
    sprintf(cidChar, "%d", chara_id);
    rapidjson::Value::ConstMemberIterator itr = this->localData[CHARA].FindMember(cidChar);
    if (itr != this->localData[CHARA].MemberEnd())
    {
        level = this->localData[CHARA][cidChar].GetInt();
    }
    return level;
}

#pragma mark -
#pragma mark Event

// イベントフラグのセット (イベントステータスへ変換 true>>1, false>>0)
void LocalPlayerData::setEventNeverAgain(const int map_id, const int event_id, const bool flag)
{
    // status取得
    int status {this->getEventStatus(map_id, event_id)};
    
    // statusを負の値にする
    if (status == 0 && flag)
    {
        status = -1; // 初回の場合
    }
    else if ( (status > 0 && flag) || (status < 0 && !flag) )
    {
        status = status * -1;
    }
    else
    {
        status = 1;
    }
    
    // イベントステータスとしてセット
    this->setEventStatus(map_id, event_id, status);
}

void LocalPlayerData::setEventStatus(const int map_id, const int event_id, const int status)
{
    // map_idが存在するかチェック
    char mid_char[10];
    sprintf(mid_char, "%d", map_id);
    rapidjson::Value mid  (kStringType);
    mid.SetString(mid_char, strlen(mid_char), this->localData.GetAllocator());
    rapidjson::Value::ConstMemberIterator itr = this->localData[EVENT].FindMember(mid_char);
    if(itr == this->localData[EVENT].MemberEnd()){
        // 存在しない場合は作成
        this->localData[EVENT].AddMember(mid, rapidjson::Value(kObjectType), this->localData.GetAllocator());
    }
    
    // event_idが存在するかチェック
    char eid_char[10];
    sprintf(eid_char, "%d", event_id);
    rapidjson::Value eid (kStringType);
    eid.SetString(eid_char,strlen(eid_char),this->localData.GetAllocator());
    itr = this->localData[EVENT][mid_char].FindMember(eid_char);
    if(itr == this->localData[EVENT][mid_char].MemberEnd()){
        this->localData[EVENT][mid_char].AddMember(eid, rapidjson::Value(status), this->localData.GetAllocator());
    } else {
        bool negative = itr->value.GetInt() < 0 ? true : false;
        int new_status = status < 0 ? status : (negative) ? status * -1 : status;
        this->localData[EVENT][mid_char][eid_char].SetInt(new_status);
    }
    return;
}

// イベントフラグの取得
int LocalPlayerData::getEventStatus(const int map_id, const int event_id)
{
    // map_idが存在するかチェック
    char mid_char[10];
    sprintf(mid_char, "%d", map_id);
    rapidjson::Value& event = this->localData[EVENT];
    rapidjson::Value::ConstMemberIterator itr = event.FindMember(mid_char);
    if(itr == event.MemberEnd()){
        return 0;
    }
    
    // event_idが存在するかチェック
    char eid_char[10];
    sprintf(eid_char, "%d", event_id);
    itr = event[mid_char].FindMember(eid_char);
    if(itr == event[mid_char].MemberEnd()){
        return 0;
    } else {
        return event[mid_char][eid_char].GetInt();
    }
}

// イベントを見たかどうか
bool LocalPlayerData::checkEventIsDone(const int map_id, const int event_id)
{
    int status = this->getEventStatus(map_id, event_id);
    return status < 0 ? true : false;
}

// イベントステータスが指定の値かどうか
bool LocalPlayerData::checkEventStatus(const int map_id, const int event_id, const int status)
{
    return status == abs(this->getEventStatus(map_id, event_id)) ? true : false;
}

#pragma mark -
#pragma mark Party

// パーティの座標のセット
void LocalPlayerData::setLocation(const vector<CharacterData>& characters)
{
    int chara_count = characters.size();
    for (int i = 0; i < chara_count; i++)
    {
        Location location {characters[i].location};
        if (i == 0) this->localData[MAP_ID].SetInt(location.map_id);
        this->localData[PARTY][i][X].SetInt(location.x);
        this->localData[PARTY][i][Y].SetInt(location.y);
        this->localData[PARTY][i][DIRECTION].SetInt(location.direction.getInt());
    }
}

// 主人公の位置をゲット
Location LocalPlayerData::getLocation(const int num)
{
    int map_id = this->localData[MAP_ID].GetInt();
    rapidjson::Value& chara = this->localData[PARTY][num];
    Location location(map_id, chara[X].GetInt(), chara[Y].GetInt(), chara[DIRECTION].GetInt());
    return location;
}


// パーティメンバーを追加する
void LocalPlayerData::setPartyMember(const CharacterData& chara)
{
    
    rapidjson::Document member;
    member.SetObject();
    rapidjson::Value jval (kStringType);
    
    // chara_id
    jval.SetString(CHARA_ID, strlen(CHARA_ID), this->localData.GetAllocator());
    member.AddMember(jval, rapidjson::Value(chara.chara_id), this->localData.GetAllocator());
    
    // obj_id
    jval.SetString(OBJ_ID, strlen(OBJ_ID), this->localData.GetAllocator());
    member.AddMember(jval, rapidjson::Value(chara.obj_id), this->localData.GetAllocator());
    
    // x
    jval.SetString(X, strlen(X), this->localData.GetAllocator());
    member.AddMember(jval, rapidjson::Value(chara.location.x), this->localData.GetAllocator());
    
    // y
    jval.SetString(Y, strlen(Y), this->localData.GetAllocator());
    member.AddMember(jval, rapidjson::Value(chara.location.y), this->localData.GetAllocator());
    
    // direction
    jval.SetString(DIRECTION, strlen(DIRECTION), this->localData.GetAllocator());
    member.AddMember(jval, rapidjson::Value(chara.location.direction.getInt()), this->localData.GetAllocator());
    
    this->localData[PARTY].PushBack(member, this->localData.GetAllocator());
}

// パーティメンバーを解除する
bool LocalPlayerData::removePartyMember(const int obj_id)
{
    bool isExsits = false;
    vector<CharacterData> members = this->getPartyMemberAll();
    int member_size = members.size();
    this->localData[PARTY].Clear();
    this->localData[PARTY].SetArray();
    for (int i = 0; i < member_size; i++)
    {
        if (members[i].obj_id == obj_id)
        {
            isExsits = true;
        }
        else
        {
            this->setPartyMember(members[i]);
        }
    }
    return isExsits;
}

// パーティーメンバーを全削除
void LocalPlayerData::removePartyMemberAll()
{
    this->localData[PARTY].Clear();
    this->localData[PARTY].SetArray();
}

// パーティメンバーを取得
CharacterData LocalPlayerData::getPartyMember(const int num)
{
    rapidjson::Value& chara = this->localData[PARTY][num];
    Location location(this->localData[MAP_ID].GetInt(), chara[X].GetInt(), chara[Y].GetInt(), chara[DIRECTION].GetInt());
    return CharacterData(chara[CHARA_ID].GetInt(), chara[OBJ_ID].GetInt(), location);
}

// パーティメンバーを全取得
vector<CharacterData> LocalPlayerData::getPartyMemberAll()
{
    vector<CharacterData> party {};
    int party_size = this->localData[PARTY].Size();
    for (int i = 0; i < party_size; i++)
    {
        party.push_back(this->getPartyMember(i));
    }
    return party;
}

// 主人公のキャラIDを取得
int LocalPlayerData::getMainCharaId()
{
    return this->localData[PARTY][0][CHARA_ID].GetInt();
}

// パーティーメンバーの正面ドット絵を取得
vector<string> LocalPlayerData::getPartyCharaDotImgFileNames()
{
    vector<string> party {};
    int partySize = this->localData[PARTY].Size();
    for (int i = 0; i < partySize; i++)
    {
        party.push_back(this->getPartyMember(i).getDotFileName());
    }
    return party;
}

#pragma mark -
#pragma mark BGM

// BGMをセット
void LocalPlayerData::setBgm(const string &bgm_name)
{
    rapidjson::Value bgname  (kStringType);
    bgname.SetString(bgm_name.c_str(), strlen(bgm_name.c_str()), this->localData.GetAllocator());
    this->localData[BGM].PushBack(bgname, this->localData.GetAllocator());
}

// BGMを削除
bool LocalPlayerData::removeBgm(const string &bgm_name)
{
    bool isExist = false;
    vector<string> bgms {this->getBgmAll()};
    this->localData[BGM].Clear();
    this->localData[BGM].SetArray();
    int bgmCount = bgms.size();
    for(int i = 0; i < bgmCount; i++)
    {
        if (bgms[i] == bgm_name)
        {
            isExist = true;
        }
        else
        {
            rapidjson::Value bgname  (kStringType);
            bgname.SetString(bgms[i].c_str(), strlen(bgms[i].c_str()), this->localData.GetAllocator());
            this->localData[BGM].PushBack(bgname, this->localData.GetAllocator());
        }
    }
    return isExist;
}

// BGMを全削除
void LocalPlayerData::removeBgmAll()
{
    this->localData[BGM].Clear();
    this->localData[BGM].SetArray();
}

// BGMを取得
vector<string> LocalPlayerData::getBgmAll()
{
    vector<string> bgms {};
    rapidjson::Value& bgmList = this->localData[BGM];
    int bgmCount = bgmList.Size();
    for(int i = 0; i < bgmCount; i++)
    {
        bgms.push_back(bgmList[i].GetString());
    }
    return bgms;
}

// init_event_idをセット
void LocalPlayerData::setInitEventId(const string &initEventId)
{
    // 既存メンバ上書き
    if (this->localData.HasMember(LocalPlayerData::INIT_EVENT_ID)) {
        this->localData[LocalPlayerData::INIT_EVENT_ID].SetString(initEventId.c_str(), strlen(initEventId.c_str()));
        return;
    }
    
    // 新規メンバ作成
    rapidjson::Value eventIdJsonValue  (kStringType);
    eventIdJsonValue.SetString(initEventId.c_str(), strlen(initEventId.c_str()), this->localData.GetAllocator());
    
    rapidjson::Value initEventIdKey (kStringType);
    initEventIdKey.SetString(LocalPlayerData::INIT_EVENT_ID, strlen(LocalPlayerData::INIT_EVENT_ID), this->localData.GetAllocator());
    
    this->localData.AddMember(initEventIdKey, eventIdJsonValue, this->localData.GetAllocator());

}

// init_event_idを削除
void LocalPlayerData::removeInitEventId()
{
    if (this->localData.HasMember(LocalPlayerData::INIT_EVENT_ID)) {
        this->localData.RemoveMember(LocalPlayerData::INIT_EVENT_ID);
    }
}

// init_event_idを取得、なければ空文字
string LocalPlayerData::getInitEventId()
{
    if (this->localData.HasMember(LocalPlayerData::INIT_EVENT_ID)) {
        return this->localData[LocalPlayerData::INIT_EVENT_ID].GetString();
    }
    
    return "";
}

#pragma mark -
#pragma mark JsonKey

string LocalPlayerData::JsonKey::getEquipmentKey(const Direction& direction)
{
    return string(EQUIPMENT) + "_" + direction.getDowncaseString();
}
