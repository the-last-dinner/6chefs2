//
//  AttackData.hpp
//  6chefs2
//
//  Created by Ryoya Ino on 2016/12/20.
//
//

#ifndef AttackData_h
#define AttackData_h

#include "define.h"

struct AttackData : public Ref
{
private:
    string name;

public:
    int power { 50 };
    float stamina { 5.0f };
    float intervalTime { 0.5f };
    string motionSound {};
    float motionSoundVolume { 1.0f };
    string hitSound {};
    float hitSoundVolume { 1.0f };

private:
    AttackData(){};
    ~AttackData(){};
    
public:
    CREATE_FUNC_WITH_PARAM(AttackData, const string&);
    bool init(const string& attackName)
    {
        name = attackName;
        return true;
    }
};

#endif /* AttackData_hpp */
