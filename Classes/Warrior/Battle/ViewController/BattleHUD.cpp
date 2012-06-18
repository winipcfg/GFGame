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

#include "BattleHUD.h"
#include <string>
#include <GFort/Core/StringHelper.h>
#include "../BattleScene.h"
#include "CCExtensions/CCSpriteHelper.h"
#include "../../ViewController/GameSettingLayer.h"
#include "BattleEndLayer.h"

using namespace cocos2d;

namespace Warrior 
{

const std::string   kHealthBarSprite                = "Assets/UI/healthbar.png";
const std::string   kHealthBarSpriteOverlay         = "Assets/UI/healthbar_overlay.png";
const short         kHealthBarX                     = 20;
const short         kHealthBarY                     = 20;

const std::string   kLabelComboFont                 = "Arial";
const int           kLabelComboFontSize             = 20;
const float         kLabelComboScaleSize            = 1.5f;
const float         kLabelComboScaleDuration        = 0.2f;

const std::string   kLabelTotalKillsSprite          = "Assets/UI/label_kill_bg.png";
const short         kLabelTotalKillsSizeY           = 32;
const std::string   kLabelTotalKillsFont            = "Arial";
const int           kLabelTotalKillsFontSize        = 40;

const std::string   kButtonPause		            = "Assets/UI/button_pause.png";
const short         kButtonPausePosition            = 20;
const std::string   kButtonDebug		            = "Assets/UI/button_debug.png";
const short         kZOrderMenu                     = 1;

BattleHUD::BattleHUD()
    : Cistron::Component("HUD"),
      health_(0),
      combo_(0),
      total_kills_background_(NULL),
      total_kills_(0),
      label_combo_(NULL),
      label_total_kills_(NULL),
      health_bar_background_(NULL),
      health_bar_(NULL),
      button_pause_(NULL),
      button_debug_(NULL)
{
    setIsTouchEnabled(true);
    setIsAccelerometerEnabled(true);

    SetupViewer();
}

BattleHUD::~BattleHUD()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

void BattleHUD::SetHealth(const float& value)
{
    health_bar_->setPercentage(value);
}

void BattleHUD::SetCombo(const short& value)
{
    if (combo_ != value)
    {
        combo_ = value;
        if (label_combo_)
        {
            std::string s = GFort::Core::StringHelper::ToString(combo_);
            label_combo_->setString(s.c_str());
        }
    }

    if (label_combo_)
    {
        cocos2d::CCScaleTo* scaleToAction = cocos2d::CCScaleTo::actionWithDuration(kLabelComboScaleDuration, kLabelComboScaleSize, kLabelComboScaleSize);
        cocos2d::CCScaleTo* scaleBackAction = cocos2d::CCScaleTo::actionWithDuration(kLabelComboScaleDuration, 1, 1);
        cocos2d::CCFiniteTimeAction* seqAction = cocos2d::CCSequence::actions(scaleToAction, scaleBackAction, NULL);    
        label_combo_->runAction(seqAction);
    }
}

void BattleHUD::SetTotalKills(const int& value)
{
    if (total_kills_ != value)
    {
        total_kills_ = value;
        if (label_total_kills_)
        {
            std::string s = GFort::Core::StringHelper::ToString(total_kills_);
            label_total_kills_->setString(s.c_str());

            cocos2d::CCScaleTo* scaleToAction = cocos2d::CCScaleTo::actionWithDuration(kLabelComboScaleDuration, kLabelComboScaleSize, kLabelComboScaleSize);
            cocos2d::CCScaleTo* scaleBackAction = cocos2d::CCScaleTo::actionWithDuration(kLabelComboScaleDuration, 1, 1);
            cocos2d::CCFiniteTimeAction* seqAction = cocos2d::CCSequence::actions(scaleToAction, scaleBackAction, NULL);    
            label_total_kills_->runAction(seqAction);
        }
    }
}

void BattleHUD::SetupViewer()
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

    // Combo Label
    label_combo_ = cocos2d::CCLabelTTF::labelWithString(
        "0", 
        kLabelComboFont.c_str(), 
        kLabelComboFontSize);
            
    label_combo_->setPosition(ccp(size.width - kLabelComboFontSize, kLabelComboFontSize));
    addChild(label_combo_);

    // Total Kills Label
    total_kills_background_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kLabelTotalKillsSprite.c_str());
    total_kills_background_->setPosition(ccp(size.width / 2, size.height - total_kills_background_->boundingBox().size.height));
    addChild(total_kills_background_);

    label_total_kills_ = cocos2d::CCLabelTTF::labelWithString(
        "0", 
        kLabelTotalKillsFont.c_str(), 
        kLabelTotalKillsFontSize);
        
    label_total_kills_->setPosition(ccp(size.width / 2, size.height - total_kills_background_->boundingBox().size.height));
    addChild(label_total_kills_);

    //---------------------------------------------------------------
    // Game Menu
    //---------------------------------------------------------------
    button_pause_ = cocos2d::CCMenuItemImage::itemWithNormalImage(
        kButtonPause.c_str(), 
        kButtonPause.c_str(), 
        this, 
        menu_selector(BattleHUD::ButtonPauseCallback));
    button_pause_->setAnchorPoint(ccp(1.0, 1.0));
    button_pause_->setPosition(CCPointMake(size.width - kButtonPausePosition, size.height - kButtonPausePosition));

    button_debug_ = cocos2d::CCMenuItemImage::itemWithNormalImage(
        kButtonDebug.c_str(), 
        kButtonDebug.c_str(), 
        this, 
        menu_selector(BattleHUD::ButtonToggleDebugCallback));
    button_debug_->setAnchorPoint(ccp(1.0, 1.0));
    button_debug_->setPosition(CCPointMake(size.width - kButtonPausePosition * 3, size.height - kButtonPausePosition));

    cocos2d::CCMenu* pMenu = CCMenu::menuWithItems(button_pause_, button_debug_, NULL);
    pMenu->setPosition(CCPointZero);    
    addChild(pMenu, kZOrderMenu, kZOrderMenu);
}

void BattleHUD::ButtonPauseCallback(CCObject* pSender)
{
    BattleScene* parent = static_cast<BattleScene*>(this->getParent());
    if (!parent->GamePaused())
    {
        CCLOG("[%s][%d] - The game changes to Paused state", __FUNCTION__, __LINE__);
        parent->PauseGame();

        //GameSettingLayer* layer = GameSettingLayer::node();
        //this->addChild(layer);
    }
    //else
    //{
    //    CCLOG("[%s][%d] - The game resumes to Normal state", __FUNCTION__, __LINE__);
    //    parent->ResumeGame();
    //}
}

void BattleHUD::ButtonToggleDebugCallback(CCObject* pSender)
{
    BattleScene* parent = static_cast<BattleScene*>(this->getParent());
    parent->TogglePhysicsDebugViewer();

    //BattleEndLayer* layer = BattleEndLayer::node();
    //this->addChild(layer);
}

} // namespace
