//
//  LocalPlayerData.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#include "Models/LocalPlayerData.h"
#include "Models/GlobalPlayerData.h"
#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"
#include "Datas/MapObject/CharacterData.h"

const char* LocalPlayerData::STATUS {"status"};
const char* LocalPlayerData::TOKEN {"token"};
const char* LocalPlayerData::SAVE_COUNT {"save_count"};
const char* LocalPlayerData::PLAY_TIME {"play_time"};
const char* LocalPlayerData::CHAPTER {"chapter"};
const char* LocalPlayerData::EQUIPMENT_RIGHT {"equipment_right"};
const char* LocalPlayerData::EQUIPMENT_LEFT {"equipment_left"};
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
const char* LocalPlayerData::BGM {"bgm"};

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
    if (save_count >= GlobalPlayerData::CHIKEN_SAVE_COUNT) this->setTrophy(9);
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
    char cid_char[10];
    sprintf(cid_char, "%d", chara_id);
    this->localData[FRIENDSHIP][cid_char].SetInt(level);
    if (level == 2 && this->setTrophy != nullptr) this->setTrophy(chara_id);
}

// 友好度を取得
int LocalPlayerData::getFriendship(const int chara_id)
{
    char cid_char[10];
    sprintf(cid_char, "%d", chara_id);
    rapidjson::Value::ConstMemberIterator itr = this->localData[FRIENDSHIP].FindMember(cid_char);
    return itr != this->localData[FRIENDSHIP].MemberEnd() ? this->localData[FRIENDSHIP][cid_char].GetInt() : -1;
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
#pragma mark Equipment

// 装備をセット
void LocalPlayerData::setItemEquipment(const Direction direction, const int item_id)
{
    const char* dir = direction == Direction::LEFT ? EQUIPMENT_LEFT : EQUIPMENT_RIGHT;
    this->localData[dir].SetInt(item_id);
}

// 装備してるアイテムIDを取得
int LocalPlayerData::getItemEquipment(const Direction direction)
{
    const char* dir = direction == Direction::LEFT ? EQUIPMENT_LEFT : EQUIPMENT_RIGHT;
    return this->localData[dir].GetInt();
}

// 指定のアイテムを装備しているかどうか
bool LocalPlayerData::isEquipedItem(const int item_id)
{
    int right = this->getItemEquipment(Direction::RIGHT);
    int left = this->getItemEquipment(Direction::LEFT);
    bool isEquiped  = (item_id == right || item_id == left) ? true : false;
    return isEquiped;
}

#pragma mark -
#pragma mark Item

// アイテムゲット時の処理
void LocalPlayerData::setItem(const int item_id){
    char iid_char[10];
    sprintf(iid_char, "%d", item_id);
    rapidjson::Value iid  (kStringType);
    iid.SetString(iid_char, strlen(iid_char), this->localData.GetAllocator());
    this->localData[ITEM].PushBack(iid, this->localData.GetAllocator());
}

// アイテムを消費
bool LocalPlayerData::removeItem(const int item_id)
{
    bool isExist = false;
    vector<int> items {this->getItemAll()};
    this->localData[ITEM].Clear();
    this->localData[ITEM].SetArray();
    int itemCount = items.size();
    for(int i = 0; i < itemCount; i++)
    {
        if (items[i] == item_id && !isExist)
        {
            isExist = true;
            
            // 右手を確認
            if(item_id == this->getItemEquipment(Direction::RIGHT))
            {
                this->setItemEquipment(Direction::RIGHT, 0);
            }
            // 左手を確認
            if (item_id == this->getItemEquipment(Direction::LEFT))
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
            this->localData[ITEM].PushBack(iid, this->localData.GetAllocator());
        } 
    }
    return isExist;
}

// 所持しているアイテムをすべて取得
vector<int> LocalPlayerData::getItemAll()
{
    vector<int> items {};
    rapidjson::Value& itemList = this->localData[ITEM];
    int itemCount = itemList.Size();
    for(int i = 0; i < itemCount; i++)
    {
        items.push_back(stoi(itemList[i].GetString()));
    }
    return items;
} 

// アイテムを所持しているか確認
bool LocalPlayerData::hasItem(const int item_id)
{
    rapidjson::Value& itemList = this->localData[ITEM];
    int itemCount = itemList.Size();
    for(int i = 0; i < itemCount; i++)
    {
        if (stoi(itemList[i].GetString()) == item_id) return true;
    }
    return false;
}

#pragma mark -
#pragma mark CharacterProfile

// キャラクタープロフィールの見れるレベルをセット
void LocalPlayerData::setCharacterProfile(const int chara_id, const int level)
{
    char cid_char[10];
    sprintf(cid_char, "%d", chara_id);
    rapidjson::Value::ConstMemberIterator itr = this->localData[CHARA].FindMember(cid_char);
    if (itr != this->localData[CHARA].MemberEnd())
    {
        // すでに追加されているキャラクター
        this->localData[CHARA][cid_char].SetInt(level);
    }
    else
    {
        // 初めて追加するキャラ
        rapidjson::Value id (kStringType);
        id.SetString(cid_char,strlen(cid_char),this->localData.GetAllocator());
        this->localData[CHARA].AddMember(id, rapidjson::Value(level), this->localData.GetAllocator());
    }
}

// キャラクターのプロフィール情報の見れるレベルを取得
int LocalPlayerData::getCharacterProfileLevel(const int chara_id)
{
    int level {-1}; // 存在しない場合は-1を返す
    char cid_char[10];
    sprintf(cid_char, "%d", chara_id);
    rapidjson::Value::ConstMemberIterator itr = this->localData[CHARA].FindMember(cid_char);
    if (itr != this->localData[CHARA].MemberEnd())
    {
        level = this->localData[CHARA][cid_char].GetInt();
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
        this->localData[PARTY][i][DIRECTION].SetInt(static_cast<int>(location.direction));
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
    member.AddMember(jval, rapidjson::Value(static_cast<int>(chara.location.direction)), this->localData.GetAllocator());
    
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
