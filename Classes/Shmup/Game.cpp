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
#include "Units/Asteroid.h"
#include "Units/BaseNode.h"
#include "Units/ShipNode.h"
#include "Units/MissileNode.h"
//#include "Constants.h"
#include "Weapons/BeamGun.h"

namespace GFort { namespace Games { namespace Shmup 
{



Game::Game()
    : num_lives_(0)
    , max_num_lives_(0)
    , game_over_(false)
    , is_shooting_(false)
{
}

Game::~Game()
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
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBoundedArea(
        world,
        b2Vec2(boundary, boundary), 
        width - boundary * 2,           
        height - boundary * 2);   

    // Set the dynamic body fixture.
    b2Fixture* fixture = body->GetFixtureList();	
    b2Filter filter = fixture[0].GetFilterData();
    filter.categoryBits = kCategoryBitsBoundary;
    filter.maskBits = kMaskBitsBoundary;
    fixture[0].SetFilterData(filter);

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

Ship* Game::SpawnPlayerShip(const Vector2& position, const GameSide& side)
{
    ObjectId objId = this->SpawnEntity();
    Ship* unit = new Ship(this, side);
    this->addComponent(objId, unit);

    // Render Component
    ShipNode* ship = new ShipNode();    
    ship->init();
    ship->setScale(0.5f);
    ship->setPosition(position.x, position.y);
    GFGame::Components::RenderComponent* render = new GFGame::Components::RenderComponent(ship);
    this->addComponent(objId, render);
    unit->render_component_.push_back(render);

    // Physics Component
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        position,
        ship->boundingBox().size.width * ship->getScaleX(),
        ship->boundingBox().size.height * ship->getScaleY());
    body->SetFixedRotation(true);

    // Set the dynamic body fixture.
    b2Fixture* fixture = body->GetFixtureList();	
    fixture[0].SetDensity(1.0f);
    fixture[0].SetFriction(1.0f);
    fixture[0].SetRestitution(0.0f);	
    body->ResetMassData();

    b2Filter filter = fixture[0].GetFilterData();
    if (side == 0)
    {
        filter.categoryBits = kCategoryBitsPlayer;
        filter.maskBits = kMaskBitsPlayer;
    }
    else
    {
        filter.categoryBits = kCategoryBitsEnemy;
        filter.maskBits = kMaskBitsEnemy;
    }
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body);
    ship->SetBody(body);
    this->addComponent(objId, physics);
    unit->physics_component_.push_back(physics);


    //####
    //####TEST
    //####
        // Render Component
    ship = new ShipNode();    
    ship->init();
    ship->setScale(0.5f);
    ship->setPosition(position.x, position.y);
    render = new GFGame::Components::RenderComponent(ship);
    this->addComponent(objId, render);
    unit->render_component_.push_back(render);

    // Physics Component
    physics = new GFGame::Components::PhysicsComponent();
    b2Body* body2 = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        b2Vec2(position.x, position.y+30),
        ship->boundingBox().size.width * ship->getScaleX()*2,
        ship->boundingBox().size.height * ship->getScaleY()*0.5);
    body2->SetFixedRotation(false);

    // Set the dynamic body fixture.
    fixture = body2->GetFixtureList();	
    fixture[0].SetDensity(1.0f);
    fixture[0].SetFriction(1.0f);
    fixture[0].SetRestitution(0.0f);	
    body2->ResetMassData();

    filter = fixture[0].GetFilterData();
    if (side == 0)
    {
        filter.categoryBits = kCategoryBitsPlayer;
        filter.maskBits = kMaskBitsPlayer;
    }
    else
    {
        filter.categoryBits = kCategoryBitsEnemy;
        filter.maskBits = kMaskBitsEnemy;
    }
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body2);
    ship->SetBody(body2);
    this->addComponent(objId, physics);
    unit->physics_component_.push_back(physics);


    //b2PrismaticJointDef jointDef;
    //b2Vec2 worldAxis(-1.0f, 1.0f);
    //jointDef.Initialize(body, body2, body->GetWorldCenter(), worldAxis);
    //jointDef.lowerTranslation = 0.0f;
    //jointDef.upperTranslation = 1.5f;
    //jointDef.enableLimit = true;
    //jointDef.maxMotorForce = 1.0f;
    //jointDef.motorSpeed = 1.0f;
    //jointDef.enableMotor = true;


    b2RevoluteJointDef jointDef;
    jointDef.Initialize(body, body2, body->GetWorldCenter());
    jointDef.lowerAngle = -0.2f * b2_pi; // -90 degrees
    jointDef.upperAngle = 0;//0.2f * b2_pi; // 45 degrees
    jointDef.enableLimit = true;
    jointDef.maxMotorTorque = 10.0f;
    jointDef.motorSpeed = 10.0f;
    jointDef.enableMotor = true;

    //b2DistanceJointDef jointDef;
    //jointDef.Initialize(body, body2, body->GetWorldCenter(), body2->GetWorldCenter());
    //jointDef.collideConnected = true;
    //jointDef.length = 1;
    //jointDef.frequencyHz = 1.0f;
    //jointDef.dampingRatio = 0.5f;

    b2Joint* joint = phys_controller_.World()->CreateJoint(&jointDef);

    //####TEST

    //// Weapon
    //BeamGun* weapon = new BeamGun();
    //this->addComponent(objId, weapon);

    return unit;
    //return objId;
}

ObjectId Game::SpawnLaser(const Vector2& position, const GameSide& side)
{
    ObjectId objId = this->SpawnEntity();
    Ship* unit = new Ship(this, side);
    this->addComponent(objId, unit);

    // Render Component
    MissileNode* graphic = new MissileNode();    
    graphic->init();
    graphic->setScale(0.5f);
    graphic->setPosition(position.x, position.y);      
    GFGame::Components::RenderComponent* node = new GFGame::Components::RenderComponent(graphic);
    this->addComponent(objId, node);

    // Physics Component
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        position,
        graphic->boundingBox().size.width * graphic->getScaleX(),
        graphic->boundingBox().size.height * graphic->getScaleY() * 0.5);
    body->SetFixedRotation(true);

    // Set the dynamic body fixture.
    b2Fixture* fixture = body->GetFixtureList();	
    b2Filter filter = fixture[0].GetFilterData();
    filter.categoryBits = kCategoryBitsMissile;
    filter.maskBits = kMaskBitsMissile;
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body);
    graphic->SetBody(body);
    this->addComponent(objId, physics);
    unit->physics_component_.push_back(physics);

    return objId;
}

Asteroid* Game::SpawnAsteroid(const Vector2& position, const GameSide& side)
{
    // Main
    ObjectId objId = this->SpawnEntity();
    Asteroid* unit = new Asteroid(this, -1);
    this->addComponent(objId, unit);

    // Render Component
    BaseNode* graphic = new BaseNode(kSpriteAsteroid);    
    graphic->setPosition(position.x, position.y);  
    graphic->setScale(0.5f);
    GFGame::Components::RenderComponent* node = new GFGame::Components::RenderComponent(graphic);
    this->addComponent(objId, node);

    // Physics Component
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateCircle(
        phys_controller_.World(),
        b2_dynamicBody,
        position,
        min(graphic->getContentSize().width, graphic->getContentSize().height) * 0.5 * graphic->getScale());
    body->SetFixedRotation(false);
    body->SetAngularDamping(0);
    body->SetAngularVelocity(10);

    // Set the dynamic body fixture.
    b2Fixture* fixture = body->GetFixtureList();	
    b2Filter filter = fixture[0].GetFilterData();
    filter.categoryBits = kCategoryBitsObstacle;
    filter.maskBits = kMaskBitsObstacle;
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body);
    graphic->SetBody(body);
    this->addComponent(objId, physics);
    unit->physics_component_ = physics;

    return unit;
    //return objId;
}

void Game::Update(const float& dt)
{
    phys_controller_.Step(&phys_settings_, dt);

    //// Get all weapons and do update
    //std::list<Cistron::Component*>::iterator it;
    //std::list<Cistron::Component*> list = this->getComponents(0, "Weapon");
    //for (it = list.begin(); it != list.end(); ++it)
    //{
    //    Weapon* weapon = static_cast<Weapon*>(*it);
    //    weapon->Update(dt);
    //}
}

} } } // namespace
