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
class HitPoint;
class AttackBox;
class HitBox;
class Sight;
class BattleCharacterData;

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
    MovePattern* _movePattern { nullptr };
    AttackBox* _battleAttackBox { nullptr };
    Sight* _sight { nullptr };
    bool _isInAttackMotion { nullptr };
    float _speed { 1.f };
    BattleCharacterData* _battleData { nullptr };
    function<void(MapObject*)> _onAttackHitted  { nullptr };
    
// インスタンスメソッド
public:
	Character();
	virtual ~Character();
    bool init(const CharacterData& data);
    
    int getCharacterId() const;
    CharacterData getCharacterData() const;
    BattleCharacterData* getBattleCharacterData() const;
    AttackBox* getBattleAttackBox() const;
    
	virtual void setDirection(const Direction& direction) override;
    virtual void setDirection(const Direction& direction, bool stopAnimation);
    void pauseAi();
    void resumeAi();
    bool walkBy(const vector<Direction>& directions, function<void(bool)> cb, float speed, bool back, bool ignoreCollision);
    void lookAround(function<void()> callback);
    void lookAround(function<void()> callback, Direction direction);
    
    // TerrainState
    void stamp(const Direction direction, const float ratio = 1.0f);
    bool isRunnable() const;
    bool consumeStaminaWalking() const;
    float getStaminaConsumptionRatio() const;
    
    // Battle
    void beInAttackMotion(bool isInAttackMotion);
    bool isInAttackMotion() const;
    void setAttackHitCallback(function<void(MapObject*)> callback);
    void onAttackHitted(MapObject* hittedObject);
    void onHurt(int damage);
    bool canAttack(MapObject* target) const override;
    
    // AttackBox
    void enableBattleAttack(bool enableAttack);
    
    // Sight
    bool isInSight(MapObject* mapObject);
    
    void setSpeed(const float& speed);

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
    virtual void onBattleStart(Battle* battle) override;
    virtual void onBattleFinished() override;
    virtual void onLostHP() override;
    
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
