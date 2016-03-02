//
//  CheapChaser.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#ifndef CheapChaser_h
#define CheapChaser_h

#include "MapObjects/MovePatterns/MovePattern.h"

class CheapChaser : public MovePattern
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CheapChaser, Character*)

// インスタンスメソッド
private:
    CheapChaser();
    ~CheapChaser();
    virtual bool init(Character* character) override;
public:
    virtual void start() override;
    virtual void onPartyMoved() override;
    virtual bool canGoToNextMap() const override;
    virtual float calcSummonDelay() const override;
    void move(function<void()> callback = nullptr);
};

#endif /* CheapChaser_h */
