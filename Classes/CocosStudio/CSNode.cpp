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
    if(!Node::init()) return false;
    
    // csbファイルをNodeとして生成
    Node* csbNode { CSLoader::createNode(filepath) };
    
    if(!csbNode) return false;
    
    this->addChild(csbNode);
    _csbNode = csbNode;
    
    // csbファイルからタイムラインを読み込む
    cocostudio::timeline::ActionTimeline* timeline { CSLoader::createTimeline(filepath) };

    if(!timeline) return false;
    
    csbNode->runAction(timeline);
    _timeline = timeline;
    
    return true;
}

void CSNode::play(const string& animationName, bool loop)
{
    this->play(animationName, 1.f, loop);
}

void CSNode::play(const string& animationName, float speed, bool loop)
{
    this->setPlaying(animationName, true);
    _timeline->setAnimationEndCallFunc(animationName, [this, animationName]
    {
        this->setPlaying(animationName, false);
        _timeline->setAnimationEndCallFunc(animationName, nullptr);
    });
    _timeline->setTimeSpeed(speed);
    _timeline->play(animationName, loop);
}

void CSNode::play(const string& animationName, function<void()> animationCallback)
{
    this->setPlaying(animationName, true);
    _timeline->setAnimationEndCallFunc(animationName, [this, animationName, animationCallback]
    {
        this->setPlaying(animationName, false);
        _timeline->setAnimationEndCallFunc(animationName, nullptr);
        if(animationCallback) animationCallback();
    });
    _timeline->setTimeSpeed(1.f);
    _timeline->play(animationName, false);
}

void CSNode::playIfNotPlaying(const string& animationName, float speed)
{
    if(this->isPlaying(animationName)) return;
    
    this->play(animationName, speed);
}

// 1フレームアニメーション再生用メソッド
// なぜか1フレームのアニメーションを設定すると終了した事にならないので作成
void CSNode::playSingleFrame(const string& animationName)
{
    _timeline->play(animationName, false);
}

#pragma mark -
#pragma mark Getter

bool CSNode::isPlaying() const
{
    bool playing = false;
    
    for(pair<string, bool> p : _isPlayingWithAnimationName)
    {
        if(!p.second) continue;
        
        playing = true;
        break;
    }
    
    return playing;
}

bool CSNode::isPlaying(const string& animationName) const
{
    if(_isPlayingWithAnimationName.count(animationName) == 0) return false;
    
    return _isPlayingWithAnimationName.at(animationName);
}

Node* CSNode::getCSChild(const string& name) const
{
    if(!_csbNode) return nullptr;
    
    return _csbNode->getChildByName(name);
}

#pragma mark -
#pragma mark private

void CSNode::setPlaying(const string& animationName, bool playing)
{
    for(pair<string, bool> p : _isPlayingWithAnimationName)
    {
        if(animationName != p.first) _isPlayingWithAnimationName[p.first] = false;
    }
    
    _isPlayingWithAnimationName[animationName] = playing;
}
