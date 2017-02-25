//
//  EventFactory.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/24.
//
//

#include "Event/EventFactory.h"

#include "Event/BattleEvent.h"
#include "Event/CameraEvent.h"
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
#include "Event/EventScriptValidator.h"

#include "Managers/DungeonSceneManager.h"


// イベントtypeからイベントクラスへのマップ
const map<string, function<GameEvent*(rapidjson::Value&)>> EventFactory::_typeToCreateFunc = {
    // 制御系
    {"sequence", EventSequence::create},      // 順番に処理を実行
    {"spawn", EventSpawn::create},            // 同時に処理を実行
    {"if", EventIf::create},                  // 場合分け処理
    {"callEvent", CallEvent::create},         // 別イベントの呼び出し
    {"repeat", EventRepeat::create},          // 繰り返し処理を実行
    {"stopEvent", EventStop::create},         // 処理を中止
    
    // 音系
    {"playBGM", PlayBGMEvent::create},           // BGM再生
    {"stopBGM", StopBGMEvent::create},           // BGM停止
    {"changeVolume", ChangeVolumeEvent::create}, // BGMの音量変更
    {"playSE", PlaySEEvent::create},             // 効果音再生
    {"playVoice", PlayVoiceEvent::create},       // ボイス再生
    
    // モーダルレイヤ系
    {"charaMsg", CharacterMessage::create},         // キャラメッセージ
    {"storyMsg", StoryMessage::create},             // ストーリーメッセージ
    {"systemMsg", SystemMessage::create},           // システムのメッセージ
    {"displayImg", DispImageEvent::create},         // 画像表示
    {"displaySaveMenu", DisplaySaveMenu::create},   // セーブメニュー表示
    {"playVideo", PlayVideoEvent::create},          // 動画再生
    
    // シーン系
    {"changeMap", ChangeMapEvent::create},          // マップ移動
    {"wait", WaitEvent::create},                    // 時間待機
    {"fadeout", FadeOutEvent::create},              // フェードアウト
    {"fadein", FadeInEvent::create},                // フェードイン
    {"gameover", GameOverEvent::create},            // ゲームオーバー
    {"ending", EndingEvent::create},                // エンディング
    {"backToTitle", BackToTitleEvent::create},      // タイトルへ戻る
    {"infoAssert", InfoAssertEvent::create},        // デバッグ用にAssertを出す
    
    // カメラ系
    {"createCamera", CreateCameraEvent::create},    // カメラ生成
    {"camera", MoveCameraEvent::create},            // カメラ移動
    {"setCamera", SetCameraTargetEvent::create},    // カメラターゲット設定
    
    // マップ系
    {"removeLayer", HideLayerEvent::create},        // マップの指定レイヤを非表示
    {"createLayer", ShowLayerEvent::create},        // マップの指定レイヤを表示
    {"swingLayer", SwingLayerEvent::create},        // マップの指定レイヤを揺らす
    {"stopSwing", StopLayerActionEvent::create},    // マップレイヤのアクションを止める
    {"quakeMap", QuakeMapEvent::create},            // マップを振動させる
    
    // マップオブジェクト系
    {"createObject", CreateMapObjectEvent::create},      // マップオブジェクトを無効リストから有効リストへ移動、マップ上に表示
    {"removeObject", RemoveMapObjectEvent::create},      // マップオブジェクトを削除（有効リストのもののみ対象）
    {"reaction", ReactionEvent::create},                // リアクション
    {"chase", FollowCharacterEvent::create},            // パーティーの追加
    {"releaseChase", ReleaseFollowingCharacterEvent::create},   // パーティの解除
    {"charaWarp", WarpMapObjectEvent::create},          // マップオブジェクトを再配置
    {"charaMoveTo", MoveToEvent::create},               // マップオブジェクトを指定座標へ移動
    {"charaMove", MoveByEvent::create},                 // マップオブジェクトを方向、格子数指定で移動
    {"setLight", SetLightEvent::create},                // オブジェクトに光をセット
    {"removeLight", RemoveLightEvent::create},          // オブジェクトから光を消す
    {"setMovable", SetMovableEvent::create},            // オブジェクトに動かせるかどうかをセット
    {"objAnimate", MapObjectAnimateEvent::create},      // 登録されているアニメーションを再生
    
    // キャラクター系
    {"changeDirection", ChangeDirectionEvent::create},  // 方向転換
    {"charaWalkTo", WalkToEvent::create},               // 指定座標へ歩行
    {"charaWalk", WalkByEvent::create},                 // 方向、歩数をして歩行
    {"changeHero", ChangeHeroEvent::create},            // 主人公を変更
    {"changeSpeed", ChangeSpeedEvent::create},          // 移動スピードを変更
    
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
    
    // 戦闘系
    {"battle", BattleStartEvent::create},           // 戦闘開始
    
    // フラグ系
    {"already", NeverAgainEvent::create},                   // イベントIDを見たことにし、二度と発動しないようにする
    {"getItem", GetItemEvent::create},                      // アイテムを入手
    {"removeItem", RemoveItemEvent::create},                // アイテムを破棄
    {"giveItem", GiveItemEvent::create},                    // アイテムの受け渡し
    {"addProfile", AddProfileEvent::create},                // プロフィールに情報を追加する
    {"changeChapter", ChangeChapterEvent::create},          // チャプターを切り替える
    {"changeStatus", ChangeLikabilityRatingEvent::create},  // キャラクタの好感度を変更
    {"changeFlg", ChangeEventStatusEvent::create},          // イベントステータス(固有フラグ)を変更
    {"setGameClear", SetGameClearEvent::create},            // ゲームクリア回数をインクリメント
    {"getTrophy", GetTrophyEvent::create},                  // トロフィーを取得
};

// 初期化
bool EventFactory::init()
{
    return true;
}

// ゲームイベントを生成して返す
GameEvent* EventFactory::createGameEvent(rapidjson::Value& json, GameEvent* caller)
{
    GameEvent* event {nullptr};
    
    if (!json.IsObject() || !json.HasMember(member::TYPE)) {
        // イベントタイプがない場合はspawnでイベント生成
        event = EventSpawn::create(json);
    } else {
        // イベントタイプがある場合はタイプからイベント生成
        string typeName {json[member::TYPE].GetString()};
        
        /**
         * NOTICE: 
         *  EventScriptValidatorはconfig/EventScriptValidator.jsonを元にチェックしているが、
         *  こっちはシステム上のGameEventがあるかどうかのチェックを行なっている。
         **/
        if (EventFactory::_typeToCreateFunc.count(typeName) == 0) {
            CCLOG("Undifined EventScript Type : %s", typeName.c_str());
            LastSupper::AssertUtils::warningAssert("EventScriptError\n" + typeName + "なんてイベントはないずら〜");
            return nullptr;
        }
        
        // デバッグモード時のみEventScriptのバリデーション
        if (ConfigDataManager::getInstance()->getDebugConfigData()->isDebugMode()) {
            int mapId = DungeonSceneManager::getInstance()->getLocation().map_id;
            string mapFileName = CsvDataManager::getInstance()->getMapData()->getFileName(mapId);
            // @FIXME: callerがない場合eventIDを引数でもらいたい
            string eventId = to_string((caller) ?
                    caller->getEventId() : etoi(DungeonSceneManager::getInstance()->getRunningEventId()));
            EventScriptValidator::create(mapFileName, eventId)->validate(json);
        }
        
        event = EventFactory::_typeToCreateFunc.at(typeName)(json);
    }
    
    // 親イベントとEventIDのセット
    if (event && caller) {
        event->setCaller(caller);
    }
    
    return event;
}
