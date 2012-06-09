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

#include "SkirmishLayer.h"
#include <CCExtensions/CCSpriteHelper.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <GFort/Core/MathHelper.h>
#include <GFort/Core/StringHelper.h>
#include "../CCExtensions/CCSpriteHelper.h"
#include "../Scene/SceneHelper.h"

using namespace cocos2d;

namespace Warrior 
{

const std::string   kSprite                     = "Assets/Test/Dummy/M/texture.png";
const std::string   kSpriteFrame                = "Assets/Test/Dummy/M/coordinates.plist";
const short         kIdleStart                  = 6;
const short         kIdleEnd                    = 9;

const std::string   kSpriteControlPointNormal   = "Assets/Test/castle.png";
const std::string   kSpriteControlPointSelected = "Assets/Test/castle.png";
const std::string   kSpriteControlPointDisable  = "Assets/Test/nige_lock.png";

const std::string   kSpriteIndicator            = "Assets/Test/ok.png";
const std::string   kSpriteIndicatorOK          = "Assets/Test/ok.png";
const std::string   kSpriteIndicatorNotOK       = "Assets/Test/not_ok.png";

const std::string   kSpriteEffectIndicator      = "Assets/Test/fire.png";
const std::string   kEffectIndicator            = "Assets/Shmup/Stars1.plist";

const std::string   kSpriteMap                  = "Assets/Test/rpg_map.png";

#define PLAY_BG_MUSIC           0//1

enum 
{
    kTagMap                 = 0,
    kTagStage               = 1,
    kTagAccessibleIndicator = 2,
    kTagHero                = 3,
    kTagEffectIndicator     = 4,
};

SkirmishLayer::SkirmishLayer()
    : Cistron::Component("CCLayer")
    , duration_(0)
    , touch_start_position_(cocos2d::CCPointZero)
    , touch_end_position_(cocos2d::CCPointZero)
    , stage_menu_(NULL)
    , player_node_(NULL)
    , current_point_(0)
    , effect_indicator_(NULL)
    , texture_ok_(NULL)
    , texture_not_ok_(NULL)
    , background_(NULL)
{
    texture_ok_ = cocos2d::CCTextureCache::sharedTextureCache()->addImage(kSpriteIndicatorOK.c_str());
    texture_not_ok_ = cocos2d::CCTextureCache::sharedTextureCache()->addImage(kSpriteIndicatorNotOK.c_str());

    SetupViewer();
    
    this->setIsTouchEnabled(true);

#if (PLAY_BG_MUSIC)
    audio_handler_.PlayBackgroundMusic();
#endif
}

SkirmishLayer::~SkirmishLayer()
{
}

void SkirmishLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    // Handle first touch
    Point touchLocation = convertTouchToNodeSpace(static_cast<cocos2d::CCTouch*>(pTouches->anyObject()));
    touch_start_position_ = touch_end_position_ = touchLocation;  
}

CCPoint SkirmishLayer::GetBoundLayerPosition(const CCPoint& newPos)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint retval = newPos;
    retval.x = MIN(retval.x, 0);
    retval.x = MAX(retval.x, -background_->getContentSize().width + winSize.width); 
    retval.y = MIN(retval.y, 0);
    retval.y = MAX(retval.y, -background_->getContentSize().height + winSize.height); 
    //retval.y = this->getPosition().y;
    return retval;
}

void SkirmishLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
    cocos2d::CCTouch* touch = static_cast<cocos2d::CCTouch*>(pTouches->anyObject());
    Point touchLocation = convertTouchToNodeSpace(touch);
    Point oldTouchLocation = touch->previousLocationInView(touch->view());
    oldTouchLocation = CCDirector::sharedDirector()->convertToGL(oldTouchLocation);
    oldTouchLocation = this->convertToNodeSpace(oldTouchLocation);

    // Check if single touch or multi touch
    if (pTouches->count() == 1)
    {
        CCPoint translation = ccpSub(touchLocation, oldTouchLocation);
        CCPoint newPosition = ccpAdd(this->getPosition(), translation);
        this->setPosition(GetBoundLayerPosition(newPosition));
    }
    else
    {

    }
}

void SkirmishLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
    Point touchLocation = convertTouchToNodeSpace(static_cast<cocos2d::CCTouch*>(pTouches->anyObject()));

    touch_start_position_ = touch_end_position_ = cocos2d::CCPointZero; 
}

void SkirmishLayer::SetupViewer()
{
    cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();   

    ////////////////////////////////////////////////////////////
    // Load data file
    ////////////////////////////////////////////////////////////
    background_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kSpriteMap.c_str()); 
    background_->setAnchorPoint(cocos2d::CCPointZero);
    addChild(background_, kTagMap, kTagMap);

    // For each control point, add an item image
    stage_menu_ = cocos2d::CCMenu::menuWithItem(NULL); 
    cocos2d::CCMenuItemImage* buttonStage;
    cocos2d::CCSprite* sprite;
    int num = 10;
    for (int i = 0; i < num; ++i)
    {
        ControlPoint pt;
        pt.Index = i;
        pt.Name = "Stage - " + GFort::Core::StringHelper::ToString(i);
        pt.Description = "DESC";
        pt.Type = 0;
        pt.X = rand() % (int)background_->getContentSize().width;
        pt.Y = rand() % (int)background_->getContentSize().height;
        control_points_.push_back(pt);

        buttonStage = cocos2d::CCMenuItemImage::itemFromNormalImage(
            kSpriteControlPointNormal.c_str(), 
            kSpriteControlPointSelected.c_str(), 
            kSpriteControlPointDisable.c_str(),
            this, 
            menu_selector(SkirmishLayer::ControlPointCallback));
        cocos2d::CCPoint point = ccp(pt.X, pt.Y);
        buttonStage->setAnchorPoint(ccp(0.5, 0.5));
        buttonStage->setPosition(point);

        sprite = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kSpriteIndicator.c_str()); 
        sprite->setAnchorPoint(ccp(0.5, 0.5));
        sprite->setPosition(point);
        this->addChild(sprite, kTagAccessibleIndicator, kTagAccessibleIndicator);

        //buttonStage->addChild(sprite);
        accessible_indicators_.push_back(sprite);

        stage_menu_->addChild(buttonStage, i, i);
    }    

    // Create control point links
    int links = 10;
    for (int i = 1; i < links; ++i)
    {
        ControlPointLink link;
        link.StartPointIndex = i-1;
        link.EndPointIndex = i;
        control_point_links_.push_back(link);
    }

    stage_menu_->setPosition(CCPointZero);    
    addChild(stage_menu_, kTagStage, kTagStage);

    // Create the player and set the location
    player_node_ = new AdvanceSprite();
    player_node_->init();
    player_node_->addFrames(kSpriteFrame.c_str(), kSprite.c_str());
    player_node_->startAnimation(kIdleStart, kIdleEnd, -1, 0, this, 10, false, false);
    this->addChild(player_node_, kTagHero, kTagHero);
    
    // Particle effect
    effect_indicator_ = cocos2d::CCParticleFire::node();
    //effect_indicator_->setPositionType(kCCPositionTypeGrouped);
    effect_indicator_->setTexture(cocos2d::CCTextureCache::sharedTextureCache()->addImage(kSpriteEffectIndicator.c_str()));
    //effect_indicator_->setAnchorPoint(ccp(0.5, 0.5));
    effect_indicator_->setPosition(cocos2d::CCPointZero);
    player_node_->addChild(effect_indicator_, kTagEffectIndicator, kTagEffectIndicator);

    ////////////////////////////////////////////////////////////
    // Load saved data file
    ////////////////////////////////////////////////////////////
    accessible_control_points_.insert(0);
    accessible_control_points_.insert(1);

    ////////////////////////////////////////////////////////////
    // Update status
    ////////////////////////////////////////////////////////////    
    SetCurrentPoint(0, 0);
    RefreshViewer();
}

void SkirmishLayer::RefreshViewer()
{
    CCLOG("[%s][%d] - Started", __FUNCTION__, __LINE__);

    // Find and refresh all control points that are able to access
    accessible_control_points_.clear();
    std::vector<ControlPointLink>::iterator it;
    for (it = control_point_links_.begin(); it != control_point_links_.end(); ++it)
    {
        if (it->StartPointIndex == current_point_ || it->EndPointIndex == current_point_)
        {
            accessible_control_points_.insert(it->StartPointIndex);
            accessible_control_points_.insert(it->EndPointIndex);
        }
    }
    //accessible_control_points_.erase(current_point_);

    // Only display control points that are accessible
    //cocos2d::CCObject* child;
    //CCArray* children = stage_menu_->getChildren();
    //CCARRAY_FOREACH(children, child)
    //{
    //    cocos2d::CCMenuItemImage* button = static_cast<cocos2d::CCMenuItemImage*>(child);
    //    if (button &&
    //        accessible_control_points_.find(button->getTag()) != accessible_control_points_.end())
    //    {
    //        button->setIsEnabled(true);
    //    }
    //    else
    //    {
    //        button->setIsEnabled(false);
    //    }
    //}

    // Update control point indicators
    for (unsigned int i = 0; i < accessible_indicators_.size(); ++i)
    {        
        if (IsControlPointAccessible(i))
            accessible_indicators_[i]->setTexture(texture_ok_);
        else
            accessible_indicators_[i]->setTexture(texture_not_ok_);
    }
    
    CCLOG("[%s][%d] - Ended", __FUNCTION__, __LINE__);
}

void SkirmishLayer::ControlPointCallback(CCObject* pSender)
{
    float duration = 1;
    cocos2d::CCMenuItemImage* menuItem = static_cast<cocos2d::CCMenuItemImage*>(pSender);
    int index = menuItem->getTag();
    if (index != current_point_)
    {
        for (std::vector<ControlPointLink>::iterator it = control_point_links_.begin(); 
            it != control_point_links_.end(); 
            ++it)
        {
            if ((it->StartPointIndex == index && it->EndPointIndex == current_point_) ||
                (it->StartPointIndex == current_point_ && it->EndPointIndex == index))
            {
                CCLOG("[%s][%d] - Move to the location", __FUNCTION__, __LINE__);
                SetCurrentPoint(index, duration);
                break;
            }
        }
    }
    else
    {
        CCLog("[%s][%d] - Go to Battle", __FUNCTION__, __LINE__);
        Warrior::SceneHelper manager;
        manager.GoBattleScene(0);
    }
}

void SkirmishLayer::SetCurrentPoint(const short& index, const float& duration)
{
    ControlPoint* point = &control_points_[index];
    if (duration < 0.01)
    {
        player_node_->setPosition(ccp(point->X, point->Y));
    }
    else
    {
        // Hide Stage Viewer
        // Move 
        // Show Stage Viewer
        cocos2d::CCMoveTo* moveAction = cocos2d::CCMoveTo::actionWithDuration(duration, ccp(point->X, point->Y));
        cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(this, callfunc_selector(SkirmishLayer::RefreshViewer));
        cocos2d::CCFiniteTimeAction* newAction = cocos2d::CCSequence::actions(moveAction, endAction, NULL);
        player_node_->runAction(newAction);
    }    
    current_point_ = index;
}

const bool SkirmishLayer::IsControlPointAccessible(const short& index) const
{
    return (accessible_control_points_.find(index) != accessible_control_points_.end());
}

void SkirmishLayer::draw(void)
{
    // Draw control point linkage
    glLineWidth(10);
    for (unsigned int i = 0; i < control_point_links_.size(); ++i)
    {
        ControlPointLink* link = &control_point_links_[i];
        ControlPoint* startPoint = &control_points_[link->StartPointIndex]; 
        ControlPoint* endPoint = &control_points_[link->EndPointIndex]; 
        
        ccDrawLine(ccp(startPoint->X, startPoint->Y), ccp(endPoint->X, endPoint->Y));
    }
}

} // namespace
