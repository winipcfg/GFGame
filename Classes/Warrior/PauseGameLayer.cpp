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
#include "CCExtensions/CCSpriteHelper.h"
#include "BattleScene.h"

using namespace cocos2d;

namespace Warrior 
{

const short         kLayerPriority                  = -999;
const std::string   kLabelPauseFont                 = "Arial";
const int           kLabelPauseFontSize             = 40;

PauseGameLayer::PauseGameLayer()
{
    setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);

    SetupViewer();
    
    scheduleUpdate();
    
    // Swallow all input below
    cocos2d::CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(
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
    cocos2d::CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
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
    static_cast<BattleScene*>(this->getParent())->ResumeGame();
    CCLOG("[%s][%d] - %d", __FUNCTION__, __LINE__, this->retainCount());
    this->removeFromParentAndCleanup(false);    
}

void PauseGameLayer::SetupViewer()
{
    // Get window size and place the label upper. 
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::CCLabelTTF* pause_label = cocos2d::CCLabelTTF::labelWithString(
        "The Game is Paused", 
        kLabelPauseFont.c_str(), 
        kLabelPauseFontSize);
            
    pause_label->setPosition(ccp(size.width / 2, size.height / 2));
    addChild(pause_label);
}

void PauseGameLayer::draw(void)
{
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();

    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

    const GLfloat glVertices[] = {
        -1.0f * size.width,    1.0f * size.height,               // top left
        -1.0f * size.width,   -1.0f * size.height,               // bottom left
         1.0f * size.width,   -1.0f * size.height,               // bottom right
         1.0f * size.width,    1.0f * size.height                // top right
    };

    glVertexPointer(2, GL_FLOAT, 0, glVertices);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // restore default GL states
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

} // namespace
