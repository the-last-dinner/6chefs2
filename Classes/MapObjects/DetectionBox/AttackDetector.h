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

class AttackDetector : public Node
{
// クラスメソッド
public:
    CREATE_FUNC(AttackDetector)
    
// インスタンス変数
private:
    Vector<AttackBox*> _attackBoxes {};
    Vector<HitBox*> _hitBoxes {};
    
// インスタンスメソッド
private:
    AttackDetector();
    virtual ~AttackDetector();
    virtual bool init() override;
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
