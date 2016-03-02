//
//  ChangeMapHistory.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/27.
//
//

#ifndef ChangeMapHistory_h
#define ChangeMapHistory_h

#include "define.h"

struct Relation
{
    Location from_location {};
    Location to_location {};
};

class ChangeMapHistory
{
// インスタンス変数
private:
    deque<Relation> relations {};
    
// インスタンスメソッド
public:
    ChangeMapHistory();
    ~ChangeMapHistory();
    
    Relation getLatestRelation() const;
    Relation getOldestRelation() const;
    
    void add(const Relation& relation);
    void deleteLatest();
    void deleteOldest();
};

#endif /* ChangeMapHistory_h */
