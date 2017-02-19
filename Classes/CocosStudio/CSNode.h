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
private:
    struct AnimationInfo
    {
        std::string name;
        int startIndex;
        int endIndex;
        std::function<void()> clipEndCallBack;
        bool isNull() const { return name == ""; }
    };
    
// クラスメソッド
public:
    CREATE_FUNC_WITH_PARAM(CSNode, const string&)
    
// インスタンス変数
private:
    Node* _csbNode { nullptr };
    cocostudio::timeline::ActionTimeline* _timeline { nullptr };
    AnimationInfo _currentAnimationInfo {};
    
// インスタンスメソッド
private:
    CSNode();
    virtual ~CSNode();
    virtual bool init(const string& filepath);
public:
    void play(const string& animationName, bool loop = false);
    void play(const string& animationName, function<void()> callback);
    void play(const string& animationName, float speed, bool loop = false, function<void()> callback = nullptr);
    
    void playIfNotPlaying(const string& animationName, float speed = 1.f);
    
    void resume();
    void pause();
public:
    bool isPlaying() const;
    bool isPlaying(const string& animationName) const;
    Node* getCSChild(const string& name) const;
private:
    void setPlaying(const string& animationName, bool playing);
    
};

#endif /* CSNode_h */
