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
    static Vector<WalkCommand*> create(const vector<Direction>& directions, int gridNum, function<void(bool)> cb, float speed, bool back, bool ignoreCollision = false);
    static Vector<WalkCommand*> create(const deque<Direction>& directions, function<void(bool)> cb, float speed, bool back, bool ignoreCollision = false);
    
// インスタンス変数
private:
    vector<Direction> _directions {};
    function<void(bool)> _callback { nullptr };
    float _speed { 1.f };
    bool _back { false };
    bool _ignoreCollision { false };
    
// インスタンスメソッド
private:
    WalkCommand();
    virtual ~WalkCommand();
    virtual bool init() override;

public:
    void setDirection(const Direction& direction);
    void setDirections(const vector<Direction>& directions);
    void setWalkCallback(function<void(bool)> callback);
    void setSpeed(const float speed);
    void setBack(const bool back);
    void setIgnoreCollision(const bool ignore);
    
// インターフェース
public:
    bool isExecutable(MapObject* target) const override;
    void execute(MapObject* target) override;
};

#endif /* WalkCommand_h */
