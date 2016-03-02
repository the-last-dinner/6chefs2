//
//  SelectEventLayer.hpp
//  LastSupper
//
//  Created by Kohei Asami on 2015/11/15.
//
//

#ifndef SelectEventLayer_hpp
#define SelectEventLayer_hpp

#include "Common.h"

class MiniSelector;

class SelectEventLayer : public Layer
{
// クラスメソッド
public:
    CREATE_FUNC_WITH_TWO_PARAM(SelectEventLayer, const string&, const vector<string>&)

// インスタンス変数
public:
    MiniSelector* mini { nullptr };
    function<void(const int)> onSelected { nullptr };
    
// インスタンスメソッド
private:
    SelectEventLayer();
    ~SelectEventLayer();
    bool init(const string& message, const vector<string>& choices);
    void onChoiceSelected(const int idx);
};

#endif /* SelectEventLayer_hpp */
