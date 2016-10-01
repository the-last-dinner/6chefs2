//
//  Sight.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#ifndef Sight_h
#define Sight_h

#include "define.h"

class Character;
class MapObject;
class MapObjectList;

class Sight : public Ref
{
// 定数
    static const float DEFAULT_VIEW_ANGLE;
    static const int DEFAULT_GRID_LIMIT_DISTANCE;

// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(Sight, Character*)
    
// インスタンス変数
private:
    Character* _chara { nullptr };
    float _angle { DEFAULT_VIEW_ANGLE };
    int _limitDistance { DEFAULT_GRID_LIMIT_DISTANCE };
    
// インスタンスメソッド
private:
    Sight();
    ~Sight();
    bool init(Character* chara);
public:
    bool isIn(const MapObject* target, const MapObjectList* list) const;
};

#endif /* Sight_h */
