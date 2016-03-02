//
//  MovePatternFactory.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/20.
//
//

#ifndef MovePatternFactory_h
#define MovePatternFactory_h

#include "Common.h"

class Character;
class MovePattern;

class MovePatternFactory : public Ref
{
// クラスメソッド
public:
    CREATE_FUNC(MovePatternFactory);
    
// インスタンスメソッド
private:
    MovePatternFactory();
    ~MovePatternFactory();
    bool init();
public:
    MovePattern* createMovePattern(const EnemyMovePattern type, Character* character);
    MovePattern* createMovePattern(const CharacterMovePattern type, Character* character);
};

#endif /* MovePatternFactory_h */
