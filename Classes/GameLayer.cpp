#include "GameLayer.h"

#include "cocos2d.h"

using namespace cocos2d;

GameLayer::GameLayer()
{}

GameLayer::~GameLayer()
{}

bool GameLayer::init()
{
    bool ret = false;

    do {
        CC_BREAK_IF(!Layer::init());

        _map = TMXTiledMap::create("pd_tilemap.tmx");
        this->addChild(_map);

        // 加载精灵的动作图片序列资源
        // 注意，2.x版本中的sharedSpriteFrameCache以弃用！！！
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile("pd_sprites.plist");
        _actors = SpriteBatchNode::create("pd_sprites.pvr.ccz");
        this->addChild(_actors);

        // 初始化英雄
        _hero = Hero::create();
        _hero->setPosition(Point(80, 80));
        _hero->idle();
        _actors->addChild(_hero);

        // 初始化机器人
        _robots = Array::createWithCapacity(5);
        _robots->retain();
        for (int i = 0; i < 5; i++) {
            addRobot();
        }

        this->scheduleUpdate();
        ret = true;
    } while(0);

    return ret;
}

void GameLayer::onAttack()
{
    _hero->attack();
    Object *object = NULL;

    CCARRAY_FOREACH(_robots, object) {
        Robot *robot = (Robot*)object;

        if (_detectCollision(_hero, robot)) {
            robot->hurt();
            robot->setHP(robot->getHP() - 20);
            if (robot->getHP() <= 0) {
                // 延时是为了等待knockout
                DelayTime *delay = DelayTime::create(1);
                Blink *blink = Blink::create(1, 4);
                CallFunc *remove = CallFunc::create(std::bind(&GameLayer::removeRobot, this, robot));
                Action *die = Sequence::create(delay, blink, remove, NULL);
                robot->knockout();
                robot->runAction(die);
            }
        }
    }
}

void GameLayer::onWalk(Point direction, float distance)
{
    // 根据精灵的x向量，来判断精灵的正面“朝向”
    _hero->setFlipX(direction.x < 0 ? true : false);
    _hero->walk();
    // 根据偏移量的大小，确定速度在1档还是2档
    _heroVelocity = direction * ((distance < 96) ? 1 : 2);
}

void GameLayer::onStop()
{
    _hero->idle();
}

void GameLayer::update(float dt)
{
    Size winSize = Director::getInstance()->getWinSize();
    float mapLength = _map->getContentSize().width;

    // 如果hero处于Walk，则刷新其坐标
    if (_hero->getActionState() == ACTION_STATE_WALK) {
        Point expect = _hero->getPosition() + _heroVelocity;
        Point actual = expect;

        // 限制hero在y轴的移动范围（防止走到墙上）
        if (expect.y < 40 || expect.y > (winSize.height / 3)) {
            actual.y = _hero->getPositionY();
        }

        // 判断hero横向移动是否在视角跟随区，背景滚动（与hero移动方向相反）
        if ((expect.x > winSize.width / 2)
            && (expect.x < (mapLength - (winSize.width / 2)))) {
            this->setPositionX(this->getPositionX() - _heroVelocity.x);
        } else if (expect.x < 35 || expect.x > mapLength) {
            // 限制hero在x轴的移动范围（防止走出屏幕之外）
            actual.x = _hero->getPositionX();
        } else {

        }

        _hero->setPosition(actual);
        _hero->setZOrder(_hero->getPositionY());
    }

    updateRobots();
}

void GameLayer::updateRobots()
{
    Object *object = NULL;
    Point distance = Point::ZERO;

    if (_robots->count() < 5) {
        addRobot();
    }

    CCARRAY_FOREACH(_robots, object) {
        Robot *robot = (Robot*)object;

        robot->execute(_hero->getPosition());

        if (robot->getActionState() == ACTION_STATE_KNOCKOUT) {
            
            //_actors->removeChild(robot, true);
            _robots->removeObject(object);
            continue;
        }

        if (robot->getActionState() == ACTION_STATE_WALK) {
            Point location = robot->getPosition();
            Point direction = robot->getMoveDirection();
            Point expect = location + direction;
            if (expect.y > 110 || expect.y < 40) {
                direction.y = 0;
            }

            robot->setFlipX(direction.x < 0 ? true : false);
            robot->setPosition(location + direction);
            robot->setZOrder(robot->getPositionY());
        }
    }
}

void GameLayer::addRobot()
{
    Size winSize = Director::getInstance()->getWinSize();
    Robot *robot = Robot::create();
    Point location = Point::ZERO;

    robot->onAttack = std::bind(&GameLayer::onRobotAttack, this, robot);
    while (abs(_hero->getPositionX() - location.x) < (winSize.width / 1.5))
    {
        location.x = _hero->getPositionX() + (CCRANDOM_MINUS1_1() * winSize.width);
    }
    location.y = CCRANDOM_0_1() * MAX_POSITION_Y;
    if (location.y < MIN_POSITION_Y) {
        location.y = MIN_POSITION_Y;
    }
    robot->setPosition(location + robot->getPosition());
    robot->idle();
    _robots->addObject(robot);
    _actors->addChild(robot);
}

void GameLayer::removeRobot(ActionSprite *robot)
{
    _robots->removeObject(robot);
    _actors->removeChild(robot, true);
}

void GameLayer::onRobotAttack(ActionSprite *robot)
{
    if (_detectCollision(robot, _hero)) {
        _hero->setHP(_hero->getHP() - robot->getATK());
        if (_hero->getHP() <= 0) {
            _hero->knockout();
        } else {
            _hero->hurt();
        }
    }
}

bool GameLayer::_detectCollision(Sprite *attacker, Sprite *target)
{
    Point attackerLocation = attacker->getPosition();
    Point targetLocation = target->getPosition();
    bool isFlipX = attacker->isFlipX();
    bool onTargetLeft = (attackerLocation.x < targetLocation.x) ? true : false;
    Point offset = attackerLocation - targetLocation;

    if ((isFlipX && !onTargetLeft) || (!isFlipX && onTargetLeft)) {
        if (abs(offset.x) <= 70 && abs(offset.y) <= 35) {
            return true;
        }
    }

    return false;
}

// Philon 2013/10/23

