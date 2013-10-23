#ifndef Philon_cocos2dx_PompaDroid_GAMELAYER_H_
#define Philon_cocos2dx_PompaDroid_GAMELAYER_H_
#pragma once

#include "cocos2d.h"
#include "Robot.h"
#include "Hero.h"
#include "OptionLayer.h"

#define MAX_POSITION_X 3800
#define MIN_POSITION_X 0
#define MAX_POSITION_Y 110
#define MIN_POSITION_Y 40

class GameLayer : public cocos2d::Layer, public OptionDelegate
{
public:
    GameLayer();
    ~GameLayer();

    virtual bool init();
    CREATE_FUNC(GameLayer);
    
    void update (float dt);

    // 实现OptionDelegate的几个方法
    void onWalk(cocos2d::Point direction, float distance);
    void onAttack();
    void onStop();
    void onRobotAttack(ActionSprite *robot);
    void addRobot();
    void removeRobot(ActionSprite *robot);
    void updateRobots();

private:
    bool _detectCollision(cocos2d::Sprite *attacker, cocos2d::Sprite *target);
    ActionSprite *_hero;
    cocos2d::Point _heroVelocity; //hero移动时的速度
    cocos2d::Array *_robots;
    cocos2d::SpriteBatchNode *_actors;
    cocos2d::TMXTiledMap *_map;
};

#endif // End of Philon_cocos2dx_PompaDroid_GAMELAYER_H_

// Philon
// 2013-10-17
