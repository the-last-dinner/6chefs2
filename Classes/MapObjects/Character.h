//
//  Character.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/06/15.
//
//

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "MapObjects/MapObject.h"

struct CharacterData;
class MovePattern;

class Character : public MapObject
{
// 定数
private:
    static const string basePath;

// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Character, const CharacterData&);
    
// インスタンス変数
private:
    int charaId { static_cast<int>(CharacterID::UNDIFINED) };                   // キャラクタID
    string texturePrefix {};                                                    // キャラプロパティリストファイル名の先頭部分
    int stampingState {0};                                                      // 歩行アニメーションの状態
protected:
    MovePattern* movePattern { nullptr };                                       // 動きのパターン
    
// インスタンスメソッド
public:
	Character();
	virtual ~Character();
    bool init(const CharacterData& data);
    
    int getCharacterId() const;
    CharacterData getCharacterData() const;
    
	virtual void setDirection(const Direction direction) override;
	void setMoving(bool _isMoving);
    void stamp(const Direction direction, const float ratio = 1.0f);
    bool walkBy(const Direction& direction, function<void()> onWalked, const float ratio = 1.0f, const bool back = false);
    bool walkBy(const vector<Direction>& directions, function<void()> onWalked, const float ratio = 1.0f, const bool back = false);
    void walkBy(const Direction& direction, const int gridNum, function<void(bool)> callback, const float ratio = 1.0f, const bool back = false);
    void walkBy(const vector<Direction>& directions, const int gridNum, function<void(bool)> callback, const float ratio = 1.0f, const bool back = false);
    void walkByQueue(deque<Direction> directionQueue, function<void(bool)> callback, const float ratio = 1.0f, const bool back = false);
    void walkByQueue(deque<vector<Direction>> directionsQueue, function<void(bool)> callback, const float ratio = 1.0f, const bool back = false);
    
    void lookAround(function<void()> callback, Direction direction = Direction::SIZE);
    
    virtual void onEnterMap() override;
    virtual void onPartyMoved();
    virtual void onSearched(MapObject* mainChara) override;
    virtual void onEventStart() override;
    virtual void onEventFinished() override;
    
    friend class TerrainObject;
};

#endif // __CHARACTER_H__
