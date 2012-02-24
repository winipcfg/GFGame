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

#include "CreditScene.h"
#include "CCExtensions/CCShake.h"

#define     kFile_BG			"Assets/UI/splash.png"
#define     kZOrder_BG          -10

CreditScene::CreditScene()
{
    setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);
    
    SetupBackground();
    SetupMenu();

    scheduleUpdate();

    cocos2d::CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(
        this, 
        0, 
        false);
}

cocos2d::CCScene* CreditScene::Scene()
{
    // 'scene' is an autorelease object
    cocos2d::CCScene* scene = cocos2d::CCScene::node();
    
    // add layer as a child to scene
    cocos2d::CCLayer* layer = new CreditScene();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}

void CreditScene::SetupMenu()
{
    
}

void CreditScene::SetupBackground()
{
    cocos2d::CCDirector* director = cocos2d::CCDirector::sharedDirector();
    cocos2d::CCSize winSize = director->getWinSize();
    
    sprite_background_ = cocos2d::CCSprite::spriteWithFile(kFile_BG);
	sprite_background_->setPosition(ccp(winSize.width / 2, winSize.height / 2));
	addChild(sprite_background_, kZOrder_BG);
}

bool CreditScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    sprite_background_->runAction(GFGame::CCShake::actionWithDuration(.45f, ccp(2,2), 12));
    return true;
}
