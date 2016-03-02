//
//  SystemMessageData.h
//  LastSupper
//
//  Created by Kohei Asami on 2015/10/12.
//
//

#ifndef __LastSupper__SystemMessageData__
#define __LastSupper__SystemMessageData__

#include "Datas/Message/MessageData.h"

class SystemMessageData : public MessageData
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(SystemMessageData, const string&)
    
// インスタンス変数
private:
    string message {};
    
// インスタンスメソッド
public:
    virtual const string getMessage() override;
private:
    SystemMessageData();
    ~SystemMessageData();
    bool init(const string& message);
};

#endif /* defined(__LastSupper__SystemMessageData__) */
