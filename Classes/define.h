#ifndef  __DEFINE_H__
#define  __DEFINE_H__

// Common.hにインクルードされるファイル群(Manager系)はこちらをインクルードする
// 最低限のインクルード、変数の初期化を行う

#include <cocos2d.h>
#include <iostream>
#include <fstream>
#include "AudioEngine.h"
#include "resources.h"
#include "env.h"
#include "Models/Direction.h"

// rapidjson
#include "external/json/document.h"
#include "external/json/filereadstream.h"
#include "external/json/writer.h"
#include "external/json/filewritestream.h"
#include "external/json/prettywriter.h"
#include "external/json/error/en.h"
#include "external/json/rapidjson.h"

using namespace rapidjson;
typedef GenericDocument< UTF8<> > Document;
typedef GenericValue< UTF8<> > Value;
typedef GenericStringStream< UTF8<> > StringStream;
typedef GenericStringBuffer< UTF8<> > StringBuffer;

// セーブデータの拡張子
#define SAVE_EXTENSION ".inos"
#define ES_EXTENSION ".json"
#define CSV_EXTENSION ".csv"

// おまけ部屋のセーブデータID
#define SPECIAL_ROOM_SAVE_ID 99

// ゲームウインドウ横幅、縦幅
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
// ゲームウインドウのサイズ比
#define WINDOW_SIZE_RATIO_MIN 0.75f
#define WINDOW_SIZE_RATIO_DEFAULT 1.f
#define WINDOW_SIZE_RATIO_MIDDLE 1.25f
#define WINDOW_SIZE_RATIO_MAX 1.5f

// 1マスの大きさ(px)
#define GRID 16

// DEBUGモードの時はdefineしておく
#define DEBUG

using namespace cocos2d;
using namespace std;

// メソッドログ
#ifdef DEBUG
#define FUNCLOG CCLOG("%s::%s", typeid(*this).name(), __FUNCTION__);
#else
#define FUNCLOG
#endif

// 画面の中心点
#define WINDOW_CENTER Point(WINDOW_WIDTH, WINDOW_HEIGHT) / 2

// セーブデータの個数
#define MAX_SAVE_COUNT 10

// 暗号化状態の判別
#define IS_ENCRYPTED (FileUtils::getInstance()->fullPathForFilename("config/IsEncrypted.json") != "")

// レイヤーのZORDER値
enum Priority
{
    KEY_CONFIG = 2000,
    TOP_COVER = 1000,
    LOADING_LAYER = 999,
    TROPHY_NOTIFICATION = 1000,
    COUNT_DOWN_DISPLAY = 900,
    STAMINA_BAR = 800,
	STORY_MESSAGE = 102,
	SYSTEM_MESSAGE = 101,
    CHARACTER_MESSAGE = 100,
    VIDEO_LAYER = 93,
    SELECT_LAYER = 92,
    BUTTON_MASHING_LAYER = 91,
    DISP_IMAGE_LAYER = 90,
    SCREEN_COVER = 80,
    MAP_NOTIFICATION = 60,
    FOCUS_LIGHT = 55,
    AMBIENT_LIGHT = 50,
    DEBUG_MASK = 40,
	MAP = 0,
};

// updateの優先度値
enum UpdatePriority
{
    FIRST = -1,
    LAST = 9999,
};

// ゲームで使うキーの種類
enum struct Key
{
    DOWN,
    RIGHT,
    LEFT,
    UP,
    MENU,
    DASH,
    ENTER,
    KEY_CONF,
    WIN_SIZE,
    
    SIZE,
};

// トリガータイプ
enum struct Trigger
{
	INIT,
	RIDE,
	SEARCH,
    AFTER_INIT,
    WILL,
    RIDE_ON,
    GET_OFF,
	SIZE,
};

// オブジェクトID
enum struct ObjectID
{
    UNDIFINED = -1,
    HERO = -2,
};

// イベントID
enum struct EventID
{
    UNDIFINED = -1,
};

// キャラクターID
enum struct CharacterID
{
    UNDIFINED = -1,
};

enum struct EnemyID
{
    UNDIFINED = -1,
};

enum struct MapID
{
    UNDIFINED = -1,
};

enum struct ItemID
{
    UNDIFINED = 0,
};

// 味方キャラクタの動き方
enum struct CharacterMovePattern
{
    NONE = 1,
    RANDOM = 2,
    
    SIZE,
};

// 敵キャラクタの動き方
enum struct EnemyMovePattern
{
    CHEAP_CHASER,
    RANDOM,
    SPEED_UP,
    PERFECT_RANDOM,
    SCOUTER,
    CHASER,
    BATTLE_MOB,
    BATTLE_BOSS,
    
    SIZE,
};

struct Location
{
    int map_id{0};
    int x {0};
    int y {0};
    Direction direction {};
    Location(int map_id, int x, int y, int direction):map_id(map_id), x(x), y(y), direction(Direction::convertInt(direction)){};
    Location(int map_id, int x, int y, Direction direction):map_id(map_id), x(x), y(y), direction(direction){};
    Location(){};
};

// enum structをint型にキャスト
#define etoi(param) static_cast<int>(param)
#define etos(param) to_string(etoi(param))

// パラメータを一つタイプを指定してcreate
#define CREATE_FUNC_WITH_PARAM(__TYPE_1__, __TYPE_2__) \
static __TYPE_1__* create(__TYPE_2__ param) \
{ \
	__TYPE_1__ *pRet = new(std::nothrow) __TYPE_1__(); \
	if (pRet && pRet->init(param)) \
	{ \
		pRet->autorelease(); \
		return pRet; \
	} \
	else \
	{ \
		delete pRet; \
		pRet = NULL; \
		return NULL; \
	} \
}

#define CREATE_FUNC_WITH_TWO_PARAM(__TYPE_1__, __TYPE_2__, __TYPE_3__) \
static __TYPE_1__* create(__TYPE_2__ param1, __TYPE_3__ param2) \
{ \
    __TYPE_1__ *pRet = new(std::nothrow) __TYPE_1__(); \
    if (pRet && pRet->init(param1, param2)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#define CREATE_FUNC_WITH_THREE_PARAM(__TYPE_1__, __TYPE_2__, __TYPE_3__, __TYPE_4__) \
static __TYPE_1__* create(__TYPE_2__ param1, __TYPE_3__ param2, __TYPE_4__ param3) \
{ \
__TYPE_1__ *pRet = new(std::nothrow) __TYPE_1__(); \
if (pRet && pRet->init(param1, param2, param3)) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

#define CREATE_FUNC_WITH_PARAM_CONSRUCT(__TYPE_1__, __TYPE_2__) \
static __TYPE_1__* create(__TYPE_2__ param) \
{ \
    __TYPE_1__ *pRet = new(std::nothrow) __TYPE_1__(param); \
    if (pRet && pRet->init()) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#endif // __DEFINE_H__
