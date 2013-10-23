#ifndef Philon_cocos2dx_PompaDroid_ROBOT_H_
#define Philon_cocos2dx_PompaDroid_ROBOT_H_
#pragma once

#include "ActionSprite.h"
#include "cocos2d.h"

typedef enum {
    ROBOT_AI_STATE_WAIT = 0,
    ROBOT_AI_STATE_STAND,
    ROBOT_AI_STATE_PATROL,
    ROBOT_AI_STATE_FOLLOW,
    ROBOT_AI_STATE_ATTACK,
} RobotAIState;

#define ROBOT_VIEW_FIELD 200
#define ROBOT_ATTACK_FIELD 50

class Robot : public ActionSprite
{
public:
    Robot();
    ~Robot();

    bool init();
    CREATE_FUNC(Robot);

    void decide(cocos2d::Point target);
    void execute(cocos2d::Point target);
    std::function<void(void)> onAttack;

    CC_SYNTHESIZE(RobotAIState, _aiState, AIState);
    CC_SYNTHESIZE(cocos2d::Point, _moveDirection, MoveDirection);

private:
    cocos2d::Point _velocity;
    unsigned short _executeTime;
};

#endif // End of Philon_cocos2dx_PompaDroid_Robot_H_
