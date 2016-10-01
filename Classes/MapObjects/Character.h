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
class CSNode;

class Character : public MapObject
{
// 定数
private:
    static const string CS_SPRITE_NODE_NAME;
    static const string CS_COLLISION_NODE_NAME;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Character, const CharacterData&);
    
// インスタンス変数
private:
    int _charaId { static_cast<int>(CharacterID::UNDIFINED) };                   // キャラクタID
protected:
    MovePattern* _movePattern { nullptr };                                       // 動きのパターン
    CSNode* _csNode { nullptr };
    
// インスタンスメソッド
public:
	Character();
	virtual ~Character();
    bool init(const CharacterData& data);
    
    int getCharacterId() const;
    CharacterData getCharacterData() const;
    
	virtual void setDirection(const Direction& direction) override;
    virtual void setDirection(const Direction& direction, bool stopAnimation);
	void setMoving(bool _isMoving);
    void pauseAi();
    void resumeAi();
    bool walkBy(const vector<Direction>& directions, function<void(bool)> cb, float speed, bool back, bool ignoreCollision);
    void lookAround(function<void()> callback);
    void lookAround(function<void()> callback, Direction direction);
    
    // CSNode
    void playAnimation(const string& name, float speed, bool loop = false);
    void playAnimationIfNotPlaying(const string& name, float speed = 1.f);
    
    // TerrainState
    void stamp(const Direction direction, const float ratio = 1.0f);
    bool isRunnable() const;
    bool consumeStaminaWalking() const;
    float getStaminaConsumptionRatio() const;
    
    // Interface
    virtual void onEnterMap() override;
    virtual void onExitMap() override;
    virtual void onJoinedParty();
    virtual void onQuittedParty();
    virtual void onPartyMoved();
    virtual void onSearched(MapObject* mainChara) override;
    virtual void onEventStart() override;
    virtual void onEventFinished() override;
    
    friend class TerrainObject;
    friend class TerrainState;
    
public:
    class AnimationName
    {
    public:
        static string getTurn(const Direction& direction);
        static string getWalk(const Direction& direction);
        static string getSwim(const Direction& direction);
    };
};

#endif // __CHARACTER_H__
