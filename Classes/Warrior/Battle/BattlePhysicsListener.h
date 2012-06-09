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

#pragma once
#ifndef WARRIOR_BATTLE_PHYSICS_LISTENER_H_
#define WARRIOR_BATTLE_PHYSICS_LISTENER_H_

#include <string>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <GFort/Core/Physics/Box2dSettings.h>
#include <GFort/Core/Physics/PhysicsController.h>

namespace Warrior 
{

class BattlePhysicsListener : public GFort::Core::Physics::PhysicsController
{
//public:
//    /// Do a single step.
//    /// @param settings
//    /// @param deltaTime
//	virtual void Step(GFort::Core::Physics::Box2dSettings* settings, const float32& deltaTime);

    friend class Battle;
};

} // namespace

#endif // WARRIOR_BATTLE_PHYSICS_LISTENER_H_
