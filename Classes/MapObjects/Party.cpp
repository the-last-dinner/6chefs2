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

    this->members.clear();
};

// 初期化
bool Party::init(const vector<CharacterData>& datas)
{
    if(datas.empty()) return false;
    
    // データを元にキャラクタを生成して格納
    for(int i { 0 }; i < datas.size(); i++)
    {
        Character* chara {Character::create(datas[i])};
        
        // 主人公のみ当たり判定
        if(i == 0) chara->setHit(true);
        
        this->members.pushBack(chara);
    }
    
    return true;
}

// パーティにキャラクタを追加
void Party::addMember(Character* character)
{
    character->setHit(false);
    this->members.pushBack(character);
    PlayerDataManager::getInstance()->getLocalData()->setPartyMember(character->getCharacterData());
}

// パーティメンバーを削除
void Party::removeMember(const int obj_id)
{
    int member_count = this->members.size();
    Vector<Character*> temp_members = this->members;
    this->members.clear();
    for (int i = 0; i < member_count; i++)
    {
        int target_obj = temp_members.at(i)->getCharacterData().obj_id;
        if (obj_id == target_obj)
        {
            PlayerDataManager::getInstance()->getLocalData()->removePartyMember(target_obj);
        }
        else
        {
            this->members.pushBack(temp_members.at(i));
        }
    }
}

// 主人公を移動
bool Party::moveMainCharacter(const vector<Direction>& directions, float ratio, function<void()> callback)
{
    return this->getMainCharacter()->walkBy(directions, [this, callback]
    {
        callback();
        if(this->onPartyMoved) this->onPartyMoved(this->getMainCharacter()->getGridRect());
    }, ratio);
}

// メンバーを移動
void Party::moveMember(Character* member, Character* previousMember, float ratio)
{
    // 前のメンバーの後ろに移動するようにする
    Direction backDirection { MapUtils::oppositeDirection(previousMember->getDirection()) };
    
    Point destPos {previousMember->getGridPosition()};
    
    switch (backDirection)
    {
        case Direction::FRONT:
        case Direction::BACK:
            destPos += MapUtils::directionsToMapVector({backDirection}) * previousMember->getGridSize().height;
            break;
            
        case Direction::LEFT:
            destPos += MapUtils::directionsToMapVector({backDirection}) * (member->getGridSize().width);
            break;
            
        case Direction::RIGHT:
            destPos += MapUtils::directionsToMapVector({backDirection}) * (previousMember->getGridSize().width);
            break;
            
        default:
            break;
    }
    
    Vec2 movement { destPos - member->getGridPosition() };
    member->walkBy(MapUtils::vectoMapDirections(movement), nullptr, ratio);
}

// パーティを移動
bool Party::move(const vector<Direction>& directions, float ratio, function<void()> callback)
{
    // 主人公を移動
    if(!this->moveMainCharacter(directions, ratio, callback)) return false;
    
    // メンバーを移動
    for(int i { 1 }; i < this->members.size(); i++)
    {
        this->moveMember(this->members.at(i), this->members.at(i - 1), ratio);
    }
    
    return true;
}

// 主人公を取得
Character* Party::getMainCharacter() const
{
    return this->members.at(0);
}

// パーティメンバーを取得
Vector<Character*> Party::getMembers() const
{
    return this->members;
}

// パーティのキャラクターデータの取得
vector<CharacterData> Party::getMembersData() const
{
    vector<CharacterData> datas {};
    int member_count = this->members.size();
    for (int i = 0; i < member_count; i++)
    {
        datas.push_back(this->members.at(i)->getCharacterData());
    }
    return datas;
}
