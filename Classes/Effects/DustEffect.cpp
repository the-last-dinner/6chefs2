//
//  DustEffect.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/12/21.
//
//

#include "Effects/DustEffect.h"

#include "CocosStudio/CSNode.h"

// 定数
const string DustEffect::CSB_FILE_PATH { "effect/dust.csb" };
const string DustEffect::ANIMATION_NAME { "animation" };

// コンストラクタ
DustEffect::DustEffect() { FUNCLOG }

// デストラクタ
DustEffect::~DustEffect() { FUNCLOG }

// 初期化
bool DustEffect::init()
{
    if (!Node::init()) return false;
    
    CSNode* csNode { CSNode::create(CSB_FILE_PATH) };
    if (!csNode) return false;
    this->addChild(csNode);
    _csNode = csNode;
    
    return true;
}

// アニメーション開始
void DustEffect::animate(function<void(DustEffect*)> callback)
{
    _csNode->play(ANIMATION_NAME, [this, callback]{ if (callback) callback(this); });
}
