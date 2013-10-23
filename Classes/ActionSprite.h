#ifndef Philon_cocos2dx_PompaDroid_ACTIONSPRITE_H_
#define Philon_cocos2dx_PompaDroid_ACTIONSPRITE_H_
#pragma once

#include "cocos2d.h"

// 根据pd_sprites.plist得到，动作精灵有五种状态
typedef enum {
    ACTION_STATE_NONE = 0,
    ACTION_STATE_IDLE,
    ACTION_STATE_WALK,
    ACTION_STATE_ATTACK,
    ACTION_STATE_HURT,
    ACTION_STATE_KNOCKOUT,
} ActionState;

class ActionSprite : public cocos2d::Sprite
{
public:
    ActionSprite();
    ~ActionSprite();

    void idle();
    void walk();
    void attack();
    void hurt();
    void knockout();

    // 定义每个状态动作的get/set方法
    CC_SYNTHESIZE_RETAIN(cocos2d::Action*, _idleAction, IdleAction);
    CC_SYNTHESIZE_RETAIN(cocos2d::Action*, _attackAction, AttackAction);
    CC_SYNTHESIZE_RETAIN(cocos2d::Action*, _hurtAction, HurtAction);
    CC_SYNTHESIZE_RETAIN(cocos2d::Action*, _knockoutAction, KnockoutAction);
    CC_SYNTHESIZE_RETAIN(cocos2d::Action*, _walkAction, WalkAction);

    // 精灵的当前状态
    CC_SYNTHESIZE(ActionState, _currentState, ActionState);

    CC_SYNTHESIZE(unsigned int, _hp, HP); // 生命值
    CC_SYNTHESIZE(unsigned int, _atk, ATK); // 攻击力

protected:
    // 定义一个创建状态动画的方法
    // fmt   - 状态的图片名格式(查看pd_sprites.plist,每种状态格式都类似hero_idle_xx.png)
    // count - 状态图片序列的数量
    // fps   - 动画的播放帧率
    static cocos2d::Animation *createAnimation(const char *fmt, int count, float fps);

private:
    // 切换演员的当前状态
    bool _changeState(ActionState state);
};

#endif // End of Philon_cocos2dx_PompaDroid_ACTIONSPRITE_H_
