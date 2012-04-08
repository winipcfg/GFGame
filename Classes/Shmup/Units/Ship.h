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
#ifndef GFGAME_SHMUP_SHIP_H_
#define GFGAME_SHMUP_SHIP_H_

#include <vector>
#include <Cistron/Cistron.h>
#include <GFort/Core/Game/IEntity.h>
#include <GFort/Core/Game/IWeapon.h>
#include "Components/PhysicsComponent.h"
#include "Components/RenderComponent.h"
//#include "../Constants.h"
#include "../Game.h"
#include "Unit.h"

namespace GFort { namespace Games { namespace Shmup 
{

/// The core component of an entity which generates and stores energy.
/// Each entity has single core
class Core : public Cistron::Component
{
    short energy_;
};

class ShipPart : public Cistron::Component
{
    short energy_consumption_;
};

/// The entities that are controlled by Players
class Ship : public Unit
{
public:
    //typedef GFort::Core::Game::IWeapon&             WeaponReference;
    //typedef GFort::Core::Game::IWeapon*             WeaponPtr;
    //typedef std::vector<WeaponPtr >                 WeaponList;
    typedef GFGame::Components::PhysicsComponent    PhysicsComponent;
    typedef GFGame::Components::RenderComponent     RenderComponent;
    typedef PhysicsComponent*                       PhysicsComponentPtr;
    typedef RenderComponent*                        RenderComponentPtr;
    typedef std::vector<PhysicsComponent* >         PhysicsComponentPtrList;
    typedef std::vector<RenderComponent* >          RenderComponentPtrList;
    
    typedef Unit                                    Component;
    typedef std::vector<Component* >                Components;

public:
    /// Constructor.
    Ship(Game* game, const GameSide& side);

    /// Destructor.
    ~Ship();

    ///// Add new part to the object.
    ///// @param weapon
    //void AddPart(WeaponReference weapon);

    ///// Remove a part from the object
    ///// @param index
    //void RemovePart(const short& index);
    
    ///// Remove all parts
    //void RemoveAllParts();
    
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

    ///// Gets the bounding region.
    //BPolygon GetBoundingRegion();


private:
    // Components
    Core*                   core_;
    Components              components_;

    PhysicsComponentPtrList physics_component_;
    RenderComponentPtrList  render_component_;

    friend class Game;
};

} } } // namespace

#endif // GFGAME_SHMUP_SHIP_H_
