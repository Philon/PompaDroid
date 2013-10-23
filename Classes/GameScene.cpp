#include "GameScene.h"

using namespace cocos2d;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

bool GameScene::init()
{
    bool ret = false;

    do {
        CC_BREAK_IF(!Scene::init());

        _gameLayer = GameLayer::create();
        _optionLayer = OptionLayer::create();

        _optionLayer->setDelegator(_gameLayer);

        this->addChild(_gameLayer, 0);
        this->addChild(_optionLayer, 1);

        ret = true;
    } while (0);

    return ret;
}
