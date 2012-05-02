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
#include <Warrior/ViewController/UnitViewer.h>
#include "BattleLayer.h"

using namespace cocos2d;

namespace Warrior 
{
    
UnitNode::UnitNode(Unit* unit)
    : Cistron::Component("UnitComponent"),
      state_(unit),
      use_physics_motion_(false)
{
    this->scheduleUpdate();
    this->schedule(schedule_selector(UnitNode::UpdateNode)); 

    UnitViewer* viewer = UnitViewer::node();
    viewer->SetUnit(state_);
    this->addChild(viewer, 1);
}

UnitNode::~UnitNode()
{
    this->unscheduleUpdate();
}

void UnitNode::Reset()
{
    state_->Reset();
}

void UnitNode::RefreshPosition()
{
    if (use_physics_motion_)
    {
        cocos2d::CCPoint position = cocos2d::CCPoint(
            (state_->Body()->GetPosition().x * PTM_RATIO), 
            (state_->Body()->GetPosition().y * PTM_RATIO));
        float rotation = CC_RADIANS_TO_DEGREES(-1 * state_->Body()->GetAngle());
        this->setRotation(rotation);           
        this->setPosition(position);
    }
    else
    {
        if (state_->Body())
        {
            state_->Body()->SetTransform(
                b2Vec2((this->getPosition().x) * GFort::Core::Physics::kINV_PTM_RATIO, 
                       (this->getPosition().y) * GFort::Core::Physics::kINV_PTM_RATIO), 
                state_->Body()->GetAngle());
            state_->Body()->SetAwake(true);
        }
    }
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

    //cocos2d::CCCallFunc* dieAction = cocos2d::CCCallFunc::actionWithTarget(this, callfunc_selector(UnitNode::SetInvisible));

    cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::actions(
        delayAction,
        //dieAction, 
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
    }
}

void UnitNode::ResetState()
{

}

void UnitNode::FinishAction()
{
    state_->action_.Reset();
}

void UnitNode::Update(Unit* subject)
{
    UpdateAction();
}

} // namespace
