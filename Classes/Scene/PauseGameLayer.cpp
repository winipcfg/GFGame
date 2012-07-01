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

#include "PauseGameLayer.h"
#include <string>
#include <GFort/Core/StringHelper.h>
#include <Scene/GameScene.h>
#include "CCExtensions/CCSpriteHelper.h"

using namespace cocos2d;

namespace GFGame { namespace Scene  
{

const short         kLayerPriority                  = -999;
const std::string   kLabelPauseFont                 = "Arial";
const int           kLabelPauseFontSize             = 40;

PauseGameLayer::PauseGameLayer()
{
    setTouchEnabled(true);
    setAccelerometerEnabled(true);

    SetupViewer();
    
    scheduleUpdate();
    
    // Swallow all input below
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(
        this, 
        kLayerPriority, 
        true);
}

PauseGameLayer::~PauseGameLayer()
{    
}

void PauseGameLayer::onExit()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLOG("[%s][%d] - %d", __FUNCTION__, __LINE__, this->retainCount());
}

bool PauseGameLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{   
    return true;
}

void PauseGameLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{    
}

void PauseGameLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCLOG("[%s][%d] - The game is resumed", __FUNCTION__, __LINE__);
    static_cast<GameScene*>(this->getParent())->ResumeGame();
    CCLOG("[%s][%d] - %d", __FUNCTION__, __LINE__, this->retainCount());
    this->removeFromParentAndCleanup(false);    
}

void PauseGameLayer::SetupViewer()
{
    // Get window size and place the label upper. 
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::CCLabelTTF* pause_label = cocos2d::CCLabelTTF::create(
        "The Game is Paused", 
        kLabelPauseFont.c_str(), 
        kLabelPauseFontSize);
            
    pause_label->setPosition(ccp(size.width / 2, size.height / 2));
    addChild(pause_label);
}

void PauseGameLayer::draw(void)
{
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    const CCPoint glPoint[] = {
        ccp(-1.0f * size.width,    1.0f * size.height),               // top left
        ccp(-1.0f * size.width,   -1.0f * size.height),               // bottom left
        ccp( 1.0f * size.width,   -1.0f * size.height),               // bottom right
        ccp( 1.0f * size.width,    1.0f * size.height)                // top right
    };
    ccDrawSolidPoly(glPoint, 4, ccc4f(0, 0, 0, 0.3f));
}

} } // namespace
