//
//  Party.cpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/05.
//
//

#include "MapObjects/Party.h"
#include "MapObjects/Character.h"

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
    if(datas.empty()) return false;
    
    // データを元にキャラクタを生成して格納
    for(int i { 0 }; i < datas.size(); i++)
    {
        Character* chara { Character::create(datas[i]) };
        
        if(!chara) continue;
        
        _members.pushBack(chara);
    }
    
    return true;
}

// パーティにキャラクタを追加
void Party::addMember(Character* character)
{
    _members.pushBack(character);
    if(character != this->getMainCharacter())
    {
        character->onJoinedParty();
    }
    PlayerDataManager::getInstance()->getLocalData()->setPartyMember(character->getCharacterData());
}

// パーティメンバーを削除
void Party::removeMember(const int objectId)
{
    Character* targetMember { nullptr };
    for(Character* member : _members)
    {
        if(member->getObjectId() == objectId) targetMember = member;
    }
    if(!targetMember) return;
    
    _members.eraseObject(targetMember);
    if(targetMember != this->getMainCharacter())
    {
        targetMember->onQuittedParty();
    }
    
    PlayerDataManager::getInstance()->getLocalData()->removePartyMember(objectId);
}

// パーティメンバー全員削除
void Party::removeMemberAll()
{
    for(Character* member : _members)
    {
        if(member == this->getMainCharacter()) continue;
        member->onQuittedParty();
    }
    
    _members.clear();
    PlayerDataManager::getInstance()->getLocalData()->removePartyMemberAll();
}

// 主人公を移動
bool Party::moveMainCharacter(const vector<Direction>& directions, float ratio, function<void()> callback)
{
    return this->getMainCharacter()->walkBy(directions, [this, callback]
    {
        callback();
        if(this->onPartyMoved) this->onPartyMoved(this->getMainCharacter()->getGridCollisionRect());
    }, ratio);
}

// メンバーを移動
void Party::moveMember(Character* member, Character* previousMember, float ratio)
{
    // 前のメンバーの移動前にいた位置を計算
    Point destPos { previousMember->getGridPosition() };
    
    for(Direction direction : previousMember->getMovingDirections())
    {
        destPos -= direction.getGridVec2() * 2;
    }
    
    Vec2 movement { destPos - member->getGridPosition() };
    member->walkBy(Direction::convertGridVec2(movement), nullptr, ratio);
}

// パーティを移動
bool Party::move(const vector<Direction>& directions, float ratio, function<void()> callback)
{
    // 主人公を移動
    if(!this->moveMainCharacter(directions, ratio, callback)) return false;
    
    // メンバーを移動
    for(int i { 1 }; i < _members.size(); i++)
    {
        this->moveMember(_members.at(i), _members.at(i - 1), ratio);
    }
    
    return true;
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
    for (int i = 0; i < member_count; i++)
    {
        datas.push_back(_members.at(i)->getCharacterData());
    }
    return datas;
}
