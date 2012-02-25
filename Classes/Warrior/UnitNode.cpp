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

#include "UnitNode.h"
#include <cmath>
#include <string>
#include "CCExtensions/CCTranslate.h"
#include <GFort/Core/Macros.h>
#include <GFort/Core/StringHelper.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <Warrior/Model/UnitAction.h>
#include <Warrior/Model/BattleHelper.h>
#include <Warrior/UnitDebugViewer.h>
#include "BattleLayer.h"

using namespace cocos2d;

namespace Warrior 
{

//const float         kPlayerHeight                       = 48.0f;
const short         kUnitMoveSpeed                      = 2;

UnitNode::UnitNode(Unit* unit)
    : Cistron::Component("UnitComponent"),
      needs_update_(false),
      sprite_(NULL),
      //sprite_new_(NULL),
      state_(unit),
      use_physics_motion_(false)
{
    this->scheduleUpdate();
    this->schedule(schedule_selector(UnitNode::UpdateNode)); 

    UnitDebugViewer* viewer = UnitDebugViewer::node();
    viewer->SetUnit(state_);
    this->addChild(viewer, 1);
}

UnitNode::~UnitNode()
{
    for (std::hash_map<std::string, cocos2d::CCAction* >::iterator it = actions_.begin(); 
        it != actions_.end(); 
        ++it) 
    {
        if (it->second)
            it->second->release();
    } 
    this->unscheduleUpdate();
    //CC_SAFE_RELEASE(sprite_new_);
}

cocos2d::CCRect UnitNode::boundingBox(void) 
{ 
    if (sprite_)
    {
        cocos2d::CCRect region = sprite_->boundingBox();
        return CCRectMake(
            this->getPosition().x + region.origin.x,
            this->getPosition().y + region.origin.y,
            region.size.width,
            region.size.height);
    }
    else
    {
        return cocos2d::CCRectZero; 
    }
} 

cocos2d::CCSize UnitNode::Size() 
{ 
    return (sprite_) ? sprite_->boundingBox().size : cocos2d::CCSizeZero;
}

BPolygon UnitNode::GetBoundingRegion()
{
    cocos2d::CGFloat minX, minY, maxX, maxY;
    cocos2d::CCRect rect = this->boundingBox();
    minX = cocos2d::CCRect::CCRectGetMinX(rect);
    maxX = cocos2d::CCRect::CCRectGetMaxX(rect);
    minY = cocos2d::CCRect::CCRectGetMinY(rect);
    maxY = cocos2d::CCRect::CCRectGetMaxY(rect);
    return BattleHelper::ConvertToPolygon(minX, minY, maxX, maxY);
}

void UnitNode::Reset()
{
    state_->Reset();
}

void UnitNode::ResolveAttack()
{
    if (state_->Alive() && 
        state_->CurrentAction().ActionType == kUnitActionTypeAttack)
    {
        UnitNode* target = state_->CurrentAction().Target;
        if (target && target->GetState()->Alive())
        {
            BattleLayer* battleLayer = static_cast<BattleLayer*>(this->getParent());
            //battleLayer->ResolveAttack(*this, *target);
            
            b2Vec2 position(this->getPosition().x + this->getParent()->getPosition().x, this->getPosition().y + 60 + this->getParent()->getPosition().y);
            b2Vec2 targetPosition(target->getPosition().x + this->getParent()->getPosition().x, target->getPosition().y + 60 + this->getParent()->getPosition().y);
            MissileNode* node = battleLayer->GetBattle()->ShootMissile(0, 0, position, targetPosition, 1.0f, true);
            battleLayer->addChild(node);
        }
    }
}

void UnitNode::Die()
{
    CCLOG("Node is death");    
    state_->Die();

    cocos2d::CCDelayTime* delayAction = cocos2d::CCDelayTime::actionWithDuration(10);

    cocos2d::CCCallFunc* dieAction = cocos2d::CCCallFunc::actionWithTarget(this, callfunc_selector(UnitNode::SetInvisible));

    cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::actions(
        delayAction,
        dieAction, 
        NULL);

    this->stopAllActions();
    this->runAction(action);
    use_physics_motion_ = true;
}

void UnitNode::ChangeFacingDirection(const FacingDirection& value)
{
    if (state_->Facing() != value)
    {
        state_->SetFacing(value);
        needs_update_ = true;
    }
}

void UnitNode::ShowAnimation(const UnitAnimationType& value)
{
    if (animation_type_ != value ||
        needs_update_)
    {
        animation_type_ = value;
        sprite_->stopAllActions();

        if (state_->Facing() == kFacingLeft)
        {
            switch (animation_type_)
            {
            case kUnitAnimationIdle:
                //sprite_->runAction(cocos2d::CCAnimate::actionWithDuration(1, animations_["RUN_LEFT"], false));
                sprite_->setFlipX(true);
                sprite_->runAction(actions_["RUN_LEFT"]);
                break;
            case kUnitAnimationRun:
                //sprite_->runAction(cocos2d::CCAnimate::actionWithDuration(1, animations_["RUN_LEFT"], false));
                sprite_->setFlipX(true);
                sprite_->runAction(actions_["RUN_LEFT"]);
                break;
            case kUnitAnimationAttack:
                //sprite_->runAction(cocos2d::CCAnimate::actionWithDuration(1, animations_["ATTACK_LEFT"], false));
                sprite_->setFlipX(true);
                sprite_->runAction(actions_["ATTACK_LEFT"]);
                break;
            default:
                break;
            }
        }
        else
        {
            switch (animation_type_)
            {
            case kUnitAnimationIdle:
                //sprite_->runAction(cocos2d::CCAnimate::actionWithDuration(1, animations_["RUN_RIGHT"], false));
                sprite_->setFlipX(false);
                sprite_->runAction(actions_["RUN_RIGHT"]);
                break;
            case kUnitAnimationRun:
                //sprite_->runAction(cocos2d::CCAnimate::actionWithDuration(1, animations_["RUN_RIGHT"], false));
                sprite_->setFlipX(false);
                sprite_->runAction(actions_["RUN_RIGHT"]);
                break;
            case kUnitAnimationAttack:
                //sprite_->runAction(cocos2d::CCAnimate::actionWithDuration(1, animations_["ATTACK_RIGHT"], false));
                sprite_->setFlipX(false);
                sprite_->runAction(actions_["ATTACK_RIGHT"]);
                break;
            default:
                break;
            }
        }
        needs_update_ = false;
    }
}

void UnitNode::ResetState()
{

}

void UnitNode::UpdateNode(cocos2d::ccTime dt)
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
                    break;
			    case kUnitActionTypeMoveRight:
                    speed = kUnitMoveSpeed;
                    break;
                default:
                    break;
            }

            if (speed)
            {
                speed *= 2;
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
        sprite_->setAnchorPoint(ccp(0.5, 0.5));
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
                    break;
			    case kUnitActionTypeMoveRight:
                    speed = kUnitMoveSpeed;
                    break;
                default:
                    break;
            }
            position = this->getPosition();
            position.x += speed;
            this->setPosition(position);
        }

        //if (state_->Body())
        //{
        //    state_->Body()->SetTransform(
        //        b2Vec2((this->getParent()->getPosition().x + this->getPosition().x) * GFort::Core::Physics::kINV_PTM_RATIO, 
        //               (this->getParent()->getPosition().y + this->getPosition().y + this->boundingBox().size.height / 2) * GFort::Core::Physics::kINV_PTM_RATIO), 
        //        state_->Body()->GetAngle());
        //    state_->Body()->SetAwake(true);
        //}
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

void UnitNode::FinishAction()
{
    state_->action_.Reset();
    ShowAnimation(kUnitAnimationIdle);
}

void UnitNode::SetInvisible()
{
    this->setIsVisible(false);
}

void UnitNode::Update(Unit* subject)
{
    UpdateAction();
}

} // namespace
