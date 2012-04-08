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

#include "Ship.h"
#include <GFort/Core/Physics/PhysicsHelper.h>
//#include "../Constants.h"
#include "../Game.h"

namespace GFort { namespace Games { namespace Shmup 
{

Ship::Ship(Game* game, const GameSide& side)
    : Unit("Unit", game, side)
    , physics_component_(NULL)
    , render_component_(NULL)
{
    //SetWeapon(-1);
}

Ship::~Ship()
{
}

void Ship::Move(const float& dx, const float& dy)
{
    // Gets the first physics component and do action
    if (physics_component_.size() > 0)
    {
        b2Body* body = physics_component_[0]->Body();
        b2Vec2 velocity = body->GetLinearVelocity();
        b2Vec2 impulse(dx - velocity.x, dy - velocity.y);
        impulse *= body->GetMass();
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter());
    }
}

void Ship::StopMove()
{
    // Gets the first physics component and do action
    if (physics_component_.size() > 0)
    {
        b2Body* body = physics_component_[0]->Body();
        b2Vec2 velocity = body->GetLinearVelocity();
        b2Vec2 impulse = 0.2f * velocity;
        impulse *= body->GetMass();
        body->ApplyLinearImpulse(-impulse, body->GetWorldCenter());
    }
}

//BPolygon Ship::GetBoundingRegion()
//{
//    if (physics_component_.size() > 0)
//    {
//        return BPolygon();
//    }
//    else if (render_component_.size() > 0)
//    {
//        return BPolygon();
//    }
//    else
//    {
//        return BPolygon();
//    }
//}

//void Ship::AddWeapon(WeaponReference weapon)
//{
//    weapons_.push_back(&weapon);
//    if (current_weapon_index_ == -1)
//        current_weapon_index_ = 0;
//}
//
//void Ship::RemoveWeapon(const short& index)
//{
//    if (!weapons_.empty() && 
//        index >= 0 &&
//        static_cast<WeaponList::size_type>(index) < weapons_.size())
//    {
//        weapons_.erase(weapons_.begin() + index);
//        if (current_weapon_index_ >= index)
//            SetWeapon(current_weapon_index_ - 1);
//    }
//}
//
//
//void Ship::RemoveAllWeapons()
//{
//    weapons_.clear();
//    SetWeapon(-1);
//}
void Ship::Die()
{
}

void Ship::Destroy()
{
}
    
} } } // namespace
