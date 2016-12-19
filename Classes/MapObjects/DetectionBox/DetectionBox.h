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
    MapObject* getMOParent() const;
    bool isEnabled() const;
public:
    virtual void update(float delta);
public:
    Rect getGridRect(const vector<Direction>& directions = {}) const;
    Rect getGridRect(const Point& parentGridPosition) const;
    bool intersectsGrid(DetectionBox* other, const vector<Direction>& directions = {}) const;
    bool intersectsGrid(DetectionBox* other, const Point& gridPosition) const;
    bool intersectsGridForPath(DetectionBox* other, const Point& gridPosition) const;
public:
    Rect getRect() const;
    Rect getRect(const Direction& direction) const;
    Rect getRect(const vector<Direction>& directions) const;
public:
    bool isBetween(const DetectionBox* other1, const DetectionBox* other2) const;
public:
    virtual bool intersects(const DetectionBox* other) const;
    
// デバッグ
private:
    void drawDebugMask();
    virtual Color4F getDebugMaskColor() const;
};

#endif /* DetectionBox_h */
