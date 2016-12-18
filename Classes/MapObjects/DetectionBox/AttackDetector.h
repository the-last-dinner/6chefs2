//
//  AttackDetector.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/09/19.
//
//

#ifndef AttackDetector_h
#define AttackDetector_h

#include "MapObjects/DetectionBox/AttackBox.h"
#include "MapObjects/DetectionBox/HitBox.h"

class CollisionDetector;

class AttackDetector : public Node
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(AttackDetector, CollisionDetector*);
    
// インスタンス変数
private:
    Vector<AttackBox*> _attackBoxes {};
    Vector<HitBox*> _hitBoxes {};
    CollisionDetector* _collisionDetector { nullptr };
    
// インスタンスメソッド
private:
    AttackDetector();
    virtual ~AttackDetector();
    virtual bool init(CollisionDetector* detector);
public:
    void addAttackBox(AttackBox* attackBox);
    void removeAttackBox(AttackBox* attackBox);
    void addHitBox(HitBox* hitBox);
    void removeHitBox(HitBox* hitBox);
    HitBox* getIntersectHitBox(AttackBox* attackBox);
public:
    void update(float delta) override;
};

#endif /* AttackDetector_h */
