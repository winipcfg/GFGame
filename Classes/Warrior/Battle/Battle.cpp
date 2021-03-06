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

#include "Battle.h"

#include <Box2D/Box2D.h>
#include <GFort/Core/Physics/Box2dSettings.h>
#include <GFort/Core/Physics/PhysicsHelper.h>
#include <GFort/Core/MathHelper.h>
#include "Units/Missile.h"
#include "Units/Nodes/GraphicNode.h"
#include "Units/EntityB2Category.h"

namespace Warrior 
{

const short kMapGravity             = -20;

const short kMapBoundary            = 20;
const float kImpulseStrength        = 1200.0f;
    
Battle::Battle()
    : battle_ended_(false)
{		
    phys_controller_.World()->SetGravity(b2Vec2(0, kMapGravity));
    //phys_controller_.SetDefaultGravity(); 	
    phys_controller_.World()->SetContinuousPhysics(true);                    
	
    this->Reset();
}


Battle::~Battle()
{
}

void Battle::Initialize()
{   
    battle_ended_ = false;
    b2Body* body;

    // Ground
    body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        b2_staticBody,
        b2Vec2(map_.Width() / 2, map_.Landscape() / 2),
        map_.Width() * 10,
        map_.Landscape());

    // Set the dynamic body fixture.
	b2Fixture* fixture = body->GetFixtureList();	
	fixture[0].SetFriction(1.0f);
	fixture[0].SetRestitution(0.0f);	

    b2Filter filter = fixture[0].GetFilterData();
    filter.categoryBits = kCategoryBitsGround;
    filter.maskBits = kMaskBitsGround;
    fixture[0].SetFilterData(filter);

    map_.Contents().push_back(body);
}
    
bool Battle::MouseDown(const b2Vec2& p)
{
    phys_controller_.MouseDown(p);        
    return false;
}

void Battle::MouseMove(const b2Vec2& p)
{
    phys_controller_.MouseMove(p);	
}

void Battle::MouseUp(const b2Vec2& p)
{
    phys_controller_.MouseUp(p);	
}

void Battle::Reset()
{
    Initialize();
}

void Battle::DoSlice(const Trail& trail, std::vector<Unit>& affectedUnits)
{    
    BPolygon poly;
    std::set<Enemy* > damaged_enemies; 
    Enemy* enemy;
    std::vector<BTurnInfo> turns;
    //for (unsigned int j = 0; j < enemies_.size(); ++j)
    //{
    //    enemy = enemies_[j];
    //    if (enemy->Alive())
    //    {         
    //        turns.clear();
    //        poly = enemy->GetBoundingRegion();
    //        if (trail.Within(poly) || trail.Collide(poly, turns))
    //        {
    //            damaged_enemies.insert(enemy);

    //            float dx = trail.GetEndPosition().x() - turns[0].point.x();
    //            float dy = trail.GetEndPosition().y() - turns[0].point.y();
    //            float distance = boost::geometry::distance(trail.GetEndPosition(), turns[0].point);

    //            b2Vec2 impulse = b2Vec2(kImpulseStrength * dx / distance, kImpulseStrength * dy / distance);
    //            b2Vec2 center = enemy->Body()->GetWorldCenter();
    //    
    //            ApplyForceOnUnit(*enemy, turns[0].point, impulse);
    //        }
    //    }
    //}

    //if (damaged_enemies.size() > 0)
    //{
    //    std::set<Enemy* >::iterator it;
    //    for (it = damaged_enemies.begin(); it != damaged_enemies.end(); ++it)
    //    {
    //        static_cast<Enemy* >(*it)->Die();
    //    }

    //    stat_.TotalKills += damaged_enemies.size();

    //}
}

void Battle::Update(const float& dt)
{
    // Update physics layer and do collision checking
    phys_controller_.Step(&phys_settings_, dt);
    for (int i = 0; i < phys_controller_.num_contact_points_; ++i)
    {
        void* userDataA = phys_controller_.contact_points_[i].fixtureA->GetBody()->GetUserData();
        void* userDataB = phys_controller_.contact_points_[i].fixtureB->GetBody()->GetUserData();
        if (userDataA && userDataB)
        {
            //Cistron::Component* missile;
            //missile = static_cast<Cistron::Component*>(userDataA);
            //if (missile)
            //{
            //    string name = missile->getName();
            //    this->destroyObject(missile->getId());
            //}
            int i=0;
        }
    }

    // Update all units
    std::list<Cistron::Component*>::iterator it;
    std::list<Cistron::Component*> components = this->getComponentsByName("Unit");
    for (it = components.begin(); it != components.end(); ++it)
    {
        static_cast<Unit*>(*it)->Update(dt);
    }

    // Graphic Node
    components = this->getComponentsByName("UnitComponent");
    for (it = components.begin(); it != components.end(); ++it)
    {
        static_cast<GraphicNode*>(*it)->UpdateNode(dt);
    }

    // Remove objects that are pending to destroy
    if (objects_pending_destroyed_.size() > 0)
    {
        Cistron::ObjectId objId;        
        objId = objects_pending_destroyed_.front();
        this->destroyObject(objId);
        objects_pending_destroyed_.pop_front();
    }
}

void Battle::ResolveAttack(Unit& attacker, Unit& target)
{
    target.TakeDamage(&attacker, 1);
}

void Battle::SpawnEnemy(const b2Vec2& position)
{

}

MissileNode* Battle::ShootMissile(
    const Cistron::ObjectId& owner,
    const short missileType,
    const b2Vec2& position, 
    const b2Vec2& targetPosition, 
    const bool& duration,
    const bool& projectile)
{
    Cistron::ObjectId objId = this->SpawnEntity();
    Missile* unit = new Missile();
    this->addComponent(objId, unit);

    MissileNode* node = new MissileNode();    
    node->init();
    node->setPosition(ccp(position.x, position.y));      
    this->addComponent(objId, node);
    //this->addChild(node);

    // Create body
    GFGame::Components::PhysicsComponent* physics = new GFGame::Components::PhysicsComponent();
    b2Body* body = GFort::Core::Physics::PhysicsHelper::CreateBox(
        phys_controller_.World(),
        b2Vec2(node->getPosition().x, node->getPosition().y),
        node->boundingBox().size.width,
        node->boundingBox().size.height);
    body->SetBullet(true);
    body->SetUserData(unit);

    // Set the dynamic body fixture.
	b2Fixture* fixture = body->GetFixtureList();	
	fixture[0].SetDensity(1.0f);
	fixture[0].SetFriction(1.0f);
	fixture[0].SetRestitution(0.0f);	
	body->ResetMassData();

    b2Filter filter = fixture[0].GetFilterData();
    //filter.categoryBits = 0x0002;
    //filter.maskBits = 0x0001;
    filter.categoryBits = kCategoryBitsMissile;
    filter.maskBits = kMaskBitsMissile;
    fixture[0].SetFilterData(filter);

    physics->AddBody("root", body);
    node->SetBody(body);
    this->addComponent(objId, physics);
    
    // Apply force
    b2Vec2 impulse = GFort::Core::Physics::PhysicsHelper::GetTrajectoryVelocity(
        phys_controller_.World(),
        &this->phys_settings_,
        position,
        targetPosition,
        duration);
    body->ApplyLinearImpulse(body->GetMass() * impulse, body->GetWorldCenter());
    
    return node;
}

bool Battle::MeetsWinCondition()
{
    // If all enemies are destroyed
    return true;
}

bool Battle::MeetsLossCondition()
{
    // If player is dead
    return true;
}    

} // namespace
