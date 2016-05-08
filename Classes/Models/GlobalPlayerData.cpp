//
//  GlobalPlayerData.cpp
//  LastSupper
//
//  Created by Ryoya Ino on 2015/12/04.
//
//

#include "Models/GlobalPlayerData.h"

#include "Managers/CsvDataManager.h"
//#include "Managers/KeyconfigManager.h"
#include "Managers/NotificationManager.h"

#include "Utils/JsonUtils.h"
#include "Utils/StringUtils.h"

const string GlobalPlayerData::GLOBAL_DATA_PATH {"save/global"};
const string GlobalPlayerData::GLOBAL_TEMPLATE_PATH {"save/global_template"};

const char* GlobalPlayerData::STATUS {"status"};
const char* GlobalPlayerData::GLOBAL_ID {"global_id"};
const char* GlobalPlayerData::CLEAR_COUNT {"clear_count"};
const char* GlobalPlayerData::BEST_SAVE_COUNT {"best_save_count"};
const char* GlobalPlayerData::BEST_CLEAR_TIME {"best_clear_time"};
const char* GlobalPlayerData::TROPHY {"trophy"};
const char* GlobalPlayerData::TOKENS {"tokens"};
const char* GlobalPlayerData::BGM_VOLUME {"bgm_volume"};
const char* GlobalPlayerData::SE_VOLUME {"se_volume"};
const char* GlobalPlayerData::CURSOR_KEY {"cursor_key"};
const char* GlobalPlayerData::ENTER_KEY {"enter_key"};
const char* GlobalPlayerData::DASH_KEY {"dash_key"};

const int GlobalPlayerData::CHIKEN_SAVE_COUNT {50};
const int GlobalPlayerData::FAST_CLEAR_TIME {1800};

#pragma mark GlobalDataFile

// 初期化
bool GlobalPlayerData::init()
{
    if (this->loadGlobalData()) return true;
    if (this->initGlobalData()) return true;
    return false;
}

// グローバルデータのロード
bool GlobalPlayerData::loadGlobalData()
{
    string path = FileUtils::getInstance()->fullPathForFilename(GLOBAL_DATA_PATH + SAVE_EXTENSION);
    if (path == "") return false;
    this->globalData = LastSupper::JsonUtils::readJsonCrypted(path);
    return true;
}

// グローバルデータの初期化
bool GlobalPlayerData::initGlobalData()
{
    string path = FileUtils::getInstance()->fullPathForFilename(GLOBAL_TEMPLATE_PATH + SAVE_EXTENSION);
    if (path == "") return false;
    this->globalData = LastSupper::JsonUtils::readJsonCrypted(path);
    this->saveGlobalData();
    return true;
}

// グローバルデータのセーブ
void GlobalPlayerData::saveGlobalData()
{
    LastSupper::JsonUtils::writeJsonCrypt(GLOBAL_DATA_PATH + SAVE_EXTENSION, this->globalData);
}

#pragma mark -
#pragma mark Status

// statusの取得
int GlobalPlayerData::getStatus()
{
    return this->globalData[STATUS].GetInt();
}


#pragma mark -
#pragma mark ClearCount

// クリア回数をtokenチェックを行ってからセット
void GlobalPlayerData::setClearCount(const string& token)
{
    if (!this->isClearedToken(token))
    {
        int clear_count = this->globalData[CLEAR_COUNT].GetInt();
        if (clear_count < 999)
        {
            this->globalData[CLEAR_COUNT].SetInt(clear_count + 1);
        }
    }
}

// クリア回数を取得
int GlobalPlayerData::getClearCount(){
    if (!this->globalData.HasMember(CLEAR_COUNT)) return 0;
    return this->globalData[CLEAR_COUNT].GetInt();
}

// クリアしているか
bool GlobalPlayerData::isCleared(){return this->getClearCount() > 0 ? true : false;}

#pragma mark -
#pragma mark MinimumSaveCount

// 最小セーブ記録をセット
void GlobalPlayerData::setBestSaveCount(const int save_count)
{
    // トロフィーチェック
    if (save_count == 0) this->setTrophy(11); // マエストロ
    
    if (save_count > this->getBestSaveCount()) return;
    this->globalData[BEST_SAVE_COUNT].SetInt(save_count);
}

// セーブ回数の最小記録を取得
int GlobalPlayerData::getBestSaveCount(){return this->globalData[BEST_SAVE_COUNT].GetInt();}

#pragma mark -
#pragma mark BestClearTime

// 最短クリア時間をセット
void GlobalPlayerData::setBestClearTime(const int play_time)
{
    if (play_time > this->getBestClearTimeSecound()) return;
    this->globalData[BEST_CLEAR_TIME].SetInt(play_time);
    
    // トロフィーチェック
    if (play_time < FAST_CLEAR_TIME) this->setTrophy(10); // 3分クッキング
}

// 最短クリア時間を取得
int GlobalPlayerData::getBestClearTimeSecound(){return this->globalData[BEST_CLEAR_TIME].GetInt();}

#pragma mark -
#pragma mark TrophyData

// トロフィーゲット処理
void GlobalPlayerData::setTrophy(const int trophy_id)
{
    char tid_char[10];
    sprintf(tid_char, "%d", trophy_id);
    rapidjson::Value tid  (kStringType);
    tid.SetString(tid_char, strlen(tid_char), this->globalData.GetAllocator());
    rapidjson::Value::ConstMemberIterator itr = this->globalData[TROPHY].FindMember(tid_char);
    
    // 指定したトロフィーIDが存在するかチェック
    if(!this->hasTrophy(trophy_id)){
        // keyを作成してtrueを登録
        if (itr == this->globalData[TROPHY].MemberEnd())
        {
            this->globalData[TROPHY].AddMember(tid, rapidjson::Value(true), this->globalData.GetAllocator());
        }
        else
        {
            this->globalData[TROPHY][tid].SetBool(true);
        }
        
        // トロフィーゲット通知
        NotificationManager::getInstance()->notifyTrophy(trophy_id);
        
        // トロコンチェック
        this->setTrophyComplete();
        
        // グローバルデータをセーブ　　　
        this->saveGlobalData();
    }
}

// 指定のトロフィーを持っているか
bool GlobalPlayerData::hasTrophy(const int trophy_id)
{
    bool hasTrophy {false};
    char tid_char[10];
    sprintf(tid_char, "%d", trophy_id);
    
    if (this->globalData[TROPHY].HasMember(tid_char))
    {
        if (this->globalData[TROPHY][tid_char].GetBool())
        {
            hasTrophy =  true;
        }
    }
    return hasTrophy;
}

// トロフィーコンプリート処理
void GlobalPlayerData::setTrophyComplete()
{
    vector<int> trophies = CsvDataManager::getInstance()->getTrophyData()->getIdAll();
    int trophy_count {0};
    for(int trophy_id : trophies)
    {
        if (this->hasTrophy(trophy_id)) trophy_count++;
    }
    int tro_size  = trophies.size();
    if (trophy_count >= tro_size - 1)
    {
        this->setTrophy(tro_size);
    }
}

#pragma mark -
#pragma mark Token

// トークンをセット
void GlobalPlayerData::setClearToken(const string& token)
{
    rapidjson::Value jtoken  (kStringType);
    jtoken.SetString(token.c_str(), strlen(token.c_str()), this->globalData.GetAllocator());
    this->globalData[TOKENS].PushBack(jtoken, this->globalData.GetAllocator());
}

// クリア済みのデータかチェックする
bool GlobalPlayerData::isClearedToken(const string &token)
{
    int token_count = this->globalData[TOKENS].Size();
    for (int i = 0; i < token_count; i++)
    {
        if (this->globalData[TOKENS][i].GetString() == token)
        {
            return true;
        }
    }
    return false;
}

#pragma mark -
#pragma mark Volume

// BGMのマスターボリュームをセット
void GlobalPlayerData::setBgmVolume(const float& volume)
{
    this->globalData[BGM_VOLUME].SetDouble(volume);
}

// BGMのマスターボリュームをゲット
float GlobalPlayerData::getBgmVolume()
{
    if (!this->globalData.HasMember(BGM_VOLUME)) return 0.5;
    return this->globalData[BGM_VOLUME].GetDouble();
}

// SEのマスターボリュームをセット
void GlobalPlayerData::setSeVolume(const float &volume)
{
    this->globalData[SE_VOLUME].SetDouble(volume);
}

// SEのマスターボリュームをゲット
float GlobalPlayerData::getSeVolume()
{
    if (!this->globalData.HasMember(SE_VOLUME)) return 0.5;
    return this->globalData[SE_VOLUME].GetDouble();
}

#pragma mark -
#pragma mark KeyConfig

// 移動キーをセット
void GlobalPlayerData::setCursorKey(const KeyconfigManager::CursorKeyType keyType)
{
    int keyNum {etoi(keyType)};
    if (this->globalData.HasMember(CURSOR_KEY))
    {
        this->globalData[CURSOR_KEY].SetInt(keyNum);
    }
    else
    {
        rapidjson::Value kid (kStringType);
        kid.SetString(CURSOR_KEY, strlen(CURSOR_KEY), this->globalData.GetAllocator());
        this->globalData.AddMember(kid, rapidjson::Value(keyNum), this->globalData.GetAllocator());
    }
}

// 移動キーを取得 (デフォルト矢印)
KeyconfigManager::CursorKeyType GlobalPlayerData::getCursorKey()
{
    if (!this->globalData.HasMember(CURSOR_KEY)) return KeyconfigManager::CursorKeyType::ARROW;
    return static_cast<KeyconfigManager::CursorKeyType>(this->globalData[CURSOR_KEY].GetInt());
}

// ダッシュキーをセット
void GlobalPlayerData::setDashKey(const KeyconfigManager::DashKeyType keyType)
{
    int keyNum {etoi(keyType)};
    if (this->globalData.HasMember(DASH_KEY))
    {
        this->globalData[DASH_KEY].SetInt(keyNum);
    }
    else
    {
        rapidjson::Value kid (kStringType);
        kid.SetString(DASH_KEY, strlen(DASH_KEY), this->globalData.GetAllocator());
        this->globalData.AddMember(kid, rapidjson::Value(keyNum), this->globalData.GetAllocator());
    }
}

// ダッシュキーを取得 (デフォルト左SHIFT)
KeyconfigManager::DashKeyType GlobalPlayerData::getDashKey()
{
    if (!this->globalData.HasMember(DASH_KEY)) return KeyconfigManager::DashKeyType::LEFT_SHIFT;
    return static_cast<KeyconfigManager::DashKeyType>(this->globalData[DASH_KEY].GetInt());
}

// キーコンフィグのセーブ
void GlobalPlayerData::saveKeyConfig(const KeyconfigManager::CursorKeyType cursorKey, const KeyconfigManager::DashKeyType dashKey)
{
    this->setCursorKey(cursorKey);
    this->setDashKey(dashKey);
    this->saveGlobalData();
}