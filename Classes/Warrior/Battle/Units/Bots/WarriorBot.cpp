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

#include "WarriorBot.h"
#include "cocos2d.h"

#include <GFort/Core/MathHelper.h>
#include "../UnitAction.h"
#include "../Nodes/UnitNode.h"
#include "../../BattleLayer.h"


namespace Warrior 
{

WarriorBot::WarriorBot(UnitNode* node)
    : Bot(node)
{
}

WarriorBot::~WarriorBot()
{
}

bool IsTargetInFront(UnitNode& unit, UnitNode& target)
{
    // If diff is +ve, then target is on right hand side of the unit
    float diff = target.getPosition().x - unit.getPosition().x;
    if (unit.GetState()->Facing() == kFacingRight)
        return (diff >= 0) ? true : false;
    else
        return (diff >= 0) ? false : true;
}

void WarriorBot::Update(BattleLayer& gameState)
{
    if (node_)
    {
        PlayerNode* player = gameState.GetPlayerNode();
        cocos2d::CCPoint diff = cocos2d::ccpSub(player->getPosition(), node_->getPosition());
        //cocos2d::CCFloat distance = cocos2d::ccpLength(diff);

		float distance = 100;

        if (abs(diff.x) > distance)
        {
            // Chase player
            bool moveLeft;
            if (diff.x > 0)
                moveLeft = false;
            else if (diff.x < 0)
                moveLeft = true;
            else
                moveLeft = (GFort::Core::MathHelper::RandomBetween(0.0, 1.0) > 0.5);

            UnitAction action;
            action.ActionType = moveLeft ? kUnitActionTypeMoveLeft : kUnitActionTypeMoveRight;
            action.PositionX = player->getPosition().x;
            action.PositionY = player->getPosition().y;
            node_->GetState()->IssueCommand(action, true);
        }    
		else
		{
			// Attack player
            UnitAction action;
            action.ActionType = kUnitActionTypeAttack;
            action.PositionX = player->getPosition().x;
            action.PositionY = player->getPosition().y;
            action.Target = player;
            action.Interruptible = false;
            node_->GetState()->IssueCommand(action, true);
		}
    }
}

} // namepsace
