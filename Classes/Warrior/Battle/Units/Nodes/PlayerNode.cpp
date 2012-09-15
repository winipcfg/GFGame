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
#include "CCExtensions/CCSpriteHelper.h"
#include "CCExtensions/CCTranslate.h"
#include "CCExtensions/CCB2BodyMoveBy.h"
#include "../../BattleHelper.h"
#include "../../BattleLayer.h"

using namespace cocos2d;

namespace Warrior 
{

PlayerNode::PlayerNode(Unit* unit)
    : UnitNode(unit)
{
    use_physics_motion_ = true;
    //use_physics_motion_ = false;
}

PlayerNode::~PlayerNode()
{
}

void PlayerNode::ResetState()
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

bool PlayerNode::init()
{
    return true;
}

void PlayerNode::UpdateNode(float dt)
{
    RefreshPosition();
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
            default:
                break;
        }
    }
}

void PlayerNode::Walk(const cocos2d::CCPoint& position)
{
    cocos2d::CCPoint diff = cocos2d::ccpSub(position, this->getPosition());
    diff.y = 0;
    float duration = abs(diff.x) / state_->Properties()->WalkSpeed();

    CCLOG("Node move to (%0.2f, %0.2f) with duration %0.2f", 
            position.x, 
            position.y, 
            duration);
    
    GFGame::CCB2BodyMoveBy* moveAction = GFGame::CCB2BodyMoveBy::create(duration, state_->Body(), diff, PTM_RATIO);
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::create(this, callfunc_selector(PlayerNode::FinishAction));
    cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::create(moveAction, endAction, NULL);

    this->stopAllActions();
    this->runAction(action);
        
    ChangeFacingDirection((diff.x > 0) ? kFacingRight : kFacingLeft);
}

void PlayerNode::Run(const cocos2d::CCPoint& position)
{
    cocos2d::CCPoint diff = cocos2d::ccpSub(position, this->getPosition());
    diff.y = 0;
    float duration = abs(diff.x) / state_->Properties()->RunSpeed();

    CCLOG("Node move to (%0.2f, %0.2f) with duration %0.2f", 
            position.x, 
            position.y, 
            duration);

    GFGame::CCB2BodyMoveBy* moveAction = GFGame::CCB2BodyMoveBy::create(duration, state_->Body(), diff, PTM_RATIO);
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::create(this, callfunc_selector(PlayerNode::FinishAction));
    cocos2d::CCFiniteTimeAction* action = cocos2d::CCSequence::create(moveAction, endAction, NULL);

    this->stopAllActions();
    this->runAction(action);
    
    ChangeFacingDirection((diff.x > 0) ? kFacingRight : kFacingLeft);
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
    
    cocos2d::CCCallFunc* startAction = cocos2d::CCCallFunc::create(this, callfunc_selector(PlayerNode::StartNodeMotion));
    cocos2d::CCMoveTo* moveAction = cocos2d::CCMoveTo::create(moveDuration, position);
    cocos2d::CCCallFunc* refreshAction = cocos2d::CCCallFunc::create(this, callfunc_selector(PlayerNode::RefreshPosition));
    cocos2d::CCCallFunc* callAction = cocos2d::CCCallFunc::create(this->getParent(), callfunc_selector(BattleLayer::DoSlice));
    cocos2d::CCCallFunc* stopAction = cocos2d::CCCallFunc::create(this, callfunc_selector(PlayerNode::StopNodeMotion));
    cocos2d::CCCallFunc* endAction = cocos2d::CCCallFunc::create(this, callfunc_selector(PlayerNode::FinishAction));
    
    cocos2d::CCFiniteTimeAction* newAction = cocos2d::CCSequence::actions(
        startAction, moveAction, refreshAction, callAction, 
        stopAction, 
        endAction, 
        NULL);
    
    this->stopAllActions();
    this->runAction(newAction);

    float dx = position.x - this->getPosition().x;
    ChangeFacingDirection((dx > 0) ? kFacingRight : kFacingLeft);
}

} // namespace
