#include "Hero.h"

#include "cocos2d.h"
#include "ActionSprite.h"

using namespace cocos2d;

Hero::Hero()
{}

Hero::~Hero()
{}

bool Hero::init()
{
    bool ret = false;

    do {
        CC_BREAK_IF(!ActionSprite::initWithSpriteFrameName("hero_idle_00.png"));

        // 之前的CallFunc::create(this, callfunc_selector(Hero::idle))已经被废弃额
        // 改用c++11特性来回调
        CallFunc *callbackIdle = CallFunc::create(std::bind(&Hero::idle, this));

        // 创建idle（空闲）动画，调用后反复播放
        Animation *idle = createAnimation("hero_idle_%02d.png", 6, 12);
        setIdleAction(RepeatForever::create(Animate::create(idle)));

        // 创建walk（行走）动画，同样调用后反复播放
        //Animation *walk = createAnimation("hero_walk_%02d.png", 7, 24);
        Animation *walk = createAnimation("hero_walk_%02d.png", 7, 14);
        setWalkAction(RepeatForever::create(Animate::create(walk)));

        // 创建attact（攻击）动画，播放后回调到idle动画
        //Animation *attack = createAnimation("hero_attack_00_%02d.png", 3, 12);
        Animation *attack = createAnimation("hero_attack_00_%02d.png", 3, 20);
        setAttackAction(Sequence::create(Animate::create(attack), callbackIdle, NULL));

        // 创建hurt（受伤）动画，播放后回调到idle动画
        Animation *hurt = createAnimation("hero_hurt_%02d.png", 3, 15);
        setHurtAction(Sequence::create(Animate::create(hurt), callbackIdle, NULL));

        // 创建knockout（被击倒）动画，播放后不做任何调用
        Animation *knockout = createAnimation("hero_knockout_%02d.png", 5, 12);
        setKnockoutAction(Sequence::create(Animate::create(knockout), NULL));

        setATK(20); // 攻击力
        setHP(100); // 生命值

        ret = true;
    } while (0);

    return ret;
}
