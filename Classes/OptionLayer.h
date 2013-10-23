#ifndef Philon_cocos2dx_PompaDroid_OPTIONLAYER_H_
#define Philon_cocos2dx_PompaDroid_OPTIONLAYER_H_
#pragma once

#include "cocos2d.h"

class OptionDelegate
{
public:
    // 移动，direction为向量，distance是与起点的直线距离
    virtual void onWalk(cocos2d::Point direction, float distance) = 0;
    // 攻击
    virtual void onAttack() = 0;
    // 停止移动
    virtual void onStop() = 0;
};

class OptionLayer : public cocos2d::Layer
{
public:
    OptionLayer();
    ~OptionLayer();

    virtual bool init();
    CREATE_FUNC(OptionLayer);

    // 触控的三个事件函数重载
    void ccTouchesBegan(cocos2d::Set *ts, cocos2d::Event *e);
    void ccTouchesMoved(cocos2d::Set *ts, cocos2d::Event *e);
    void ccTouchesEnded(cocos2d::Set *ts, cocos2d::Event *e);

    // 委托者
    CC_SYNTHESIZE(OptionDelegate*, _delegator, Delegator);

private:
    // 摇杆，分为“摇杆”、“摇杆基座”两个部分
    cocos2d::Sprite *_joystick;
    cocos2d::Sprite *_joystick_bg;
    // 激活“摇杆精灵”，并更新其坐标
    void _activityJoystick(cocos2d::Point position);
    // 隐藏“摇杆精灵”，并将“摇杆”置于“摇杆基座”中心
    void _inactivityJoystick();
    // 刷新“摇杆”相对于“摇杆基座”的位置（根据触控手势）
    void _updateJoystick(cocos2d::Point direction, float distance);
};

#endif // End of Philon_cocos2dx_PompaDroid_GAMELAYER_H_
