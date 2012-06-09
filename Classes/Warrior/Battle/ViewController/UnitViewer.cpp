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

#include "UnitViewer.h"
#include <string>
#include "../BattleHelper.h"

using namespace cocos2d;

namespace Warrior 
{

const std::string   kLabelActionFont                    = "Arial";
const int           kLabelActionFontSize                = 20;
    
UnitViewer::UnitViewer()
    : unit_(NULL),
      label_action_(NULL),
      alive_(false),
      current_action_type_(kUnitActionTypeIdle)
{
    this->scheduleUpdate();
    this->schedule(schedule_selector(UnitViewer::UpdateNode));

    //---------------------------------------------------------------
    // Labels
    //---------------------------------------------------------------
    label_action_ = cocos2d::CCLabelTTF::labelWithString(
        "X", 
        kLabelActionFont.c_str(), 
        kLabelActionFontSize);
        
    cocos2d::CCPoint pt = label_action_->getAnchorPoint();
    label_action_->setAnchorPoint(ccp(0.5, -4.0));
    this->addChild(label_action_, 1);
}

UnitViewer::~UnitViewer()
{
    this->unscheduleUpdate();
}

void UnitViewer::UpdateNode(cocos2d::ccTime dt)
{
    if (unit_)
    {
        if (alive_ != unit_->Alive() ||
            current_action_type_ != unit_->CurrentAction().ActionType)
        {
            alive_ = unit_->Alive();
            current_action_type_ = unit_->CurrentAction().ActionType;
            if (alive_)
                label_action_->setString(BattleHelper::GetAbbreviation(current_action_type_).c_str());
            else
                label_action_->setString("D");
        }
    }
}

} // namespace
