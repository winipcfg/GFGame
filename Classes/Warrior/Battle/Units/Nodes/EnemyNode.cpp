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
const short         kUnitMoveSpeed                      = 4;

EnemyNode::EnemyNode(Unit* unit)
    : UnitNode(unit),
      bot_(NULL)
{
    use_physics_motion_ = true;
    //use_physics_motion_ = false;
    this->schedule(schedule_selector(EnemyNode::Think), 0.5);
}

EnemyNode::~EnemyNode()
{
}

void EnemyNode::ResetState()
{
    if (state_ != NULL)
    {
        state_->Reset();
        //state_->SetWalkSpeed(kWalkSpeed);
        //state_->SetRunSpeed(kRunSpeed);
        state_->Detach(this);
        state_->Attach(this);
    }
}

bool EnemyNode::init()
{
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
    float duration = 2.0f;
    cocos2d::CCPoint position = action.Target->getPosition();
    CCLOG("Node tries move from (%0.2f, %0.2f) to attack target at (%0.2f, %0.2f) with duration %0.2f",
        this->getPosition().x,
        this->getPosition().y,
        position.x,
        position.y,
        duration);

    cocos2d::CCDelayTime* delayAction = cocos2d::CCDelayTime::create(duration);
    cocos2d::CCCallFunc* resolveAction = cocos2d::CCCallFunc::create(
        this,
        callfunc_selector(EnemyNode::ResolveAttack));
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::create(
        this,
        callfunc_selector(EnemyNode::FinishAction));
        
    cocos2d::CCFiniteTimeAction* newAction = cocos2d::CCSequence::create(
        delayAction,
        resolveAction, 
        endAction, 
        NULL);

    this->stopAllActions();
    this->runAction(newAction);
    ChangeFacingDirection((dx > 0) ? kFacingRight : kFacingLeft);
}

void EnemyNode::UpdateNode(CCFloat dt)
{            
    if (use_physics_motion_)
    {
        if (state_->Alive() && state_->Body())
        {
            CCPoint position;
            short speed = 0;
            switch (state_->CurrentAction().ActionType)
            {
			    case kUnitActionTypeMoveLeft:
                    speed = -kUnitMoveSpeed;
                    state_->SetFacing(kFacingLeft);
                    break;
			    case kUnitActionTypeMoveRight:
                    speed = kUnitMoveSpeed;
                    state_->SetFacing(kFacingRight);
                    break;
                default:
                    break;
            }

            if (speed)
            {
                b2Vec2 velocity = state_->Body()->GetLinearVelocity();
                float velChange = speed - velocity.x;
                float impulse = state_->Body()->GetMass() * velChange;
                state_->Body()->ApplyLinearImpulse(b2Vec2(impulse,0), state_->Body()->GetWorldCenter());
            }
        }   
    }
    else
    {
        if (state_->Alive())
        {
            CCPoint position;
            short speed = 0;
            switch (state_->CurrentAction().ActionType)
            {
			    case kUnitActionTypeMoveLeft:
                    speed = -kUnitMoveSpeed;
                    state_->SetFacing(kFacingLeft);
                    break;
			    case kUnitActionTypeMoveRight:
                    speed = kUnitMoveSpeed;
                    state_->SetFacing(kFacingRight);
                    break;
                default:
                    break;
            }
            position = this->getPosition();
            position.x += speed;
            this->setPosition(position);
        }
    }
    RefreshPosition();
}

void EnemyNode::Think(CCFloat dt)
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
