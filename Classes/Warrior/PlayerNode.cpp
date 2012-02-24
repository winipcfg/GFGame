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

#include "PlayerNode.h"
#include <cmath>
#include <boost/format.hpp>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include "BattleLayer.h"
#include "CCExtensions/CCSpriteHelper.h"
#include "CCExtensions/CCTranslate.h"

using namespace cocos2d;

namespace Warrior 
{
const float kWalkSpeed                      = 200.0f;
const float kRunSpeed                       = 400.0f;

//const std::string   kPlayerSprite                       = "Assets/FSM/vx_chara/vx_chara04_a.png";
//const std::string   kPlayerSpriteFrame                  = "Assets/FSM/vx_chara/vx_chara04_a.plist";
//const std::string   kUnitSpriteFrameNameSyntax          = "warrior%02d.png";
//const std::string   kUnitSpriteAttackFrameNameSyntax    = "warrior_attack%02d.png";

const std::string   kUnitSpriteIdle                     = "Assets/Char/hero_idle.png";
const std::string   kUnitSpriteFrameNameSyntaxRL        = "Assets/Char/enemy_run%02d.png";
const std::string   kUnitSpriteFrameNameSyntaxRR        = "Assets/Char/enemy_run%02d.png";
const std::string   kUnitSpriteFrameNameSyntaxAL        = "Assets/Char/hero_idle.png";
const std::string   kUnitSpriteFrameNameSyntaxAR        = "Assets/Char/hero_idle.png";

PlayerNode::PlayerNode(Unit* unit)
    : UnitNode(unit)
{
    use_physics_motion_ = false;
}

PlayerNode::~PlayerNode()
{
}

void PlayerNode::ResetState()
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

bool PlayerNode::init()
{
    cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    //cache->addSpriteFramesWithFile(kPlayerSpriteFrame.c_str());

    // Create a sprite sheet with image
    //cocos2d::CCSpriteBatchNode* spriteSheet = cocos2d::CCSpriteBatchNode::batchNodeWithFile(kPlayerSprite.c_str());
    //addChild(spriteSheet);

    //---------------------------------------------------------------
    // Sprites
    //---------------------------------------------------------------
    char frameName[100] = {0};
    sprite_ = GFGame::CCSpriteHelper::spriteWithSpriteFrameNameOrFile(kUnitSpriteIdle.c_str());
    cocos2d::CCPoint pt = sprite_->getAnchorPoint();
    sprite_->setAnchorPoint(ccp(0.5, 0));
    this->addChild(sprite_);
    
    //---------------------------------------------------------------
    // Animations
    //---------------------------------------------------------------    
    //run_left_animation_ = cocos2d::CCAnimation::animation();        
    //sprintf(frameName, kUnitSpriteFrameNameSyntax.c_str(), 5);
    //run_left_animation_->addFrame(cache->spriteFrameByName(frameName));
    //sprintf(frameName, kUnitSpriteFrameNameSyntax.c_str(), 6);
    //run_left_animation_->addFrame(cache->spriteFrameByName(frameName));
    //sprintf(frameName, kUnitSpriteFrameNameSyntax.c_str(), 5);
    //run_left_animation_->addFrame(cache->spriteFrameByName(frameName));
    //sprintf(frameName, kUnitSpriteFrameNameSyntax.c_str(), 4);
    //run_left_animation_->addFrame(cache->spriteFrameByName(frameName));
    //run_left_action_ = cocos2d::CCRepeatForever::actionWithAction(
    //    cocos2d::CCAnimate::actionWithDuration(1, run_left_animation_, true));
    //run_left_action_->retain();

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

void PlayerNode::UpdateAction()
{
    if (state_->Alive())
    {
        switch (state_->CurrentAction().ActionType)
        {
            case kUnitActionTypeWalk:			
                Walk(ccp(state_->action_.PositionX, state_->action_.PositionY));
                break;
            case kUnitActionTypeRun:
                Run(ccp(state_->action_.PositionX, state_->action_.PositionY));
                break;
            case kUnitActionTypeAttack:
                Attack(state_->action_);
                break;
            case kUnitActionTypeMoveLeft:
			case kUnitActionTypeMoveRight:
                Move(ccp(state_->action_.PositionX, state_->action_.PositionY));
                break;
            default:
                break;
        }
    }
}

void PlayerNode::Walk(const cocos2d::CCPoint& position)
{
    cocos2d::CCPoint diff = cocos2d::ccpSub(position, this->getPosition());
    diff.y = 0;
    float duration = abs(diff.x) / state_->WalkSpeed();

    CCLOG("Node move to (%0.2f, %0.2f) with duration %0.2f", 
            position.x, 
            position.y, 
            duration);
    
    if (use_physics_motion_)
    {
        //float maxAllowableForce = 1000.0f;
        //b2Vec2 currentPosition = b2Vec2(state_->Body()->GetPosition().x, state_->Body()->GetPosition().y);
        //b2Vec2 desiredPosition = b2Vec2(position.x * GFort::Core::Physics::kINV_PTM_RATIO, position.y * GFort::Core::Physics::kINV_PTM_RATIO);
        //b2Vec2 necessaryMovement = desiredPosition - currentPosition;
        //float necessaryDistance = necessaryMovement.Length();
        //necessaryMovement.Normalize();
        //float forceMagnitude = b2Min(maxAllowableForce, necessaryDistance);
        //b2Vec2 force = forceMagnitude * necessaryMovement;
        //state_->Body()->ApplyForceToCenter(state_->Body()->GetMass() * 100 * force);

        //// Apply force
        //GFort::Core::Physics::Box2dSettings setting;
        //b2Vec2 impulse = GFort::Core::Physics::PhysicsHelper::GetTrajectoryVelocity(
        //    state_->Body()->GetWorld(),
        //    &setting,
        //    b2Vec2(this->getPosition().x, this->getPosition().y),
        //    b2Vec2(position.x, position.y),
        //    duration);
        //state_->Body()->ApplyLinearImpulse(state_->Body()->GetMass() * impulse, state_->Body()->GetWorldCenter());

        //cocos2d::CCDelayTime* delayAction = cocos2d::CCDelayTime::actionWithDuration(duration);
        //cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
        //    this,
        //    callfunc_selector(PlayerNode::FinishAction));
        //cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::actions(delayAction, endAction, NULL);
        //this->stopAllActions();
        //this->runAction(action);
    }
    else
    {
        GFGame::CCTranslate* moveAction = GFGame::CCTranslate::actionWithDuration(duration, diff);
        cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
            this,
            callfunc_selector(PlayerNode::FinishAction));
        cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::actions(moveAction, endAction, NULL);

        this->stopAllActions();
        this->runAction(action);
    }
        
    ChangeFacingDirection((diff.x > 0) ? kFacingRight : kFacingLeft);
    ShowAnimation(kUnitAnimationRun);
}

void PlayerNode::Run(const cocos2d::CCPoint& position)
{
    cocos2d::CCPoint diff = cocos2d::ccpSub(position, this->getPosition());
    diff.y = 0;
    float duration = abs(diff.x) / state_->RunSpeed();

    CCLOG("Node move to (%0.2f, %0.2f) with duration %0.2f", 
            position.x, 
            position.y, 
            duration);

    GFGame::CCTranslate* moveAction = GFGame::CCTranslate::actionWithDuration(duration, diff);
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
        this,
        callfunc_selector(PlayerNode::FinishAction));
    cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::actions(moveAction, endAction, NULL);

    this->stopAllActions();
    this->runAction(action);
    
    ChangeFacingDirection((diff.x > 0) ? kFacingRight : kFacingLeft);
    ShowAnimation(kUnitAnimationRun);
}

void PlayerNode::Attack(UnitAction& action)
{
    cocos2d::CCPoint position = ccp(state_->action_.PositionX, state_->action_.PositionY);
    float moveDuration = 0.0f;
    CCLOG("Node tries move from (%0.2f, %0.2f) to (%0.2f, %0.2f) with duration %0.2f",
        this->getPosition().x,
        this->getPosition().y,
        position.x,
        position.y,
        moveDuration);
    
    if (use_physics_motion_)
    {
        //// Apply force
        //GFort::Core::Physics::Box2dSettings setting;
        //b2Vec2 impulse = GFort::Core::Physics::PhysicsHelper::GetTrajectoryVelocity(
        //    state_->Body()->GetWorld(),
        //    &setting,
        //    b2Vec2(this->getPosition().x, this->getPosition().y),
        //    b2Vec2(position.x, position.y),
        //    moveDuration);
        //state_->Body()->ApplyLinearImpulse(state_->Body()->GetMass() * impulse, state_->Body()->GetWorldCenter());


        //cocos2d::CCDelayTime* delayAction = cocos2d::CCDelayTime::actionWithDuration(moveDuration);
        //cocos2d::CCCallFunc* callAction = cocos2d::CCCallFunc::actionWithTarget(
        //    this->getParent(), 
        //    callfunc_selector(BattleLayer::DoSlice));
        //cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
        //    this,
        //    callfunc_selector(PlayerNode::FinishAction));
        //cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::actions(delayAction, callAction, endAction, NULL);
        //this->stopAllActions();
        //this->runAction(action);
    }
    else
    {
        cocos2d::CCMoveTo* moveAction = cocos2d::CCMoveTo::actionWithDuration(
            moveDuration, 
            position);
        cocos2d::CCCallFunc* callAction = cocos2d::CCCallFunc::actionWithTarget(
            this->getParent(), 
            callfunc_selector(BattleLayer::DoSlice));
        cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
            this,
            callfunc_selector(PlayerNode::FinishAction));
    
        cocos2d::CCFiniteTimeAction* newAction = cocos2d::CCSequence::actions(
            moveAction, callAction, endAction, NULL);
    
        this->stopAllActions();
        this->runAction(newAction);
    }

    float dx = position.x - this->getPosition().x;
    ChangeFacingDirection((dx > 0) ? kFacingRight : kFacingLeft);
    ShowAnimation(kUnitAnimationAttack);
}

void PlayerNode::Move(const cocos2d::CCPoint& position)
{
    cocos2d::CCPoint diff = cocos2d::ccpSub(position, this->getPosition());
    diff.y = 0;
    this->stopAllActions();
        
    ChangeFacingDirection((diff.x > 0) ? kFacingRight : kFacingLeft);
    ShowAnimation(kUnitAnimationRun);
}

} // namespace
