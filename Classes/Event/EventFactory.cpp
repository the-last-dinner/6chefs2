//
//  EventFactory.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/24.
//
//

#include "Event/EventFactory.h"

#include "Event/CharacterEvent.h"
#include "Event/EnemyEvent.h"
#include "Event/FlagEvent.h"
#include "Event/GameEvent.h"
#include "Event/EffectEvent.h"
#include "Event/MapEvent.h"
#include "Event/MapObjectEvent.h"
#include "Event/MiniGameEvent.h"
#include "Event/ModalLayerEvent.h"
#include "Event/SoundEvent.h"
#include "Event/SceneEvent.h"

#include "Event/EventScriptMember.h"

// コンストラクタ
EventFactory::EventFactory() {FUNCLOG};

// デストラクタ
EventFactory::~EventFactory() {FUNCLOG};

// 初期化
bool EventFactory::init()
{
    return true;
}

// ゲームイベントを生成して返す
GameEvent* EventFactory::createGameEvent(rapidjson::Value& json)
{
    map<string, function<GameEvent*(rapidjson::Value&)>> pCreateFuncs
    {
        // 制御系
        {"sequence", EventSequence::create},    // 順番に処理を実行
        {"spawn", EventSpawn::create},          // 同時に処理を実行
        {"if", EventIf::create},                // 場合分け処理
        {"callEvent", CallEvent::create},       // 別イベントの呼び出し
        {"repeat", EventRepeat::create},        // 繰り返し処理を実行
        
        // 音系
        {"playBGM", PlayBGMEvent::create},           // BGM再生
        {"stopBGM", StopBGMEvent::create},           // BGM停止
        {"changeVolume", ChangeVolumeEvent::create}, // BGMの音量変更
        {"playSE", PlaySEEvent::create},             // 効果音再生
        
        // モーダルレイヤ系
        {"charaMsg", CharacterMessage::create},         // キャラメッセージ
        {"storyMsg", StoryMessage::create},             // ストーリーメッセージ
        {"systemMsg", SystemMessage::create},           // システムのメッセージ
        {"displayImg", DispImageEvent::create},         // 画像表示
        
        // シーン系
        {"changeMap", ChangeMapEvent::create},          // マップ移動
        {"createCamera", CreateCameraEvent::create},    // カメラ生成
        {"camera", MoveCameraEvent::create},            // カメラ移動
        {"wait", WaitEvent::create},                    // 時間待機
        {"fadeout", FadeOutEvent::create},              // フェードアウト
        {"fadein", FadeInEvent::create},                // フェードイン
        {"gameover", GameOverEvent::create},            // ゲームオーバー
        {"ending", EndingEvent::create},                // エンディング
        {"backToTitle", BackToTitleEvent::create},      // タイトルへ戻る
        
        // マップ系
        {"removeLayer", HideLayerEvent::create},        // マップの指定レイヤを非表示
        {"createLayer", ShowLayerEvent::create},        // マップの指定レイヤを表示
        {"swingLayer", SwingLayerEvent::create},        // マップの指定レイヤを揺らす
        {"stopSwing", StopLayerActionEvent::create},    // マップレイヤのアクションを止める
        {"quakeMap", QuakeMapEvent::create},            // マップを振動させる
        
        // マップオブジェクト系
        {"createChara", CreateMapObjectEvent::create},      // マップオブジェクトを無効リストから有効リストへ移動、マップ上に表示
        {"removeChara", RemoveMapObjectEvent::create},      // マップオブジェクトを削除（有効リストのもののみ対象）
        {"reaction", ReactionEvent::create},                // リアクション
        {"chase", FollowCharacterEvent::create},            // パーティーの追加
        {"releaseChase", ReleaseFollowingCharacterEvent::create},   // パーティの解除
        {"charaWarp", WarpMapObjectEvent::create},          // マップオブジェクトを再配置
        {"charaMoveTo", MoveToEvent::create},               // マップオブジェクトを指定座標へ移動
        {"charaMove", MoveByEvent::create},                 // マップオブジェクトを方向、格子数指定で移動
        {"setLight", SetLightEvent::create},                // オブジェクトに光をセット
        {"removeLight", RemoveLightEvent::create},          // オブジェクトから光を消す
        
        // キャラクター系
        {"changeDirection", ChangeDirectionEvent::create},  // 方向転換
        {"charaWalkTo", WalkToEvent::create},               // 指定座標へ歩行
        {"charaWalk", WalkByEvent::create},                 // 方向、歩数をして歩行
        {"changeHero", ChangeHeroEvent::create},            // 主人公を変更
    
        // 敵系
        {"createEnemy", CreateEnemyEvent::create},          // 敵生成
        {"removeEnemy", RemoveEnemyEvent::create},          // 敵削除
        
        // エフェクト
        {"setAmbient", SetAmbientLightEvent::create},       // 環境光設定
        {"animation", AnimationEvent::create},              // アニメーション
        {"createFog", CreateFogEvent::create},              // 靄を発生させる
        {"createRain", CreateRainEvent::create},            // 雨を降らせる
        {"createUnderwater", CreateUnderwaterEvent::create},// 水中
        
        // ミニゲーム系
        {"barrage", ButtonMashingEvent::create},        // 連打イベント
        {"questionMsg", SelectEvent::create},           // 選択肢イベント
        {"password", PasswordEvent::create},            // パスワードイベント
        {"countDown", CountDownEvent::create},          // カウントダウン
        {"stopCount", StopCountEvent::create},          // ストップカウント
        
        // フラグ系
        {"already", NeverAgainEvent::create},                   // イベントIDを見たことにし、二度と発動しないようにする
        {"getItem", GetItemEvent::create},                      // アイテムを入手
        {"removeItem", RemoveItemEvent::create},                // アイテムを破棄
        {"addProfile", AddProfileEvent::create},                // プロフィールに情報を追加する
        {"changeChapter", ChangeChapterEvent::create},          // チャプターを切り替える
        {"changeStatus", ChangeLikabilityRatingEvent::create},  // キャラクタの好感度を変更
        {"changeFlg", ChangeEventStatusEvent::create},          // イベントステータス(固有フラグ)を変更

        {"getTrophy", GetTrophyEvent::create},                  // トロフィーを取得
    };
    
    // イベントタイプがなければ同時実行を生成して返す
    if(!json.IsObject() || !json.HasMember(member::TYPE)) return EventSpawn::create(json);
    
    // イベントタイプ名取得
    string typeName {json[member::TYPE].GetString()};
    
    if(pCreateFuncs.count(typeName) == 0)
    {
        CCLOG("Undifined EventScript Type : %s", typeName.c_str());
        return nullptr;
    }
    
    return pCreateFuncs.at(typeName)(json);
}

// イベントベクタを生成
Vector<GameEvent*> EventFactory::createEventVector(rapidjson::Value& json)
{
    Vector<GameEvent*> events {};
    
    rapidjson::Value& eventJson {(json.IsObject() && json.HasMember(member::ACTION))?json[member::ACTION]:json};
    
    for(int i { 0 }; i < eventJson.Size(); i++)
    {
        if(GameEvent* event { this->createGameEvent(eventJson[i]) })
        {
            events.pushBack(event);
        }
    }
    
    return events;
}

// イベントキューを生成
queue<GameEvent*> EventFactory::createEventQueue(rapidjson::Value& json)
{
    queue<GameEvent*> events {};
    
    rapidjson::Value& eventJson {(json.IsObject() && json.HasMember(member::ACTION))?json[member::ACTION]:json};
    
    for (int i { 0 }; i < eventJson.Size(); i++)
    {
        if(GameEvent* event { this->createGameEvent(eventJson[i]) })
        {
            CC_SAFE_RETAIN(event);
            events.push(event);
        }
    }
    
    return events;
}