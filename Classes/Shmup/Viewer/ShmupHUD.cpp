//Copyright (C) 2012 by Gavin Fong
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

#include "ShmupHUD.h"
#include <string>
#include <GFort/Core/StringHelper.h>
#include "CCExtensions/CCSpriteHelper.h"
#include "../Scene/ShmupScene.h"
//#include "../Scene/GameSettingLayer.h"

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

const std::string   kHealthBarSprite                = "Assets/UI/healthbar.png";
const std::string   kHealthBarSpriteOverlay         = "Assets/UI/healthbar_overlay.png";
const short         kHealthBarX                     = 20;
const short         kHealthBarY                     = 20;

const std::string   kButtonPause		            = "Assets/UI/button_pause.png";
const short         kButtonPausePosition            = 20;
const std::string   kButtonDebug		            = "Assets/UI/button_debug.png";
const short         kZOrderMenu                     = 1;

ShmupHUD::ShmupHUD()
    : Cistron::Component("HUD"),
      health_(0),
      health_bar_background_(NULL),
      health_bar_(NULL),
      button_pause_(NULL),
      button_debug_(NULL)
{
    setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);

    SetupViewer();
}

ShmupHUD::~ShmupHUD()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

void ShmupHUD::SetHealth(const float& value)
{
    health_bar_->setPercentage(value);
}

void ShmupHUD::SetupViewer()
{
    // Get window size and place the label upper. 
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();

    // Health Bar
    health_bar_background_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kHealthBarSprite.c_str());
    health_bar_background_->setAnchorPoint(ccp(0.0, 1.0));
    health_bar_background_->setPosition(ccp(kHealthBarX, size.height - kHealthBarY));
    addChild(health_bar_background_);

    health_bar_ = cocos2d::CCProgressTimer::progressWithSprite(
        GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kHealthBarSpriteOverlay.c_str()));
    health_bar_->setType(kCCProgressTimerTypeBar);    
    health_bar_->setAnchorPoint(ccp(0.0, 1.0));
    health_bar_->setPosition(ccp(kHealthBarX, size.height - kHealthBarY));
    health_bar_->setPercentage(100);
    addChild(health_bar_);
        
    //---------------------------------------------------------------
    // Game Menu
    //---------------------------------------------------------------
    // Pause Button
    button_pause_ = cocos2d::CCMenuItemImage::itemWithNormalImage(
        kButtonPause.c_str(), 
        kButtonPause.c_str(), 
        this, 
        menu_selector(ShmupHUD::ButtonPauseCallback));
    button_pause_->setAnchorPoint(ccp(1.0, 1.0));
    button_pause_->setPosition(CCPointMake(size.width - kButtonPausePosition, size.height - kButtonPausePosition));

    // Debug Button
    button_debug_ = cocos2d::CCMenuItemImage::itemWithNormalImage(
        kButtonDebug.c_str(), 
        kButtonDebug.c_str(), 
        this, 
        menu_selector(ShmupHUD::ButtonToggleDebugCallback));
    button_debug_->setAnchorPoint(ccp(1.0, 1.0));
    button_debug_->setPosition(CCPointMake(size.width - kButtonPausePosition * 3, size.height - kButtonPausePosition));

    cocos2d::CCMenu* pMenu = CCMenu::menuWithItems(button_pause_, button_debug_, NULL);
    pMenu->setPosition(CCPointZero);    
    addChild(pMenu, kZOrderMenu, kZOrderMenu);
}

void ShmupHUD::ButtonPauseCallback(CCObject* pSender)
{
    ShmupScene* parent = static_cast<ShmupScene*>(this->getParent());
    if (!parent->GamePaused())
    {
        CCLOG("[%s][%d] - The game changes to Paused state", __FUNCTION__, __LINE__);
        parent->PauseGame();
    }
}

void ShmupHUD::ButtonToggleDebugCallback(CCObject* pSender)
{
    ShmupScene* parent = static_cast<ShmupScene*>(this->getParent());
    parent->TogglePhysicsDebugViewer();
}

} } } // namespace
