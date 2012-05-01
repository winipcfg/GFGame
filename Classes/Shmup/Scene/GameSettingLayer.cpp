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

#include "GameSettingLayer.h"
#include <string>

using namespace cocos2d;

namespace GFort { namespace Games { namespace Shmup 
{

const std::string   kSliderTrackSprite                = "Assets/Test/Controls/Slider/sliderTrack.png";
const std::string   kSliderProgressSprite             = "Assets/Test/Controls/Slider/sliderProgress.png";
const std::string   kSliderThumbSprite                = "Assets/Test/Controls/Slider/sliderThumb.png";

const std::string   kSwitchSpriteMask                 = "Assets/Test/Controls/Switch/switch-mask.png";
const std::string   kSwitchSpriteOn                   = "Assets/Test/Controls/Switch/switch-on.png";
const std::string   kSwitchSpriteOff                  = "Assets/Test/Controls/Switch/switch-off.png";
const std::string   kSwitchSpriteThumb                = "Assets/Test/Controls/Switch/switch-thumb.png";

//const std::string   kButtonBackSprite                 = "Assets/Test/Controls/Button/button.png";
const std::string   kButtonBackSprite                 = "Assets/Test/joystick_bg.png";

const float         kMusicVolumeMinimum               = 0.0f;
const float         kMusicVolumeMaximum               = 100.0f;
const float         kMusicVolumeInterval              = 20.0f;

GameSettingLayer::GameSettingLayer()
    : display_value_label_(NULL)
{
    // Get window size and place the label upper. 
    cocos2d::CCSize size = cocos2d::CCDirector::sharedDirector()->getWinSize();

    //---------------------------------------------------------------
    // Music Volume
    //---------------------------------------------------------------
    // Add a label in which the slider value will be displayed
    display_value_label_ = CCLabelTTF::labelWithString("Move the slider thumb!", "Marker Felt", 32);
    display_value_label_->setAnchorPoint(ccp(0.5f, -1.0f));
    display_value_label_->setPosition(ccp(size.width / 2.0f, size.height / 2.0f));
    addChild(display_value_label_);

    music_volume_control_ = CCControlSlider::sliderFromFiles(
        kSliderTrackSprite.c_str(), 
        kSliderProgressSprite.c_str(), 
        kSliderThumbSprite.c_str());
    music_volume_control_->setMinimumValue(kMusicVolumeMinimum);
    music_volume_control_->setMaximumValue(kMusicVolumeMaximum);
    music_volume_control_->setSnappingInterval(kMusicVolumeInterval);
    music_volume_control_->setPosition(ccp(size.width / 2.0f, size.height / 2.0f + 20));
    music_volume_control_->addTargetWithActionForControlEvents(
        this, 
        menu_selector(GameSettingLayer::ValueChanged),
        CCControlEventValueChanged);
    addChild(music_volume_control_);    

    //---------------------------------------------------------------
    // Audio Volume
    //---------------------------------------------------------------
    audio_volume_control_ = CCControlSlider::sliderFromFiles(
        kSliderTrackSprite.c_str(), 
        kSliderProgressSprite.c_str(), 
        kSliderThumbSprite.c_str());
    audio_volume_control_->setMinimumValue(kMusicVolumeMinimum);
    audio_volume_control_->setMaximumValue(kMusicVolumeMaximum);
    audio_volume_control_->setSnappingInterval(kMusicVolumeInterval);
    audio_volume_control_->setPosition(ccp(size.width / 2.0f, size.height / 2.0f - 20));
    audio_volume_control_->addTargetWithActionForControlEvents(
        this, 
        menu_selector(GameSettingLayer::ValueChanged),
        CCControlEventValueChanged);
    addChild(audio_volume_control_); 

    //---------------------------------------------------------------
    // Show Hints
    //--------------------------------------------------------------- 
    show_hints_control_ = CCControlSwitch::switchWithMaskSprite(
        CCSprite::spriteWithFile(kSwitchSpriteMask.c_str()),
        CCSprite::spriteWithFile(kSwitchSpriteOn.c_str()),
        CCSprite::spriteWithFile(kSwitchSpriteOff.c_str()),
        CCSprite::spriteWithFile(kSwitchSpriteThumb.c_str()),
        CCLabelTTF::labelWithString("ON", "Arial-BoldMT", 16),
        CCLabelTTF::labelWithString("OFF", "Arial-BoldMT", 16));
    show_hints_control_->setPosition(ccp(size.width / 2.0f, size.height / 2.0f - 80));
    show_hints_control_->setAnchorPoint(ccp(1, 0.5));
    addChild(show_hints_control_);

    //---------------------------------------------------------------
    // Clear Saved Data Button (Only exists in Splash Screen)
    //---------------------------------------------------------------
    // If parent is Splash, add button

    // Back Button
    CCScale9Sprite *backgroundButton = CCScale9Sprite::scale9SpriteWithFile(kButtonBackSprite.c_str(), CCRect(0,0,42,48));
    CCLabelTTF* test = CCLabelTTF::labelWithString("Back", "Marker Felt", 16);

    CCControlButton* button = CCControlButton::buttonWithLabelAndBackgroundSprite(test, backgroundButton);
    button->setPosition(ccp(size.width / 2.0f, 30));
    button->setAnchorPoint(ccp(0.5, 0));
    button->addTargetWithActionForControlEvents(
        this,
        menu_selector(GameSettingLayer::ButtonClicked),
        CCControlEventTouchUpInside);
    addChild(button);

    LoadUserDefault();
}

void GameSettingLayer::ValueChanged(CCControlSlider* sender)
{
    char text[100];    
    sprintf(text, "Slider value = %.0f%%", sender->getValue());
    display_value_label_->setString(text);
}

void GameSettingLayer::ButtonClicked(CCControlButton* sender)
{
    SaveUserDefault();
    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

GameSettingLayer::~GameSettingLayer()
{
}

void GameSettingLayer::LoadUserDefault()
{
    CCUserDefault* profile = CCUserDefault::sharedUserDefault();
    std::string path = profile->getXMLFilePath();
    CCLog("[%s][%d] - Retrieving user setting from ", __FUNCTION__, __LINE__, path);
    
    if (!profile->getBoolForKey("Initialized", false))
    {
        // If profile file does not exist, set default value and save.
        profile->setBoolForKey("Initialized", true);
        SaveUserDefault();
    }
    else
    {
        // If profile file does exist, load settings.
        music_volume_control_->setValue(profile->getFloatForKey("Music", 0));
        audio_volume_control_->setValue(profile->getFloatForKey("Audio", 0));
        show_hints_control_->setOn(profile->getBoolForKey("Hints", 0), false);
    }    
    
    CCLog("[%s][%d] - Retrieve user setting Completed ", __FUNCTION__, __LINE__);
}

void GameSettingLayer::SaveUserDefault()
{
    CCUserDefault* profile = CCUserDefault::sharedUserDefault();
    std::string path = profile->getXMLFilePath();
    CCLog("[%s][%d] - Saving user setting from ", __FUNCTION__, __LINE__, path);
    profile->setFloatForKey("Music", music_volume_control_->getValue());
    profile->setFloatForKey("Audio", audio_volume_control_->getValue());
    profile->setBoolForKey("Hints", show_hints_control_->getIsOn());
    profile->flush();
    CCLog("[%s][%d] - Save user setting Completed ", __FUNCTION__, __LINE__);
}

} } } // namespace
