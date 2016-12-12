//
//  HurtCommand.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/12.
//
//

#ifndef HurtCommand_h
#define HurtCommand_h

#include "MapObjects/Command/MapObjectCommand.h"

class HurtCommand : public MapObjectCommand
{
// クラスメソッド
public:
    CREATE_FUNC(HurtCommand);
    
private:
    HurtCommand();
    ~HurtCommand();
    bool init() override;
    
// インターフェース
public:
    virtual bool isExecutable(MapObject* target) const override;
    virtual void execute(MapObject* target) override;
};

#endif /* HurtCommand_hpp */
