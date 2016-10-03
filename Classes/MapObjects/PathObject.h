//
//  PathObject.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/12/09.
//
//

#ifndef PathObject_h
#define PathObject_h

#include "MapObjects/MapObject.h"

class PathObject : public MapObject
{
// クラスメソッド
public:
    CREATE_FUNC(PathObject)
    
// インスタンス変数
private:
    int pathId { -1 };
    int previousId { -1 };
    int nextId { -1 };
    bool through { false };
    float speedRatio { 1.f };
    Direction direction { Direction::NONE };
    
// インスタンスメソッド
private:
    PathObject();
    ~PathObject();
    bool init();
public:
    void setPathId(int pathId);
    void setPreviousId(int previousId);
    void setNextId(int nextId);
    void setThrough(bool through);
    void setSpeedRatio(float speedRatio);
    void setLookDirection(const Direction& direction);
    
    int getPathId() const;
    int getPreviousId() const;
    int getNextId() const;
    bool needsLookingAround() const;
    float getSpeedRatio() const;
    Direction getLookDirection() const;
};

#endif /* PathObject_h */
