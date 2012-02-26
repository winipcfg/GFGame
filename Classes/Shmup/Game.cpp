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

#include "Game.h"
#include <GFort/Core/Physics/Box2dSettings.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include "../Components/RenderComponent.h"
#include "../Components/PhysicsComponent.h"
#include "Units/Ship.h"
#include "Units/ShipNode.h"
#include "Units/MissileNode.h"

namespace GFort { namespace Games { namespace Shmup 
{



Game::Game()
    : num_lives_(0)
    , max_num_lives_(0)
    , game_over_(false)
    , is_shooting_(false)
{
}

bool Game::Initialize()
{
    // Physics World
    float32 width = 480;
	float32 height = 320;
		
    b2World* world = (b2World*) phys_controller_.World();
    world->SetContinuousPhysics(true);    

    short boundary = 20;
    GFort::Core::Physics::PhysicsHelper::CreateBoundedArea(
        world,
        b2Vec2(boundary, boundary), 
        width - boundary * 2,           
        height - boundary * 2);   

    // Game Setup
    

    return true;
}

bool Game::Initialize(const std::string& filePath)
{
    return true;
}

bool Game::DoHousekeep()
{
    return true;
}

void Game::ExitGame()
{
}

Cistron::ObjectId Game::SpawnPlayerShip(const b2Vec2& position, const short& side)
{
    Cistron::ObjectId objId = this->SpawnEntity();
    Ship* unit = new Ship();
    this->addComponent(objId, unit);

    // Render Component
    ShipNode* ship = new ShipNode();    
    ship->init();
    ship->setPosition(ccp(position.x, position.y));      
    GFGame::Components::RenderComponent* node = new GFGame::Components::RenderComponent(ship);
    this->addComponent(objId, node);

    // Physics Component
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        b2Vec2(ship->getPosition().x, ship->getPosition().y),
        ship->boundingBox().size.width,
        ship->boundingBox().size.height);

 //   // Set the dynamic body fixture.
	//b2Fixture* fixture = body->GetFixtureList();	
	//fixture[0].SetDensity(1.0f);
	//fixture[0].SetFriction(1.0f);
	//fixture[0].SetRestitution(0.0f);	
	//body->ResetMassData();

 //   b2Filter filter = fixture[0].GetFilterData();
 //   filter.categoryBits = 0x0002;
 //   filter.maskBits = 0x0001;
 //   fixture[0].SetFilterData(filter);

 //   physics->AddBody("root", body);
 //   node->SetBody(body);
 //   this->addComponent(objId, physics);

    return objId;
}

Cistron::ObjectId Game::SpawnLaser(const b2Vec2& position, const short& side)
{
    Cistron::ObjectId objId = this->SpawnEntity();
    Ship* unit = new Ship();
    this->addComponent(objId, unit);

    // Render Component
    MissileNode* missile = new MissileNode();    
    missile->init();
    missile->setPosition(ccp(position.x, position.y));      
    GFGame::Components::RenderComponent* node = new GFGame::Components::RenderComponent(missile);
    this->addComponent(objId, node);

    // Physics Component
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        b2Vec2(missile->getPosition().x, missile->getPosition().y),
        missile->boundingBox().size.width,
        missile->boundingBox().size.height);

    return objId;
}

void Game::Update(const float& dt)
{
    phys_controller_.Step(&phys_settings_, dt);
}

} } } // namespace
