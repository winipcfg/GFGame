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

#include "GraphicNode.h"
#include <cmath>
#include <CCExtensions/AdvanceSprite.h>
#include <CCExtensions/CCSpriteHelper.h>
//#include <Warrior/Model/Struct.h>

using namespace cocos2d;

namespace Warrior 
{

const std::string   kSprite             = "Assets/Test/Dummy/M/texture.png";
const std::string   kSpriteFrame        = "Assets/Test/Dummy/M/coordinates.plist";

const short kIdleStart      = 6;
const short kIdleEnd        = 9;
const short kWalkStart      = 18;
const short kWalkEnd        = 25;
const short kRunStart       = 10;
const short kRunEnd         = 17;

GraphicNode::GraphicNode(Unit* unit)
    : Cistron::Component("UnitComponent")
    , state_(unit)
    , sprite_(NULL)
    , facing_(kFacingLeft)
    , action_type_(kUnitActionTypeIdle)
    , needs_update_(false)
{
    //this->schedule(schedule_selector(GraphicNode::UpdateNode));
}

GraphicNode::~GraphicNode()
{
    this->unscheduleAllSelectors();
    this->unscheduleUpdate();
    sprite_->removeFromParentAndCleanup(true);
}

cocos2d::CCSize GraphicNode::Size() 
{ 
    return (sprite_) ? sprite_->boundingBox().size : cocos2d::CCSizeZero;
}

bool GraphicNode::init()
{
    sprite_ = new AdvanceSprite();
    sprite_->init();
    sprite_->addFrames(kSpriteFrame.c_str(), kSprite.c_str());
    sprite_->startAnimation(kIdleStart, kIdleEnd, -1, 0, this, 10, false, false);
    this->addChild(sprite_);
   
    return true;
}

void GraphicNode::UpdateNode(float dt)
{    
    if (action_type_ != state_->CurrentAction().ActionType ||
        facing_ != state_->Facing())
    {
        action_type_ = state_->CurrentAction().ActionType;
        facing_ = state_->Facing();
        needs_update_ = true;
        ShowAnimation();
    }
    b2Vec2 position(
        (state_->Body()->GetPosition().x * PTM_RATIO) + offset_.x, 
        (state_->Body()->GetPosition().y * PTM_RATIO) + offset_.y);
    UpdatePositionAndRotation(position, state_->Body()->GetAngle());
}

void GraphicNode::Destroy() 
{ 
    if (state_->Body())
    {
        state_->Body()->GetWorld()->DestroyBody(state_->Body());
    }

    this->removeFromParentAndCleanup(true);
}

void GraphicNode::ResolveAttack()
{
    //if (state_->Alive() && 
    //    state_->CurrentAction().ActionType == kUnitActionTypeAttack)
    //{
    //    

    //    UnitNode* target = state_->CurrentAction().Target;
    //    if (target && target->GetState()->Alive())
    //    {
    //        //BattleLayer* battleLayer = static_cast<BattleLayer*>(this->getParent());
    //        //battleLayer->ResolveAttack(*this, *target);
    //        
    //        //b2Vec2 position(this->getPosition().x + this->getParent()->getPosition().x, this->getPosition().y + 60 + this->getParent()->getPosition().y);
    //        //b2Vec2 targetPosition(target->getPosition().x + this->getParent()->getPosition().x, target->getPosition().y + 60 + this->getParent()->getPosition().y);
    //        //MissileNode* node = battleLayer->GetBattle()->ShootMissile(0, 0, position, targetPosition, 1.0f, true);
    //        //battleLayer->addChild(node);
    //    }
    //}
    //state_->action_.Reset();
}

void GraphicNode::GetPositionAndRotation(b2Vec2& position, float& rotation)
{
    position.Set(this->getPosition().x * GFort::Core::Physics::kINV_PTM_RATIO,
        this->getPosition().y * GFort::Core::Physics::kINV_PTM_RATIO);
    rotation = -1 * CC_DEGREES_TO_RADIANS(this->getRotation());
}

void GraphicNode::UpdatePositionAndRotation(const b2Vec2& position, const float& rotation)
{    
    float rad = CC_RADIANS_TO_DEGREES(-1 * state_->Body()->GetAngle());
    this->setRotation(rad);           
    this->setPosition(ccp(position.x, position.y));  
}

void GraphicNode::ShowAnimation()
{
    if (needs_update_)
    {
        sprite_->setFlipX(facing_ == kFacingLeft);
        switch (action_type_)
        {
        case kUnitActionTypeIdle:
            sprite_->startAnimation(kIdleStart, kIdleEnd, -1, 0, this, 10, false, false);
            break;
        case kUnitActionTypeWalk:
            sprite_->startAnimation(kWalkStart, kWalkEnd, -1, 0, this, 10, false, false);  
            break;
        case kUnitActionTypeRun:
        case kUnitActionTypeMoveLeft:
        case kUnitActionTypeMoveRight:
            sprite_->startAnimation(kRunStart, kRunEnd, -1, 0, this, 10, false, false);
            break;
        case kUnitActionTypeAttack:
            sprite_->startAnimation(0, 5, 1, callfunc_selector(GraphicNode::ResolveAttack), this, 6, false, false);
            break;
        default:
            break;
        }
        needs_update_ = false;
    }
}

void GraphicNode::StopAnimation()
{
    sprite_->stopAnimaiton();
    needs_update_ = false;
}

} // namespace
