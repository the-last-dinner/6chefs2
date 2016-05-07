//
//  Party.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/05.
//
//

#ifndef __LastSupper__Party__
#define __LastSupper__Party__

#include "Common.h"

#include "Datas/MapObject/CharacterData.h"

class Character;

class Party : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Party, const vector<CharacterData>&)

// インスタンス変数
private:
    Vector<Character*> members {};
public:
    function<void(const Rect&)> onPartyMoved { nullptr };

// インスタンスメソッド
private:
    Party();
    ~Party();
    bool init(const vector<CharacterData>& datas);
    bool moveMainCharacter(const vector<Direction>& directions, float ratio, function<void()> callback);
    void moveMember(Character* member, Character* previousMember, float ratio);
public:
    void addMember(Character* character);
    void removeMember(const int obj_id);
    void removeMemberAll();
    bool move(const vector<Direction>& directions, float ratio, function<void()> callback);
    Character* getMainCharacter() const;
    Vector<Character*> getMembers() const;
    vector<CharacterData> getMembersData() const;
};

#endif /* defined(__LastSupper__Party__) */
