//
//  CSNode.h
//  6chefs2
//
//  Created by Kohei Asami on 2016/08/20.
//
//

#ifndef CSNode_h
#define CSNode_h

#include "define.h"

namespace cocostudio
{
    namespace timeline
    {
        class ActionTimeline;
    }
}

class CSNode : public Node
{    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CSNode, const string&)
    
// インスタンス変数
private:
    Node* _csbNode { nullptr };
    cocostudio::timeline::ActionTimeline* _timeline { nullptr };
    map<string, bool> _isPlayingWithAnimationName {};
    
// インスタンスメソッド
private:
    CSNode();
    virtual ~CSNode();
    virtual bool init(const string& filepath);
public:
    void play(const string& animationName, bool loop = false);
    void play(const string& animationName, float speed, bool loop = false);
    void play(const string& animationName, function<void()> animationCallback);
    
    void playIfNotPlaying(const string& animationName, float speed = 1.f);
public:
    bool isPlaying() const;
    bool isPlaying(const string& animationName) const;
    Node* getCSChild(const string& name) const;
private:
    void setPlaying(const string& animationName, bool playing);
    
};

#endif /* CSNode_h */
