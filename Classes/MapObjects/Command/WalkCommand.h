//
//  WalkCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/24.
//
//

#ifndef WalkCommand_h
#define WalkCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class WalkCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(WalkCommand)
    
// インスタンス変数
private:
    vector<Direction> _directions {};
    function<void()> _onWalked { nullptr };
    float _speed { 1.f };
    bool _back { false };
    
// インスタンスメソッド
private:
    WalkCommand();
    virtual ~WalkCommand();
    virtual bool init() override;

public:
    void setDirection(const Direction& direction);
    void setDirections(const vector<Direction>& directions);
    void setWalkCallback(function<void()> callback);
    void setSpeed(const float speed);
    void setBack(const bool back);
    
// インターフェース
public:
    void execute(MapObject* target, function<void()> callback) override;
};

#endif /* WalkCommand_h */
