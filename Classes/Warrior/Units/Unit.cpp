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

#include "Unit.h"
#include <GFort/Core/Macros.h>
#include <Warrior/Model/BattleHelper.h>

namespace Warrior 
{  
    
Unit::Unit()
    : Cistron::Component("Unit"),
      side_(0),
      lives_(0),
      stamina_(0),
      facing_(kFacingRight),
      last_attacker_(NULL),
      motion_updated_by_phys_(false),
      body_(NULL)
{
    //Cistron::ObjectId objId = this->getObjectManager()->createObject();
    properties_ = new UnitProperties();
    //this->getObjectManager()->addComponent(objId, properties_);
}

Unit::~Unit()
{
    SAFE_DELETE(properties_);
}

const bool Unit::TakeDamage(Unit* attacker, const short& damage)
{
    last_attacker_ = attacker;
    lives_ -= damage;
    if (!Alive())
    {
        this->action_.Reset();
        this->pending_actions_.clear();
        this->Notify();
    }
    return Alive();
}

void Unit::Die()                                      
{ 
    side_ = 0;
    lives_ = 0; 
    stamina_ = 0;
    this->action_.Reset();
    this->pending_actions_.clear();
    this->Notify();
}

void Unit::Reset()
{
    //max_lives_ = 0;
    //walk_speed_ = 0;
    //run_speed_ = 0;
    lives_ = 1;
    action_.Reset();
    facing_ = kFacingRight;
    this->action_.Reset();
    this->pending_actions_.clear();
}

void Unit::Update(const float& dt)
{
    // Pop pending action if it is idle and there are pending actions
    if (this->action_.ActionType == kUnitActionTypeIdle &&
        !this->pending_actions_.empty())
    {   
        this->action_ = this->pending_actions_.front();
        this->pending_actions_.pop_front();   
        this->Notify();
    }
}

BPolygon Unit::GetBoundingRegion() const
{
    if (body_)
        return BattleHelper::GetPolygon(*body_);
    else
        return BPolygon();
}

void Unit::IssueCommand(const UnitAction& action, const bool& replace)
{
    if (replace)
    {
        if (this->action_.Interruptible)
        {
            this->action_ = action;
            this->pending_actions_.clear();
            this->Notify();
        }
    }
    else
    {
        this->pending_actions_.push_back(action); 
    }
}
    
} // namespace
