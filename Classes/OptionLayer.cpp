#include "OptionLayer.h"

using namespace cocos2d;

OptionLayer::OptionLayer()
{}

OptionLayer::~OptionLayer()
{}

bool OptionLayer::init()
{
    bool ret = false;

    do {
        CC_BREAK_IF(!Layer::init());

        _joystick = Sprite::create("joystick.png");
        _joystick_bg = Sprite::create("joystick_bg.png");
        this->addChild(_joystick_bg);
        this->addChild(_joystick);

        _inactivityJoystick();

        setTouchEnabled(true);

        ret = true;
    } while(0);

    return ret;
}

void OptionLayer::ccTouchesBegan(Set *ts, Event *e)
{
    Size winSize = Director::getInstance()->getWinSize();
    SetIterator iter = ts->begin();

    while (iter != ts->end()) {
        Touch *t = (Touch*)(*iter);
        Point p = t->getLocation();
        // left，当触控操作的起点小于屏幕宽度的一半，说明触控发生在左屏
        if (p.x <= winSize.width / 2) {
            _activityJoystick(p);
        } else {
            // right，否则发生在右屏，会产生“攻击”信号
            _delegator->onAttack();
        }

        iter++;
    }
}

void OptionLayer::ccTouchesMoved(Set *ts, Event *e)
{
    Size winSize = Director::getInstance()->getWinSize();
    SetIterator iter = ts->begin();
    Touch *t = (Touch*)(*iter);
    Point start = t->getStartLocation();

    // 如果该触控的起点是右屏产生的，则不做“滑动”处理
    if (start.x > winSize.width / 2) {
        return;
    }

    Point p = t->getLocation();
    // 获取位移点与起始点的偏移量（直线距离）
    float distance = start.getDistance(p);
    // 获取起始点到位移点的向量（单位为1的坐标）
    Point direction = (p - start).normalize();

    _updateJoystick(direction, distance);
    _delegator->onWalk(direction, distance);
}

void OptionLayer::ccTouchesEnded(Set *ts, Event *e)
{
    if (_joystick_bg->isVisible()) {
        _inactivityJoystick();
        _delegator->onStop();
    }
}

void OptionLayer::_activityJoystick(Point position)
{
    _joystick->setPosition(position);
    _joystick_bg->setPosition(position);
    _joystick->setVisible(true);
    _joystick_bg->setVisible(true);
}

void OptionLayer::_inactivityJoystick()
{
    _joystick->setPosition(_joystick_bg->getPosition());
    _joystick->setVisible(false);
    _joystick_bg->setVisible(false);
}

void OptionLayer::_updateJoystick(Point direction, float distance)
{
    // 以“摇杆基座”的圆心为触控起点参考，“摇杆”做相应的便宜
    Point start = _joystick_bg->getPosition();

    if (distance < 32) {
        // 如果移动未超出“摇杆基座”，“摇杆”在“基座”做偏移
        _joystick->setPosition(start + (direction * distance));
    } else if (distance > 96) {
        // 如果移动超出“摇杆基座”，“摇杆”圆心始终在“基座”边缘做偏移
        _joystick->setPosition(start + (direction * 64));
    } else {
        // 如果移动超出“摇杆基座”，“摇杆”边缘始终在“基座”边缘做偏移
        _joystick->setPosition(start + (direction * 32));
    }
}
