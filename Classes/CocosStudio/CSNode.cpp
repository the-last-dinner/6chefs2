//
//  CSNode.cpp
//  6chefs2
//
//  Created by Kohei Asami on 2016/08/20.
//
//

#include "CocosStudio/CSNode.h"

#include "cocostudio/CocoStudio.h"
#include "cocostudio/CCComExtensionData.h"
#include "ui/CocosGUI.h"

// コンストラクタ
CSNode::CSNode() { FUNCLOG };

// デストラクタ
CSNode::~CSNode() { FUNCLOG };

// 初期化
bool CSNode::init(const string& filepath)
{
    if (!Node::init()) return false;
    
    // csbファイルをNodeとして生成
    Node* csbNode { CSLoader::createNode(filepath) };
    if (!csbNode) return false;
    
    csbNode->setCascadeOpacityEnabled(true);
    this->addChild(csbNode);
    _csbNode = csbNode;
    
    // csbファイルからタイムラインを読み込む
    cocostudio::timeline::ActionTimeline* timeline { CSLoader::createTimeline(filepath) };
    if (!timeline) return false;
    
    csbNode->runAction(timeline);
    _timeline = timeline;
    
    this->setCascadeOpacityEnabled(true);
    
    return true;
}

void CSNode::play(const string& animationName, bool loop)
{
    this->play(animationName, 1.f, loop);
}

void CSNode::play(const string& animationName, function<void()> callback)
{
    this->play(animationName, 1.f, false, callback);
}

void CSNode::play(const string& animationName, float speed, bool loop, function<void()> callback)
{
    this->setPlaying(animationName, true);
    _timeline->setAnimationEndCallFunc(animationName, [this, animationName, callback] {
        this->setPlaying(animationName, false);
        _timeline->setAnimationEndCallFunc(animationName, nullptr);
        if (callback) callback();
    });
    _timeline->setTimeSpeed(speed);
    _timeline->play(animationName, loop);
}

void CSNode::playIfNotPlaying(const string& animationName, float speed)
{
    if (this->isPlaying(animationName)) return;
    this->play(animationName, speed);
}

void CSNode::pause()
{
    _csbNode->pause();
}

void CSNode::resume()
{
    _csbNode->resume();
}

#pragma mark -
#pragma mark Getter

bool CSNode::isPlaying() const
{
    return !_currentAnimationInfo.isNull();
}

bool CSNode::isPlaying(const string& animationName) const
{
    return _currentAnimationInfo.name == animationName;
}

Node* CSNode::getCSChild(const string& name) const
{
    if (!_csbNode) return nullptr;
    return _csbNode->getChildByName(name);
}

#pragma mark -
#pragma mark private

void CSNode::setPlaying(const string& animationName, bool playing)
{
    if (!playing) {
        _currentAnimationInfo = AnimationInfo();
        return;
    }
    
    cocostudio::timeline::AnimationInfo originInfo { _timeline->getAnimationInfo(animationName) };
    AnimationInfo info {};
    info.name = animationName;
    info.startIndex = originInfo.startIndex;
    info.endIndex = originInfo.endIndex;
    info.clipEndCallBack = originInfo.clipEndCallBack;
    
    _currentAnimationInfo = info;
}
