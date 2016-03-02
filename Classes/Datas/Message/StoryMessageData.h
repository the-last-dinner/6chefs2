//
//  StoryMessageData.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#ifndef __LastSupper__StoryMessageData__
#define __LastSupper__StoryMessageData__

#include "Datas/Message/MessageData.h"

class StoryMessageData : public MessageData
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(StoryMessageData, const string&)
    
// インスタンス変数
private:
    string message {};
    
// インスタンスメソッド
public:
    const string getMessage() override;
private:
    StoryMessageData();
    ~StoryMessageData();
    bool init(const string& message);
};

#endif /* defined(__LastSupper__StoryMessageData__) */
