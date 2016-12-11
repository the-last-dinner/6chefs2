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
class HitPoint;
class AttackBox;
class HitBox;
class Sight;

class Character : public MapObject
{
// 定数
protected:
    static const string CS_SPRITE_NODE_NAME;
    static const string CS_COLLISION_NODE_NAME;
    static const string CS_HIT_NODE_NAME;
    static const string CS_BATTLE_ATTACK_NODE_NAME;
    static const string CS_ATTACK_NODE_NAME;
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Character, const CharacterData&);
    
// インスタンス変数
private:
    int _charaId { static_cast<int>(CharacterID::UNDIFINED) };
protected:
    function<void(Character*)> _onLostHP { nullptr };
    MovePattern* _movePattern { nullptr };
    CSNode* _csNode { nullptr };
    AttackBox* _battleAttackBox { nullptr };
    HitBox* _hitBox { nullptr };
    Sight* _sight { nullptr };
    bool _isInAttackMotion { nullptr };
    
// インスタンスメソッド
public:
	Character();
	virtual ~Character();
    bool init(const CharacterData& data);
    
    int getCharacterId() const;
    CharacterData getCharacterData() const;
    
	virtual void setDirection(const Direction& direction) override;
    virtual void setDirection(const Direction& direction, bool stopAnimation);
    void pauseAi();
    void resumeAi();
    bool walkBy(const vector<Direction>& directions, function<void(bool)> cb, float speed, bool back, bool ignoreCollision);
    void lookAround(function<void()> callback);
    void lookAround(function<void()> callback, Direction direction);
    
    // CSNode
    bool isAnimationPlaying() const;
    void playAnimation(const string& name, float speed, bool loop = false);
    void playAnimationIfNotPlaying(const string& name, float speed = 1.f);
    void playAnimation(const string& name, function<void(Character*)> callback);
    
    // TerrainState
    void stamp(const Direction direction, const float ratio = 1.0f);
    bool isRunnable() const;
    bool consumeStaminaWalking() const;
    float getStaminaConsumptionRatio() const;
    
    // Battle
    void beInAttackMotion(bool isInAttackMotion);
    bool isInAttackMotion() const;
    void onMyAttackHitted(MapObject* hittedObject);
    void onAttackHitted(int damage);
    
    // HP
    void setLostHPCallback(function<void(Character*)> callback);
    void onLostHP();
    
    // Sight
    bool isInSight(MapObject* mapObject);

public:
    // Interface
    virtual void update(float delta) override;
    virtual void onEnterMap() override;
    virtual void onExitMap() override;
    virtual void onJoinedParty();
    virtual void onQuittedParty();
    virtual void onPartyMoved();
    virtual void onSearched(MapObject* mainChara) override;
    virtual void onEventStart() override;
    virtual void onEventFinished() override;
    virtual void onBattleStart() override;
    virtual void onBattleFinished() override;
    
    friend class TerrainObject;
    friend class TerrainState;
    
public:
    class AnimationName
    {
    public:
        static string getTurn(const Direction& direction);
        static string getWalk(const Direction& direction);
        static string getSwim(const Direction& direction);
        static string getAttack(const string& name, const Direction& direction);
    };
};

#endif // __CHARACTER_H__
