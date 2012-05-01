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
#include "ShipNode.h"
namespace GFort { namespace Games { namespace Shmup 
{

#ifndef DEGTORAD
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#endif

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
        
        float bodyAngle = body->GetAngle();
        float desiredAngle = atan2f(-dx, dy);

        float totalRotation = desiredAngle - bodyAngle;
        while (totalRotation < -180 * DEGTORAD) totalRotation += 360 * DEGTORAD;
        while (totalRotation >  180 * DEGTORAD) totalRotation -= 360 * DEGTORAD;

        float change = 1 * DEGTORAD; //allow 1 degree rotation per time step
        float newAngle = bodyAngle + min(change, max(-change, totalRotation));
        body->SetAngularVelocity(0);
        body->SetTransform(body->GetPosition(), newAngle);

        UpdateDrive(sqrt(dx*dx+dy*dy));
    }
}

void Ship::StopMove()
{
    //// Gets the first physics component and do action
    //if (physics_component_.size() > 0)
    //{
    //    b2Body* body = physics_component_[0]->Body();
    //    b2Vec2 velocity = body->GetLinearVelocity();
    //    b2Vec2 impulse = 0.2f * velocity;
    //    impulse *= body->GetMass();
    //    body->ApplyLinearImpulse(-impulse, body->GetWorldCenter());
    //}
}

Vector2 Ship::Forward()
{
    if (physics_component_.size() > 0)
        return physics_component_[0]->Body()->GetWorldVector(b2Vec2(0, 1));
    else
        return Vector2(0, 0);
}

void Ship::UpdateDrive(const float& delta)
{
    const float m_maxForwardSpeed = 80;
    const float m_maxBackwardSpeed = 80;
    const float m_maxDriveForce = 1;
    const float m_currentTraction = 1;

    // Gets the first physics component and do action
    if (physics_component_.size() > 0)
    {
        b2Body* body = physics_component_[0]->Body();
        float desiredSpeed = 0;
        if (delta > 0)
            desiredSpeed = m_maxForwardSpeed;
        else if (delta < 0)
            desiredSpeed = -m_maxBackwardSpeed;
        else
            return;        
        
        // Find current speed in forward direction
        b2Vec2 currentForwardNormal = body->GetWorldVector(b2Vec2(0, 1));
        b2Vec2 currentForwardVelocity = b2Dot(currentForwardNormal, body->GetLinearVelocity()) * currentForwardNormal;
        float currentSpeed = b2Dot(currentForwardVelocity, currentForwardNormal);

        // Apply necessary force
        float force = 0;
        if (desiredSpeed > currentSpeed)
            force = m_maxDriveForce;
        else if (desiredSpeed < currentSpeed)
            force = -m_maxDriveForce;
        else
            return;

        float magitude = 0.5;
        b2Vec2 velocity = body->GetLinearVelocity();
        body->ApplyLinearImpulse(force * magitude * currentForwardNormal, body->GetWorldCenter());
        body->SetLinearDamping(5);
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

void Ship::Update(const float& dt)
{
    if (render_component_.size() > 0 && physics_component_.size() > 0)
    {
        Vector2 position;
        float rotation;
        physics_component_[0]->GetPositionAndRotation(position, rotation);
        render_component_[0]->UpdatePositionAndRotation(position, rotation);
    }
}
    
} } } // namespace
