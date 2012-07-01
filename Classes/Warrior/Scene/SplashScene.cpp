//Copyright (C) 2011 by Gavin Fong
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#include "SplashScene.h"
#include "SceneHelper.h"
#include "CCExtensions/CCShake.h"

const std::string   kFile_BG			= "Assets/UI/splash.png";
const std::string   kButtonNewGame		= "Assets/UI/button_newgame.png";
const std::string   kButtonContinue		= "Assets/UI/button_cont.png";

const short         kZOrderBackground   = -10;
const short         kZOrderMenu         = 1;

using namespace cocos2d;

SplashScene::SplashScene()
    : sprite_background_(NULL)
{
    setTouchEnabled(true);
    setAccelerometerEnabled(true);
    
    SetupBackground();
    SetupMenu();

    this->scheduleUpdate();

    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this, 
        0, 
        false);
}

SplashScene::~SplashScene()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

cocos2d::CCScene* SplashScene::Scene()
{
    // 'scene' is an autorelease object
    cocos2d::CCScene* scene = cocos2d::CCScene::create();
    
    // add layer as a child to scene
    cocos2d::CCLayer* layer = new SplashScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void SplashScene::SetupBackground()
{
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    cocos2d::CCSize winSize = director->getWinSize();
    
    sprite_background_ = cocos2d::CCSprite::create(kFile_BG.c_str());
	sprite_background_->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(sprite_background_, kZOrderBackground, kZOrderBackground);
}

void SplashScene::SetupMenu()
{
    cocos2d::CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    cocos2d::CCMenuItemImage* menuItem;
    menuItem = cocos2d::CCMenuItemImage::create(
        kButtonNewGame.c_str(), 
        kButtonNewGame.c_str(), 
        this, 
        menu_selector(SplashScene::ButtonNewGameCallback));
    menuItem->setPosition(CCPointMake(winSize.width * 0.5, winSize.height * 0.2));

    cocos2d::CCMenu* pMenu = CCMenu::create(menuItem, NULL);
    pMenu->setPosition(CCPointZero);    
    addChild(pMenu, kZOrderMenu, kZOrderMenu);
}

bool SplashScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    sprite_background_->runAction(GFGame::CCShake::create(0.45f, ccp(5, 5), 12));    
    return true;
}

void SplashScene::ButtonNewGameCallback(CCObject * pSender)
{    
    Warrior::SceneHelper manager;
    manager.GoBattleScene(1);
}
