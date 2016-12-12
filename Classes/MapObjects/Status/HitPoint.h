//
//  HitPoint.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/10/02.
//
//

#ifndef HitPoint_h
#define HitPoint_h

#include "define.h"

class HitPoint : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(HitPoint, int, function<void()>)
    
// インスタンス変数
private:
    int _max { 0 };
    int _current { 0 };
    function<void()> _onLost { nullptr };
    
// インスタンスメソッド
private:
    HitPoint();
    virtual ~HitPoint();
    bool init(int max, function<void()> onLost);
public:
    // HPを減少させる
    void reduce(int damage);
    void setMax(int max);
    bool isLost() const;
    int getCurrent() const;
};

#endif /* HitPoint_h */
