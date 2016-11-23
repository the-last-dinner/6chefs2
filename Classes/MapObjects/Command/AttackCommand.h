//
//  AttackCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/11/20.
//
//

#ifndef AttackCommand_h
#define AttackCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class Character;

class AttackCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(AttackCommand);
    
// インスタンス変数
private:
    string _name {};
    function<void(Character*)> _callback { nullptr };
    
// インスタンスメソッド
public:
    void setName(const string& name);
    void setCallback(function<void(Character*)> callback);
    
private:
    AttackCommand();
    ~AttackCommand();
    virtual bool init() override;
    
// インターフェース
public:
    virtual bool isExecutable(MapObject* target) const override;
    virtual void execute(MapObject* target) override;
    
// コールバック
public:
    void onAttackAnimationFinished(Character* character);
};

#endif /* AttackCommand_h */
