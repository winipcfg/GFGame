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

#include "EnemyNode.h"
#include <cmath>
#include <CCExtensions/AdvanceSprite.h>
#include <CCExtensions/CCSpriteHelper.h>

using namespace cocos2d;

namespace Warrior 
{

const float kWalkSpeed                  = 100.0f;
const float kRunSpeed                   = 200.0f;

const std::string   kUnitSprite                         = "Assets/Char/enemy_idle.png";
//const std::string   kUnitSpriteFrameNameSyntaxRL        = "Assets/Char/enemy_run%02d.png";
//const std::string   kUnitSpriteFrameNameSyntaxRR        = "Assets/Char/enemy_run%02d.png";
//const std::string   kUnitSpriteFrameNameSyntaxAL        = "Assets/Char/enemy_idle.png";
//const std::string   kUnitSpriteFrameNameSyntaxAR        = "Assets/Char/enemy_idle.png";
const std::string   kUnitSpriteFrameNameSyntaxRL        = "Assets/Test/rogue.png";
const std::string   kUnitSpriteFrameNameSyntaxRR        = "Assets/Test/rogue.png";
const std::string   kUnitSpriteFrameNameSyntaxAL        = "Assets/Test/rogue.png";
const std::string   kUnitSpriteFrameNameSyntaxAR        = "Assets/Test/rogue.png";


EnemyNode::EnemyNode(Unit* unit)
    : UnitNode(unit),
      bot_(NULL)
{
    this->schedule(schedule_selector(EnemyNode::Think), 0.5);
    //this->scheduleUpdate();
}

EnemyNode::~EnemyNode()
{
}

void EnemyNode::ResetState()
{
    if (state_ != NULL)
    {
        state_->Reset();
        state_->SetWalkSpeed(kWalkSpeed);
        state_->SetRunSpeed(kRunSpeed);
        state_->Detach(this);
        state_->Attach(this);
    }
}

bool EnemyNode::init()
{
    cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    //cache->addSpriteFramesWithFile(kEnemySpriteFrame.c_str());

    // Create a sprite sheet with image
    //cocos2d::CCSpriteBatchNode* spriteSheet = cocos2d::CCSpriteBatchNode::batchNodeWithFile(kEnemySprite.c_str());
    //addChild(spriteSheet);

    //---------------------------------------------------------------
    // Sprites
    //---------------------------------------------------------------
    char frameName[100] = {0};
    sprite_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kUnitSprite.c_str());
    cocos2d::CCPoint pt = sprite_->getAnchorPoint();
    sprite_->setAnchorPoint(ccp(0.5, 0));
    this->addChild(sprite_);

    //---------------------------------------------------------------
    // Animations
    //---------------------------------------------------------------    
    cocos2d::CCAnimation* animation;
    animation = cocos2d::CCAnimation::animation();    
    animations_["RUN_LEFT"] = animation;
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRL.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRL.c_str(), 3);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRL.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRL.c_str(), 1);
    animation->addFrameWithFileName(frameName);    
    actions_["RUN_LEFT"] = cocos2d::CCRepeatForever::actionWithAction(
        cocos2d::CCAnimate::actionWithDuration(1, animation, true));
    actions_["RUN_LEFT"]->retain();

    animation = cocos2d::CCAnimation::animation();
    animations_["RUN_RIGHT"] = animation;
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRR.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRR.c_str(), 3);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRR.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxRR.c_str(), 1);
    animation->addFrameWithFileName(frameName);    
    actions_["RUN_RIGHT"] = cocos2d::CCRepeatForever::actionWithAction(
        cocos2d::CCAnimate::actionWithDuration(1, animation, true));
    actions_["RUN_RIGHT"]->retain();
    
    animation = cocos2d::CCAnimation::animation();       
    animations_["ATTACK_LEFT"] = animation;
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAL.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAL.c_str(), 3);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAL.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAL.c_str(), 1);
    animation->addFrameWithFileName(frameName);    
    actions_["ATTACK_LEFT"] = cocos2d::CCAnimate::actionWithDuration(1, animation, false);
    actions_["ATTACK_LEFT"]->retain();

    animation = cocos2d::CCAnimation::animation(); 
    animations_["ATTACK_RIGHT"] = animation;
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAR.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAR.c_str(), 3);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAR.c_str(), 2);
    animation->addFrameWithFileName(frameName);
    sprintf(frameName, kUnitSpriteFrameNameSyntaxAR.c_str(), 1);
    animation->addFrameWithFileName(frameName);    
    actions_["ATTACK_RIGHT"] = cocos2d::CCAnimate::actionWithDuration(1, animation, false);
    actions_["ATTACK_RIGHT"]->retain();
   
    return true;
}

void EnemyNode::UpdateAction()
{
    if (state_->Alive())
    {
        switch (state_->CurrentAction().ActionType)
        {
            case kUnitActionTypeAttack:
                Attack(state_->action_);
                break;
            default:
                break;
        }
    }
}

void EnemyNode::Attack(UnitAction& action)
{
    float dx = action.PositionX - this->getPosition().x;
    float jumpDuration = 1.0f;
    float jumpHeight = boundingBox().size.height;
    cocos2d::CCPoint position = action.Target->getPosition();
    CCLOG("Node tries move from (%0.2f, %0.2f) to attack target at (%0.2f, %0.2f) with duration %0.2f",
        this->getPosition().x,
        this->getPosition().y,
        position.x,
        position.y,
        jumpDuration);
    
    //cocos2d::CCJumpTo* jumpAction = cocos2d::CCJumpTo::actionWithDuration(
    //    jumpDuration, 
    //    position, 
    //    jumpHeight, 
    //    1);
    cocos2d::CCCallFunc* resolveAction = cocos2d::CCCallFunc::actionWithTarget(
        this,
        callfunc_selector(EnemyNode::ResolveAttack));
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
        this,
        callfunc_selector(EnemyNode::FinishAction));
        
    cocos2d::CCFiniteTimeAction* newAction = cocos2d::CCSequence::actions(
        //jumpAction, 
        resolveAction, 
        endAction, 
        NULL);

    this->stopAllActions();
    this->runAction(newAction);
    ChangeFacingDirection((dx > 0) ? kFacingRight : kFacingLeft);
    ShowAnimation(kUnitAnimationAttack);
}

void EnemyNode::Update(cocos2d::ccTime dt)
{    
    
}

void EnemyNode::Think(cocos2d::ccTime dt)
{
    BattleLayer* layer = (BattleLayer*) this->getParent();
    if (bot_ && layer && 
        this->state_->Alive() &&
        this->state_->IsUnderControl())
    {
        bot_->Update(*layer);
    }
}

void EnemyNode::Destroy() 
{ 
    if (state_->Body())
    {
        state_->Body()->GetWorld()->DestroyBody(state_->Body());
    }

    this->removeFromParentAndCleanup(true);
}

} // namespace
