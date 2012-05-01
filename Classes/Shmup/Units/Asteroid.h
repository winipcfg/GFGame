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
#ifndef GFGAME_SHMUP_ASTEROID_H_
#define GFGAME_SHMUP_ASTEROID_H_

#include <vector>
#include <Cistron/Cistron.h>
#include <GFort/Core/Game/IEntity.h>
#include <GFort/Core/Game/IWeapon.h>
#include "Unit.h"

namespace GFort { namespace Games { namespace Shmup 
{

class Asteroid : public Unit
{
public:
    /// Constructor
    Asteroid(Game* game, const GameSide& side);

    /// Destructor
    ~Asteroid()                             {}

    /// Kill the object.
    virtual void Die();

    /// Remove the object from Game.
    virtual void Destroy();

    /// Do movement.
    /// @param dx
    /// @param dy
    void Move(const float& dx, const float& dy);

    /// Stop movement.
    void StopMove();

    /// Gets the bounding region.
    BPolygon GetBoundingRegion();

    /// Update the object.
    /// @param dt
    virtual void Update(const float& dt);

private:
    // Components
    PhysicsComponent*   physics_component_;
    RenderComponent*    render_component_;

    friend class Game;
};

inline Asteroid::Asteroid(Game* game, const GameSide& side)
    : Unit("Obstacle", game, side)
{
}

inline void Asteroid::Die()
{
}

inline void Asteroid::Destroy()
{
}

} } } // namespace

#endif // GFGAME_SHMUP_ASTEROID_H_
