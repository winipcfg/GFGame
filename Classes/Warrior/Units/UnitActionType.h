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

#pragma once
#ifndef WARRIOR_UNIT_ACTION_TYPE_H_
#define WARRIOR_UNIT_ACTION_TYPE_H_

namespace Warrior 
{
    
/// Action type of units. Each unit has only 1 action.
enum UnitActionType
{
    /// The unit do nothing.
    kUnitActionTypeIdle,

    /// The unit do move.
    kUnitActionTypeWalk,

    /// The unit do move with faster speed.
    kUnitActionTypeRun,
    
    /// The unit do attack.
    kUnitActionTypeAttack,
    
    /// The unit is charging for powerful attack.
    kUnitActionTypeCharge, 

    /// The unit is charging for defense.
    kUnitActionTypeDefense, 

    /// The unit do move to left.
    kUnitActionTypeMoveLeft,

    /// The unit do move to right.
    kUnitActionTypeMoveRight,
};

} // namespace

#endif // WARRIOR_UNIT_ACTION_TYPE_H_
