//
//  CharacterMessageData.h
//  LastSupper
//
//  Created by Kohei on 2015/10/12.
//
//

#ifndef __LastSupper__CharacterMessageData__
#define __LastSupper__CharacterMessageData__

#include "Datas/Message/MessageData.h"

class CharacterMessageData : public MessageData
{
// 列挙型
public:
    enum struct Option
    {
        REACTION,
        
        SIZE
    };
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CharacterMessageData, const queue<string>&)
    static CharacterMessageData* createWithCharaId(int charaId, const queue<string>& pages, int imgId = 0, Option option = Option::SIZE);
    static CharacterMessageData* createWithCharaName(const string& charaName, const queue<string>& pages, int charaId = static_cast<int>(CharacterID::UNDIFINED), int imgId = 0, Option option = Option::SIZE);
    
// インスタンス変数
private:
    int charaId {static_cast<int>(CharacterID::UNDIFINED)};
    int imgId { 0 };
    string charaName {};
    queue<string> pages {};
    Option option {Option::SIZE};
    
// インスタンスメソッド
public:
    virtual bool hasNextPage() override;
    virtual const string getMessage() override;
    int getCharaId() const;
    int getImgId() const;
    const string& getCharaName() const;
    Option getOption() const;
    
    void setCharaId(int charaId);
    void setImgId(int imgId);
    void setCharaName(const string& charaName);
    void setOption(Option option);
    
private:
    CharacterMessageData();
    ~CharacterMessageData();
    bool init(const queue<string>& pages);
};

#endif /* defined(__LastSupper__CharacterMessageData__) */
