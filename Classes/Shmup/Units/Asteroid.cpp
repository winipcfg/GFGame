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

#include "Asteroid.h"
#include <GFort/Core/Physics/PhysicsHelper.h>

namespace GFort { namespace Games { namespace Shmup 
{

void Asteroid::Move(const float& dx, const float& dy)
{
    if (physics_component_)
    {
        b2Body* body = physics_component_->Body();
        b2Vec2 velocity = body->GetLinearVelocity();
        b2Vec2 impulse(dx - velocity.x, dy - velocity.y);
        impulse *= body->GetMass();
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter());
    }
}

void Asteroid::StopMove()
{
    if (physics_component_)
    {
        b2Body* body = physics_component_->Body();
        b2Vec2 velocity = body->GetLinearVelocity();
        b2Vec2 impulse = 0.05f * velocity;
        impulse *= body->GetMass();
        body->ApplyLinearImpulse(-impulse, body->GetWorldCenter());
    }
}

BPolygon Asteroid::GetBoundingRegion()
{
    if (physics_component_)
    {
        
    }
    else if (render_component_)
    {
    }
    else
    {
        return BPolygon();
    }
}

void Asteroid::Update(const float& dt)
{

}
  
    
} } } // namespace
