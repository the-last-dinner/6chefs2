//
//  Direction.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/10.
//
//

#ifndef __LastSupper__Direction__
#define __LastSupper__Direction__

enum struct Key;

class DirectionUp;
class DirectionDown;
class DirectionRight;
class DirectionLeft;

using namespace cocos2d;
using namespace std;

// 方向基底クラス
class Direction
{
// クラス定数
public:
    static const Direction NONE;
    static const Direction UP;
    static const Direction DOWN;
    static const Direction RIGHT;
    static const Direction LEFT;
private:
    static const map<Key, Direction> KEY_TO_DIRECTION;
    static const map<string, Direction> STR_TO_DIRECTION;
    static const map<int, Direction> INT_TO_DIRECTION;
    static const map<int, Direction> INT_TO_OPPOSITE_DIRECTION;
    
// クラスメソッド
public:
    static Direction convertKey(const Key& key);
    static vector<Direction> convertKeys(const vector<Key>& keys);
    static Direction convertString(const string& directionStr);
    static Direction convertInt(const int directionInt);
    static Direction convertVec2(const Vec2& vec2);
    static vector<Direction> convertGridVec2(const Vec2& vec2);
    
    static Vec2 getVec2(const vector<Direction>& directions);
    static Vec2 getGridVec2(const vector<Direction>& directions);
    
    static vector<Direction> getAll();
    
// インスタンス変数
protected:
    bool _isNull { true };
    int _int { -1 };
    float _angle { 0 };
    string _upcaseString {};
    string _downcaseString {};
    Vec2 _unitVec2 { cocos2d::Vec2(0, 0) };
    Vec2 _gridVec2 { cocos2d::Vec2(0, 0) };
    int _degree { 0 };
    bool _vertical { false };
    
// インスタンスメソッド
public:
    Direction();
    virtual ~Direction();
    
    bool isNull() const;
    int getInt() const;
    float getAngle() const;
    string getUpcaseString() const;
    string getDowncaseString() const;
    Vec2 getUnitVec2() const;
    Vec2 getVec2() const;
    Vec2 getGridVec2() const;
    Direction getOppositeDirection() const;
    Direction convertToWorldDirection(const Direction& direction) const;
    bool isVertical() const;
    
protected:
    int getDegree() const;
};

class DirectionUp : public Direction
{
public:
    DirectionUp();
};

class DirectionDown : public Direction
{
public:
    DirectionDown();
};

class DirectionRight : public Direction
{
public:
    DirectionRight();
};

class DirectionLeft : public Direction
{
public:
    DirectionLeft();
};

#endif /* defined(__LastSupper__Direction__) */
