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
#ifndef SHMUP_CONSTANTS_H_
#define SHMUP_CONSTANTS_H_

#include <string>
#include <Box2D/Box2D.h>
#include <Cistron/Cistron.h>
#include "Components/PhysicsComponent.h"
#include "Components/RenderComponent.h"

namespace GFort { namespace Games { namespace Shmup 
{

typedef Cistron::ObjectId       ObjectId;
typedef b2Vec2                  Vector2;  
typedef b2World                 PhysicsWorld;
typedef short                   GameSide;

typedef GFGame::Components::PhysicsComponent    PhysicsComponent;
typedef GFGame::Components::RenderComponent     RenderComponent;
typedef PhysicsComponent*                       PhysicsComponentPtr;
typedef RenderComponent*                        RenderComponentPtr;
typedef std::vector<PhysicsComponent* >         PhysicsComponentPtrList;
typedef std::vector<RenderComponent* >          RenderComponentPtrList;

static const std::string        kSpriteBeam                 = "Assets/Shmup/laserbeam.png";
static const std::string        kSpriteAsteroid             = "Assets/Shmup/asteroid_sm.png";

// Name of components
static const std::string        kComponentUnit              = "Ship";
static const std::string        kComponentCore              = "Core";
static const std::string        kComponentMissile           = "Missile";
static const std::string        kComponentWeapon            = "Weapon";

/// Mask Bits and Category Bits. They are classified into the following types
///    Boundary - Boundary of the battle scene
///    Player   - Flight of Players. They could collide themselves
///    Missile  - Any missiles/laser/etc shoot from objects
///    Obstacle - any other stuff
///    
/// Collision mapping   
///               Boundary | Player | Missile | Obstacle
///    Boundary | Y | Y | N | N
///    Player   | Y | Y | Y | Y
///    Missile  | N | Y | Y | Y
///    Obstacle | N | Y | Y | Y
enum EntityB2Category 
{
    BOUNDARY            = 0x0001,
    FRIENDLY            = 0x0002,
    ENEMY               = 0x0004,
    SHIP                = 0x0008,
    MISSILE             = 0x0010,
    OBSTACLE            = 0x0020,
};

static const uint16&       kCategoryBitsBoundary       = BOUNDARY;
static const uint16&       kMaskBitsBoundary           = 0xFFFF;

static const uint16&       kCategoryBitsPlayer         = FRIENDLY | SHIP;
static const uint16&       kMaskBitsPlayer             = BOUNDARY | ENEMY | OBSTACLE;

static const uint16&       kCategoryBitsEnemy          = ENEMY | SHIP;
static const uint16&       kMaskBitsEnemy              = BOUNDARY | FRIENDLY | OBSTACLE;

static const uint16&       kCategoryBitsMissile        = FRIENDLY | MISSILE;
static const uint16&       kMaskBitsMissile            = ENEMY | SHIP | OBSTACLE;

static const uint16&       kCategoryBitsObstacle       = OBSTACLE;
static const uint16&       kMaskBitsObstacle           = FRIENDLY | ENEMY | SHIP | MISSILE | OBSTACLE;


} } } // namespace

#endif // SHMUP_CONSTANTS_H_
