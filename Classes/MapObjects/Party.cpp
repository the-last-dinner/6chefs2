//
//  Party.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/05.
//
//

#include "MapObjects/Party.h"

#include "MapObjects/Character.h"
#include "MapObjects/Command/WalkCommand.h"

#include "Managers/PlayerDataManager.h"

// コンストラクタ
Party::Party() {FUNCLOG};

// デストラクタ
Party::~Party()
{
    FUNCLOG

    _members.clear();
};

// 初期化
bool Party::init(const vector<CharacterData>& datas)
{
    if (datas.empty()) return false;
    
    // データを元にキャラクタを生成して格納
    for (int i { 0 }; i < datas.size(); i++) {
        Character* chara { Character::create(datas[i]) };
        
        if (!chara) continue;
        
        _members.pushBack(chara);
    }
    
    return true;
}

// パーティにキャラクタを追加
void Party::addMember(Character* character)
{
    _members.pushBack(character);
    if (character != this->getMainCharacter()) character->onJoinedParty();
    PlayerDataManager::getInstance()->getLocalData()->setPartyMember(character->getCharacterData());
}

// パーティメンバーを削除
void Party::removeMember(const int objectId)
{
    Character* targetMember { nullptr };
    for (Character* member : _members) {
        if (member->getObjectId() == objectId) targetMember = member;
    }
    if (!targetMember) return;
    
    _members.eraseObject(targetMember);
    if (targetMember != this->getMainCharacter()) targetMember->onQuittedParty();
    
    PlayerDataManager::getInstance()->getLocalData()->removePartyMember(objectId);
}

// パーティメンバー全員削除
void Party::removeMemberAll()
{
    for (Character* member : _members) {
        if (member == this->getMainCharacter()) continue;
        member->onQuittedParty();
    }
    
    _members.clear();
    PlayerDataManager::getInstance()->getLocalData()->removePartyMemberAll();
}

// 主人公を移動
void Party::moveMainCharacter(const vector<Direction>& directions, float speed, function<void(bool)> callback)
{
    WalkCommand* command { WalkCommand::create() };
    command->setDirections(directions);
    command->setSpeed(speed);
    command->setWalkCallback([this, callback](bool walked) {
        callback(walked);
        if (this->onPartyMoved) this->onPartyMoved(this->getMainCharacter()->getGridCollisionRect());
    });
    
    this->getMainCharacter()->pushCommand(command);
}

// メンバーを移動
void Party::moveMember(Character* member, Character* previousMember, float speed)
{
    // 前のメンバーの移動前にいた位置を計算
    Point destPos { previousMember->getGridPosition() };
    vector<Direction> previousMemberMovingDirections { previousMember->getMovingDirections() };
    
    for (Direction direction : previousMemberMovingDirections) {
        int fixNum = 2;
        if (direction.isVertical()) fixNum = 1;
        
        destPos -= direction.getGridVec2() * fixNum;
    }
    
    Vec2 gridMovement { destPos - member->getGridPosition() };
    
    Vec2 previousMemberMovement { Direction::getVec2(previousMemberMovingDirections) };
    Vec2 movement { MapUtils::gridVecToVec2(gridMovement) };
    
    float degree { MapUtils::radianToDegree(abs(movement.getAngle(previousMemberMovement))) };

    if (!previousMemberMovement.isZero() && degree > 90) return;
    
    WalkCommand* command { WalkCommand::create() };
    command->setDirections(Direction::convertGridVec2(gridMovement));
    command->setSpeed(speed);
    
    member->pushCommand(command);
}

// パーティを移動
void Party::move(const vector<Direction>& directions, float speed, function<void(bool)> callback)
{
    // 主人公を移動
    this->moveMainCharacter(directions, speed, callback);
    
    // メンバーを移動
    for (int i { 1 }; i < _members.size(); i++) {
        this->moveMember(_members.at(i), _members.at(i - 1), speed);
    }
}

// 主人公を取得
Character* Party::getMainCharacter() const
{
    return _members.at(0);
}

// パーティメンバーを取得
Vector<Character*> Party::getMembers() const
{
    return _members;
}

// パーティのキャラクターデータの取得
vector<CharacterData> Party::getMembersData() const
{
    vector<CharacterData> datas {};
    int member_count = _members.size();
    for (int i = 0; i < member_count; i++) {
        datas.push_back(_members.at(i)->getCharacterData());
    }
    
    return datas;
}
