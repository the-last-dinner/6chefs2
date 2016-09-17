//
//  DetectionBox.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/06.
//
//

#ifndef DetectionBox_h
#define DetectionBox_h

#include "define.h"

class MapObject;

class DetectionBox : public Node
{
// インスタンス変数
protected:
    MapObject* _parent { nullptr };
    Node* _origin { nullptr };

// インスタンスメソッド
protected:
    DetectionBox();
    virtual ~DetectionBox();
    virtual bool init(MapObject* parent, Node* origin);
    virtual bool init(MapObject* parent, const Rect& rect);
protected:
    virtual void setOriginInfo(Node* origin);
    void setOriginInfo(const Rect& originRect);
public:
    virtual void startDetection();
    virtual void update(float delta);
public:
    Rect getGridRect(const vector<Direction>& directions = {}) const;
    bool isGridHit(DetectionBox* other, const vector<Direction>& directions = {}) const;
    bool isGridHit(const Rect& gridRect, const vector<Direction>& directions) const;
public:
    Rect getRect() const;
    Rect getRect(const Direction& direction) const;
    Rect getRect(const vector<Direction>& directions) const;
public:
    bool isBetween(const MapObject* obj1, const MapObject* obj2) const;
};

#endif /* DetectionBox_h */
