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

#include "SkirmishHUD.h"
#include <string>
#include <GFort/Core/StringHelper.h>
#include "../Battle/BattleScene.h"
#include "CCExtensions/CCSpriteHelper.h"
#include "../ViewController/GameSettingLayer.h"
#include "../Card3d.h"

using namespace cocos2d;

namespace Warrior 
{

const float         kLabelComboScaleSize            = 1.5f;
const float         kLabelComboScaleDuration        = 0.2f;

const std::string   kLabelTitle                     = "Assets/Test/banner.png";
const short         kLabelTitleSizeY                = 32;
const std::string   kLabelTitleFont                 = "Arial";
const int           kLabelTitleFontSize             = 40;

const std::string   kButtonPause		            = "Assets/UI/button_pause.png";
const short         kButtonPausePosition            = 20;
const std::string   kButtonDebug		            = "Assets/UI/button_debug.png";
const short         kZOrderMenu                     = 1;

SkirmishHUD::SkirmishHUD()
    : Cistron::Component("HUD")
    , label_stage_background_(NULL)
    , stage_(0)
    , label_stage_(NULL)
    , button_pause_(NULL)
    , button_debug_(NULL)
    , title_("")
{
    setTouchEnabled(true);
    setAccelerometerEnabled(true);

    SetupViewer();
}

SkirmishHUD::~SkirmishHUD()
{
    this->unscheduleUpdate();
    this->unscheduleAllSelectors();
}

void SkirmishHUD::SetTitle(const int& value)
{
    if (stage_ != value)
    {
        stage_ = value;
        if (label_stage_)
        {
            std::string s = GFort::Core::StringHelper::ToString(stage_);
            label_stage_->setString(s.c_str());

            cocos2d::CCScaleTo* scaleToAction = cocos2d::CCScaleTo::create(kLabelComboScaleDuration, kLabelComboScaleSize, kLabelComboScaleSize);
            cocos2d::CCScaleTo* scaleBackAction = cocos2d::CCScaleTo::create(kLabelComboScaleDuration, 1, 1);
            cocos2d::CCFiniteTimeAction* seqAction = cocos2d::CCSequence::create(scaleToAction, scaleBackAction, NULL);    
            label_stage_->runAction(seqAction);
        }
    }
}

void SkirmishHUD::SetupViewer()
{
    // Get window size and place the label upper. 
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();
    
    // Stage Label
    label_stage_background_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kLabelTitle.c_str());
    label_stage_background_->setPosition(ccp(size.width / 2, size.height - label_stage_background_->boundingBox().size.height));    
    addChild(label_stage_background_);

    label_stage_ = cocos2d::CCLabelTTF::create(
        "0", 
        kLabelTitleFont.c_str(), 
        kLabelTitleFontSize);
        
    label_stage_->setPosition(ccp(size.width / 2, size.height - label_stage_background_->boundingBox().size.height));
    addChild(label_stage_);

    //// TEST///
    Card3d* card = Card3d::cardWithFile("stripes.png", "stripes_p.png");
    //card->setPosition(ccp(size.width / 2, size.height - label_stage_background_->boundingBox().size.height));    
    addChild(card, -1);

    CCOrbitCamera* action1 = CCOrbitCamera::create(10, 1,1,0,360, 0, 0);
    cocos2d::CCSprite* sprite = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile("stripes.png");
    sprite->setPosition(ccp(size.width / 2, size.height - label_stage_background_->boundingBox().size.height));
    addChild(sprite);
    sprite->runAction(CCRepeatForever::create((CCActionInterval*)action1));
    //card->runAction(CCRepeatForever::create((CCActionInterval*)action1));
    //card->runAction(action1);

    //---------------------------------------------------------------
    // Game Menu
    //---------------------------------------------------------------
    button_pause_ = cocos2d::CCMenuItemImage::create(
        kButtonPause.c_str(), 
        kButtonPause.c_str(), 
        this, 
        menu_selector(SkirmishHUD::ButtonPauseCallback));
    button_pause_->setAnchorPoint(ccp(1.0, 1.0));
    button_pause_->setPosition(CCPointMake(size.width - kButtonPausePosition, size.height - kButtonPausePosition));

    button_debug_ = cocos2d::CCMenuItemImage::create(
        kButtonDebug.c_str(), 
        kButtonDebug.c_str(), 
        this, 
        menu_selector(SkirmishHUD::ButtonToggleDebugCallback));
    button_debug_->setAnchorPoint(ccp(1.0, 1.0));
    button_debug_->setPosition(CCPointMake(size.width - kButtonPausePosition * 3, size.height - kButtonPausePosition));

    cocos2d::CCMenu* pMenu = CCMenu::create(button_pause_, button_debug_, NULL);
    pMenu->setPosition(CCPointZero);    
    addChild(pMenu, kZOrderMenu, kZOrderMenu);
}

void SkirmishHUD::ButtonPauseCallback(CCObject* pSender)
{
    BattleScene* parent = static_cast<BattleScene*>(this->getParent());
    if (!parent->GamePaused())
    {
        CCLOG("[%s][%d] - The game changes to Paused state", __FUNCTION__, __LINE__);
        parent->PauseGame();

        //GameSettingLayer* layer = GameSettingLayer::create();
        //this->addChild(layer);
    }
    //else
    //{
    //    CCLOG("[%s][%d] - The game resumes to Normal state", __FUNCTION__, __LINE__);
    //    parent->ResumeGame();
    //}
}

void SkirmishHUD::ButtonToggleDebugCallback(CCObject* pSender)
{
    BattleScene* parent = static_cast<BattleScene*>(this->getParent());
    parent->TogglePhysicsDebugViewer();
}

} // namespace
