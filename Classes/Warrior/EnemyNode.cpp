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
    cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    //cache->addSpriteFramesWithFile(kEnemySpriteFrame.c_str());
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

    cocos2d::CCDelayTime* delayAction = cocos2d::CCDelayTime::actionWithDuration(duration);
    cocos2d::CCCallFunc* resolveAction = cocos2d::CCCallFunc::actionWithTarget(
        this,
        callfunc_selector(EnemyNode::ResolveAttack));
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::actionWithTarget(
        this,
        callfunc_selector(EnemyNode::FinishAction));
        
    cocos2d::CCFiniteTimeAction* newAction = cocos2d::CCSequence::actions(
        delayAction,
        resolveAction, 
        endAction, 
        NULL);

    this->stopAllActions();
    this->runAction(newAction);
    ChangeFacingDirection((dx > 0) ? kFacingRight : kFacingLeft);
    //ShowAnimation(kUnitAnimationAttack);
}

void EnemyNode::UpdateNode(cocos2d::ccTime dt)
{    
    state_->Update(dt);
        
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

        cocos2d::CCPoint position = cocos2d::CCPoint(
            (state_->Body()->GetPosition().x * PTM_RATIO), 
            (state_->Body()->GetPosition().y * PTM_RATIO));
        float rotation = CC_RADIANS_TO_DEGREES(-1 * state_->Body()->GetAngle());
        this->setRotation(rotation);           
        this->setPosition(position);        
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

        if (state_->Body())
        {
            state_->Body()->SetTransform(
                b2Vec2((this->getPosition().x) * GFort::Core::Physics::kINV_PTM_RATIO, 
                       (this->getPosition().y + this->boundingBox().size.height / 2) * GFort::Core::Physics::kINV_PTM_RATIO), 
                state_->Body()->GetAngle());
            state_->Body()->SetAwake(true);
        }
    }
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

BPolygon EnemyNode::GetBoundingRegion()
{
    //cocos2d::CGFloat minX, minY, maxX, maxY;
    //cocos2d::CCRect rect = this->boundingBox();
    //minX = cocos2d::CCRect::CCRectGetMinX(rect);
    //maxX = cocos2d::CCRect::CCRectGetMaxX(rect);
    //minY = cocos2d::CCRect::CCRectGetMinY(rect);
    //maxY = cocos2d::CCRect::CCRectGetMaxY(rect);
    //return BattleHelper::ConvertToPolygon(minX, minY, maxX, maxY);
    //if (state_->Body())
    //{
    //    b2AABB aabb;
    //    aabb.lowerBound = b2Vec2(HUGE_FLOAT, HUGE_FLOAT);
    //    aabb.upperBound = b2Vec2(-HUGE_FLOAT, -HUGE_FLOAT); 
    //    b2Fixture* fixture = state_->Body()->GetFixtureList();
    //    while (fixture != NULL)
    //    {
    //        aabb.Combine(aabb, fixture->GetAABB());
    //        fixture = fixture->GetNext();
    //    }
    //}
    //return BPolygon();
    return BPolygon();
}

} // namespace
